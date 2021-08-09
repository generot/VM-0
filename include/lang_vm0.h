#ifndef __LANG_VM0_H__
#define __LANG_VM0_H__

#include "vm_bytecode.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <regex>
#include <map>

constexpr const char* 
    LEX_REG = "[a-zA-Z0-9]+(?:\\.\\d+)?|[,;{}?+\\-*\\/\\(\\)]|[:=<>!&|]{1,2}|\".*\"";

constexpr const char* 
    ENTRY_POINT = "main";

using token_vector = std::vector<std::string>;
using token_iter = token_vector::iterator;

enum symbol_type { SYM_VARIABLE, SYM_FUNCTION };

class lexer {
private:
    std::string filepath;

    token_vector tokens;
    token_iter curr;
    size_t ix;

public:
    lexer(token_vector t_vec, std::string filedir);

    bool expect(std::string tok, bool kill_prog = true);
    bool eof(void);

    void unaccept(void);
    std::string accept(void);
    std::string peek(void);
    std::string file_path(void);

    void show_tokens(void);
};

struct symbol {
    symbol_type stype;

    std::string ident;
    size_s addr;
};

lexer lex_file(char *filename);
opcode match_comp(std::string strrep);
bool is_numeric(std::string str);
void dump_program(std::string filename, std::vector<instr> prog, size_s entry_addr);

//Parser
void program(lexer& lx);
void include(lexer& lx);
void decl(lexer& lx);
void procedure(lexer& lx);
void block(lexer& lx);
void statement(lexer& lx);
void assign(lexer& lx);
void cond_if(lexer& lx);
void cond_while(lexer& lx);
void call(lexer& lx);
void inpout(lexer& lx);
void expr(lexer& lx);
void comp(lexer& lx);
void term(lexer& lx);
void factor(lexer& lx);
void value(lexer& lx);

#endif //__LANG_VM0_H__
