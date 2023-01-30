#include "preprocessor.h"
#include "helpers.h"
#include "rom.h"
#include "shared.h"
#include <algorithm>
#include <cstring>
#include <exception>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>

Preprocessor::Preprocessor() {}

Preprocessor preprocessor;

void Preprocessor::handle_new_label(char *text) {
  std::cout << "[PRE] new label: " << text << "\n";
  this->current_label = std::string(text);
}

std::map<std::string, Opcode> opcodes = {
    {"nop", Opcode::NOP},     {"hlt", Opcode::HLT},
    {"movi", Opcode::MOVI},   {"movr", Opcode::MOVR},
    {"mov16a", Opcode::MOV16A},   {"mov16r", Opcode::MOV16R},
    {"pushi", Opcode::PUSHI}, {"pushr", Opcode::PUSHR},
    {"popr", Opcode::POPR},   {"popa", Opcode::POPA},
    {"sioi", Opcode::SIOI},   {"sior", Opcode::SIOR},
    {"rioia", Opcode::RIOIA}, {"rioir", Opcode::RIOIR},
    {"riora", Opcode::RIORA}, {"riorr", Opcode::RIORR},
    {"wioai", Opcode::WIOAI}, {"wioar", Opcode::WIOAR},
    {"wiori", Opcode::WIORI}, {"wiorr", Opcode::WIORR},
    {"jmpa", Opcode::JMPA},   {"jmpr", Opcode::JMPR},
    {"shli", Opcode::SHLI},   {"shlr", Opcode::SHLR},
    {"shri", Opcode::SHRI},   {"shrr", Opcode::SHRR},
};

typedef struct pseudo_predicate {
  int arg_pos;
  Preprocessor::ArgType arg_type;
  Opcode opcode;
  struct pseudo_predicate *requirement = nullptr;
} pseudo_predicate_t;

typedef struct pseudo {
  PseudoOpcode opcode;
  std::list<pseudo_predicate_t> predicates;
  int must_match_n_predicates = 1;
} pseudo_t;

// RIO/WIO is more complicated than other pseudo-opcodes
pseudo_predicate_t rio_i = {0, Preprocessor::ArgType::NUMBER, Opcode::RIOIA};
pseudo_predicate_t rio_r = {0, Preprocessor::ArgType::REGISTER, Opcode::RIORA};
pseudo_predicate_t wio_a = {0, Preprocessor::ArgType::ADDRESS, Opcode::WIOAI};
pseudo_predicate_t wio_r = {0, Preprocessor::ArgType::REGISTER, Opcode::WIORI};

pseudo_predicate_t empty_predicate = {0, Preprocessor::ArgType::ADDRESS,
                                      Opcode::NOP};

std::map<std::string, pseudo_t> pseudoOpcodes = {
    {"mov",
     {PseudoOpcode::MOV,
      {{0, Preprocessor::ArgType::NUMBER, Opcode::MOVI},
       {0, Preprocessor::ArgType::REGISTER, Opcode::MOVR}}}},
    {"mov16",
     {PseudoOpcode::MOV16,
      {{0, Preprocessor::ArgType::ADDRESS, Opcode::MOV16A},
       {0, Preprocessor::ArgType::REGISTER, Opcode::MOV16R}}}},
    {"add",
     {PseudoOpcode::ADD,
      {{0, Preprocessor::ArgType::NUMBER, Opcode::ADDI},
       {0, Preprocessor::ArgType::REGISTER, Opcode::ADDR}}}},
    {"sub",
     {PseudoOpcode::SUB,
      {{0, Preprocessor::ArgType::NUMBER, Opcode::SUBI},
       {0, Preprocessor::ArgType::REGISTER, Opcode::SUBR}}}},
    {"push",
     {PseudoOpcode::PUSH,
      {{0, Preprocessor::ArgType::NUMBER, Opcode::PUSHI},
       {0, Preprocessor::ArgType::REGISTER, Opcode::PUSHR}}}},
    {"pop",
     {PseudoOpcode::POP,
      {{0, Preprocessor::ArgType::REGISTER, Opcode::POPR},
       {0, Preprocessor::ArgType::ADDRESS, Opcode::POPA}}}},
    {"sio",
     {PseudoOpcode::SIO,
      {{0, Preprocessor::ArgType::NUMBER, Opcode::SIOI},
       {0, Preprocessor::ArgType::REGISTER, Opcode::SIOR}}}},
    {"rio",
     {PseudoOpcode::RIO,
      {rio_i,
       rio_r,
       {1, Preprocessor::ArgType::ADDRESS, Opcode::RIOIA, &rio_i},
       {1, Preprocessor::ArgType::ADDRESS, Opcode::RIORA, &rio_r},
       {1, Preprocessor::ArgType::REGISTER, Opcode::RIOIR, &rio_i},
       {1, Preprocessor::ArgType::REGISTER, Opcode::RIORR, &rio_r}},
      2}},
    {"wio",
     {PseudoOpcode::WIO,
      {wio_a,
       wio_r,
       {1, Preprocessor::ArgType::NUMBER, Opcode::WIOAI, &wio_a},
       {1, Preprocessor::ArgType::NUMBER, Opcode::WIORI, &wio_r},
       {1, Preprocessor::ArgType::REGISTER, Opcode::WIOAR, &wio_a},
       {1, Preprocessor::ArgType::REGISTER, Opcode::WIORR, &wio_r}},
      2}},
    {"jmp",
     {PseudoOpcode::JMP,
      {{0, Preprocessor::ArgType::ADDRESS, Opcode::JMPA},
       {0, Preprocessor::ArgType::REGISTER, Opcode::JMPR}}}},
    {"shl",
     {PseudoOpcode::SHL,
      {{0, Preprocessor::ArgType::NUMBER, Opcode::SHLI},
       {0, Preprocessor::ArgType::REGISTER, Opcode::SHLR}}}},
    {"shr",
     {PseudoOpcode::SHR,
      {{0, Preprocessor::ArgType::NUMBER, Opcode::SHRI},
       {0, Preprocessor::ArgType::REGISTER, Opcode::SHRR}}}},
};

