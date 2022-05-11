#include "preprocessor.h"

#include <iostream>
#include <sstream>

Preprocessor::Preprocessor() {}

Preprocessor preprocessor;

void Preprocessor::handle_new_label(char *text) {
    std::cout << "[PRE] new label: " << text << "\n";
    this->current_label = std::string(text);
}

std::map<std::string, Opcode> opcodes = {
    { "nop", Opcode::NOP },
    { "mov", Opcode::MOV },
    { "mov16", Opcode::MOV16 },
    { "load", Opcode::LOAD },
    { "stor", Opcode::STOR },
    { "eq", Opcode::EQ },
    { "ne", Opcode::NE },
    { "lt", Opcode::LT },
    { "gt", Opcode::GT },
    { "le", Opcode::LE },
    { "ge", Opcode::GE },
    { "jmp", Opcode::JMP },
    { "jeq", Opcode::JEQ },
    { "jne", Opcode::JNE },
    { "jlt", Opcode::JLT },
    { "jgt", Opcode::JGT },
    { "jle", Opcode::JLE },
    { "jge", Opcode::JGE },
    { "sio", Opcode::SIO },
    { "rio", Opcode::RIO },
    { "wio", Opcode::WIO },
    { "add", Opcode::ADD },
    { "sub", Opcode::SUB },
    { "shl", Opcode::SHL },
    { "shr", Opcode::SHR },
    { "hlt", Opcode::HLT },
};

/**
 * Handles the start of a new instruction.
 */
void Preprocessor::handle_opcode(char *text) {
    std::cout << "[PRE] new opcode: " << text << "\n";
    if (!opcodes.contains(std::string(text))) throw std::invalid_argument((std::stringstream() << "invalid opcode: " << text).str());
    this->current_instruction = new inst_t;
    this->current_instruction->opcode = opcodes[std::string(text)];
}

/**
 * Handles a LABEL preprocessor argument.
 */
void Preprocessor::handle_label_mention(char *text) {
    std::cout << "[PRE] label arg: " << text << "\n";
    this->current_instruction->arguments.push_back(new arg_t { ArgType::LABEL, std::string(text) });
}

/**
 * Handles a REGISTER preprocessor argument.
 */
void Preprocessor::handle_register(char *text) {
    std::cout << "[PRE] register arg: " << text << "\n";
    this->current_instruction->arguments.push_back(new arg_t { ArgType::REGISTER, std::string(text) });
}

/**
 * Handles an ADDRESS preprocessor argument.
 */
void Preprocessor::handle_address(char *text) {
    std::cout << "[PRE] address arg: " << text << "\n";
    this->current_instruction->arguments.push_back(new arg_t { ArgType::ADDRESS, std::string(text) });
}

/**
 * Handles a NUMBER preprocessor argument.
 */
void Preprocessor::handle_number(char *text) {
    std::cout << "[PRE] number arg: " << text << "\n";
    this->current_instruction->arguments.push_back(new arg_t { ArgType::NUMBER, std::string(text) });
}

/**
 * Handles the end of an instruction, pushing it to the labels map.
 */
void Preprocessor::handle_instruction() {
    std::cout << "[PRE] insn end\n";
    if (!labels.contains(current_label)) labels[current_label] = std::list<inst_t *>();
    labels[current_label].push_back(this->current_instruction);
}

// Helper functions
void handle_new_label(char *text) {
    preprocessor.handle_new_label(text);
}

void handle_opcode(char *text) {
    preprocessor.handle_opcode(text);
}

void handle_label_mention(char *text) {
    preprocessor.handle_label_mention(text);
}
void handle_register(char *text) {
    preprocessor.handle_register(text);
}
void handle_address(char *text) {
    preprocessor.handle_address(text);
}
void handle_number(char *text) {
    preprocessor.handle_number(text);
}
void handle_instruction() {
    preprocessor.handle_instruction();
}

Preprocessor get_preprocessor() {
    return preprocessor;
}
