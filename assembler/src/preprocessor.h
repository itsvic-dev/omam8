#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "shared.h"
#include <string>
#include <map>
#include <list>

class Preprocessor {
public:
    explicit Preprocessor();
    enum class ArgType {
        LABEL,
        REGISTER,
        // IMMEDIATE, // might be used later idk
        ADDRESS,
        NUMBER,
    };

    typedef struct {
        ArgType type;
        std::string contents;
    } arg_t;

    typedef struct {
        Opcode opcode;
        std::list<arg_t *> arguments;
    } inst_t;

    void handle_new_label(char *text);
    void handle_opcode(char *text);
    void handle_label_mention(char *text);
    void handle_register(char *text);
    void handle_address(char *text);
    void handle_number(char *text);
    void handle_instruction();

private:
    inst_t *current_instruction = nullptr;
    std::string current_label = ".default";
    std::map<std::string, std::list<inst_t *>> labels;
};

void handle_new_label(char *text);
void handle_opcode(char *text);
void handle_label_mention(char *text);
void handle_register(char *text);
void handle_address(char *text);
void handle_number(char *text);
void handle_instruction();

Preprocessor get_preprocessor();

#endif // PREPROCESSOR_H