std::map<std::string, uint8_t> registers = {
    {"a", 0b000001},  {"b", 0b000010},  {"c", 0b000100},  {"d", 0b001000},
    {"ba", 0b000011}, {"dc", 0b001100}, {"pc", 0b010000}, {"sp", 0b100000},
};

/**
 * Handles the start of a new instruction.
 */
void Preprocessor::handle_opcode(char *text) {
  std::cout << "[PRE] new opcode: " << text << "\n";
  std::string opcode = std::string(text);

  if (!opcodes.contains(opcode) && !pseudoOpcodes.contains(opcode)) {
    throw std::invalid_argument(
        (std::stringstream() << "invalid opcode: " << text).str());
  }

  this->current_instruction = new inst_t;
  if (opcodes.contains(opcode))
    this->current_instruction->opcode = opcodes[opcode];
  if (pseudoOpcodes.contains(opcode))
    this->current_instruction->pseudoOpcode = pseudoOpcodes[opcode].opcode;
}

/**
 * Handles a LABEL preprocessor argument.
 */
void Preprocessor::handle_label_mention(char *text) {
  std::cout << "[PRE] label arg: " << text << "\n";
  this->current_instruction->arguments.push_back(
      new arg_t{ArgType::LABEL, std::string(text)});
}

/**
 * Handles a REGISTER preprocessor argument.
 */
void Preprocessor::handle_register(char *text) {
  std::cout << "[PRE] register arg: " << text << "\n";
  this->current_instruction->arguments.push_back(
      new arg_t{ArgType::REGISTER, std::string(text)});
}

/**
 * Handles an ADDRESS preprocessor argument.
 */
void Preprocessor::handle_address(char *text) {
  std::cout << "[PRE] address arg: " << text << "\n";
  this->current_instruction->arguments.push_back(
      new arg_t{ArgType::ADDRESS, std::string(text)});
}

/**
 * Handles a NUMBER preprocessor argument.
 */
void Preprocessor::handle_number(char *text) {
  std::cout << "[PRE] number arg: " << text << "\n";
  this->current_instruction->arguments.push_back(
      new arg_t{ArgType::NUMBER, std::string(text)});
}

/**
 * Handles the end of an instruction, pushing it to the labels map.
 */
void Preprocessor::handle_instruction() {
  std::cout << "[PRE] insn end\n";

  if (!labels.contains(current_label))
    labels[current_label] = std::vector<inst_t *>();

  labels[current_label].push_back(this->current_instruction);
}

