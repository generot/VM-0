#include "../../include/lang_vm0.h"
#include <cctype>

using namespace std;

lexer::lexer(token_vector t_vec, string filedir) : filepath(filedir), tokens(t_vec), ix(0) {}

bool lexer::expect(string tok, bool kill_prog) {
    if(tokens[ix] != tok) {
        if(kill_prog)
            ISSUE_ERROR("Expected \"%s\", got \"%s\" instead.", tok.c_str(), tokens[ix].c_str());

        return false;
    }

    return true;
}

bool lexer::eof(void) {
    return tokens[ix] == "eof";
}

void lexer::unaccept(void) { ix--; }

string lexer::file_path(void) { 
    int i;

    for(i = (int)filepath.length() - 1; i > 0 && 
        (filepath[i] != '/' && filepath[i] != '\\'); i--);

    return string(filepath.substr(0, i)) + (i != 0 ? "/" : "");
}

string lexer::accept(void) { 
    if(!eof()) return tokens[ix++];

    return "null"; 
}

string lexer::peek(void) {
    if(tokens[ix + 1] != "eof")
        return tokens[ix + 1];

    return "null";  
}

void lexer::show_tokens(void) {
    for(string i : tokens) {
        cout << i << endl;
    }
}

opcode match_comp(string strrep) {
    string comps[] = { ">", "<", ">=", "<=", "==", "!=" };

    for(int i = 0; i < 6; i++) {
        if(strrep == comps[i])
            return opcode(OPC_GT + i);
    }

    return OPC_INVALID;
}

bool is_numeric(string str) {
    for(auto i = str.begin(); i < str.end(); i++) {
        if(!isdigit(*i) && *i != '.')
            return false;
    }

    return true;
}

void dump_program(string filename, vector<instr> prog, size_s entry_addr) {
    ofstream outf = ofstream(filename.c_str(), ios::out | ios::binary);

    bcode_header header;

    header.program_size = prog.size();
    header.stack_size_spec = bcode_header::STACK_SIZE_DEFAULT;
    header.entry_point_addr = (size_t)entry_addr;

    header.code_seg_sz = header.data_seg_sz = 0;

    outf.write((const char*)&header, sizeof(bcode_header));
    outf.write((const char*)prog.data(), prog.size() * sizeof(instr));

    outf.close();
}

//---------------------------------//
////////////////LEXER////////////////
//---------------------------------//
lexer lex_file(char *filename) {
    token_vector tokens;

    fstream file(filename, ios::in);

    if(file.fail())
        ISSUE_ERROR("File \"%s\" does not exist.", filename);

    stringstream strm;

    strm << file.rdbuf();

    regex reg(LEX_REG);
    string str = strm.str();
    sregex_token_iterator it(str.begin(), str.end(), reg);

    while(it != sregex_token_iterator()) {
        tokens.push_back(string(*it));
        it++;
    }
    
    tokens.push_back("eof");

    return lexer(tokens, string(filename));
}
//---------------------------------//
////////////////LEXER////////////////
//---------------------------------//