void Preprocessor::build_intermediate_rom() {
  std::cout << "[PRE] intermediate ROM start\n";
  for (const auto &[label, inst_list] : labels) {
    std::cout << label << " has " << inst_list.size() << " instructions\n";

    if (!stage2_rom.contains(label))
      stage2_rom[label] = std::vector<uint8_t>();

    for (inst_t *inst : inst_list) {
      // transform inst_t into raw bytes
      if (inst->pseudoOpcode != PseudoOpcode::NONE) {
        // for every pseudo-opcode
        for (const auto &[_, pseudoData] : pseudoOpcodes) {
          // if the pseudo-opcode matches our opcode
          if (pseudoData.opcode == inst->pseudoOpcode) {
            int matched_n_predicates = 0;
            std::vector<int> matched_arg_pos = {};
            std::vector<pseudo_predicate_t> matched_predicates = {};
            // for every predicate of that pseudo-opcode
            for (pseudo_predicate_t predicate : pseudoData.predicates) {
              if (inst->arguments.size() < predicate.arg_pos + 1) {
                throw std::invalid_argument(
                    "pseudo-opcode has less arguments than expected");
              }
              ArgType type = inst->arguments[predicate.arg_pos]->type;
              if (type == ArgType::LABEL) {
                // labels are fancier addresses and should be handled as such
                type = ArgType::ADDRESS;
              }
              // if the argument type matches the predicate
              if (type == predicate.arg_type &&
                  std::find(matched_arg_pos.begin(), matched_arg_pos.end(),
                            predicate.arg_pos) == matched_arg_pos.end()) {
                // if the predicate has a requirement
                if (predicate.requirement != nullptr) {
                  // this is so cursed yet i felt like this is the easiest
                  // solution for me to write as of right now.
                  // yes it looks like absolute garbage
                  bool check_success = false;
                  for (pseudo_predicate_t r : matched_predicates) {
                    if (memcmp(predicate.requirement, &r,
                               sizeof(pseudo_predicate_t)) == 0) {
                      check_success = true;
                      break;
                    }
                  }
                  if (!check_success) {
                    continue;
                  }
                }
                // this is our opcode
                inst->opcode = predicate.opcode;
                inst->pseudoOpcode = PseudoOpcode::NONE;
                matched_n_predicates++;
                matched_arg_pos.push_back(predicate.arg_pos);
                matched_predicates.push_back(predicate);
              }
            }
            if (matched_n_predicates < pseudoData.must_match_n_predicates) {
              throw std::invalid_argument(
                  "pseudo-opcode has less arguments than expected (not enough "
                  "predicates matched)");
            }
          }
        }

        // if after all that we STILL don't have the right pseudo-opcode
        if (inst->pseudoOpcode != PseudoOpcode::NONE) {
          // bail
          throw std::invalid_argument(
              (std::stringstream()
               << "invalid pseudo-opcode "
               << std::to_string(static_cast<int>(inst->pseudoOpcode)))
                  .str());
        }
      }

      stage2_rom[label].push_back(static_cast<uint8_t>(inst->opcode));

      for (arg_t *arg : inst->arguments) {
        switch (arg->type) {
        case ArgType::NUMBER: {
          int value = std::stoi(arg->contents, nullptr, 0);
          stage2_rom[label].push_back(static_cast<uint8_t>(value));
          break;
        }
        case ArgType::LABEL: {
          stage2_rom[label].push_back(0);
          stage2_rom[label].push_back(0);
          stage2_rom_label_positions[label].push_back(new label_pos_t{
              static_cast<uint16_t>(stage2_rom[label].size() - 2),
              arg->contents});
          break;
        }
        case ArgType::REGISTER: {
          stage2_rom[label].push_back(registers[arg->contents]);
          break;
        }
        case ArgType::ADDRESS: {
          int value = std::stoi(arg->contents, nullptr, 0);
          uint16_to_8_t addr = split_uint16_to_8(static_cast<uint16_t>(value));
          stage2_rom[label].push_back(addr.lower);
          stage2_rom[label].push_back(addr.upper);
          break;
        }
        default: {
          std::cout << "[PRE] unknown arg type encountered: "
                    << static_cast<int>(arg->type) << "\n";
          throw std::exception();
        }
        }
      }
    }
  }

  std::cout << "[PRE] stage 2 processing done, stage 3 start\n";
  uint16_t offset = 0x8000; // start of lower ROM
  std::map<std::string, uint16_t> label_offsets;
  // now that we know the size of each label, we can start assigning label
  // offsets
  for (const auto &[label, insts] : stage2_rom) {
    std::cout << label << " offset: " << offset << "\n";
    label_offsets[label] = offset;
    offset += insts.size();
  }
  // then assign label addresses to their 0x0000 placeholders in the ROM
  for (const auto &[label, positions] : stage2_rom_label_positions) {
    for (label_pos_t *position : positions) {
      uint16_t label_offset = label_offsets[position->label];
      uint16_to_8_t label_offset_8 = split_uint16_to_8(label_offset);
      std::cout << label << " " << position->label << " "
                << std::to_string(label_offset_8.upper) << " "
                << std::to_string(label_offset_8.lower) << "\n";
      stage2_rom[label][position->position] = label_offset_8.lower;
      stage2_rom[label][position->position + 1] = label_offset_8.upper;
    }
  }
  // and now, finish stage 3 by dumping stage 2's raw data into its own list
  for (const auto &[_, values] : stage2_rom) {
    for (uint8_t val : values) {
      stage3_rom.push_back(val);
    }
  }
  // and if stage 3's data is smaller than 0x8000, fill it with 0xFFs
  if (stage3_rom.size() < 0x8000) {
    for (int i = stage3_rom.size(); i < 0x8000; i++) {
      stage3_rom.push_back(0xFF);
    }
  }
}

void Preprocessor::save_rom(std::string path) {
  build_and_write_rom(path, stage3_rom);
}

// Helper functions
void handle_new_label(char *text) { preprocessor.handle_new_label(text); }

void handle_opcode(char *text) { preprocessor.handle_opcode(text); }

void handle_label_mention(char *text) {
  preprocessor.handle_label_mention(text);
}
void handle_register(char *text) { preprocessor.handle_register(text); }
void handle_address(char *text) { preprocessor.handle_address(text); }
void handle_number(char *text) { preprocessor.handle_number(text); }
void handle_instruction() { preprocessor.handle_instruction(); }

Preprocessor &get_preprocessor() { return preprocessor; }
