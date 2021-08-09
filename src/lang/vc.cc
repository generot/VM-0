#include "../../include/lang_vm0.h"

//Not really trying to be convenient.
using namespace std;
using bcode_vector = vector<instr>;

map<string, symbol> symbol_table;
bcode_vector bytecode;

size_s var_num = 1;

//----------------------------------//
////////////////PARSER////////////////
//----------------------------------//

#define EXPECT(tk) lx.expect(tk, false)

void program(lexer& lx) {
    while(!lx.eof()) {
        if(!(EXPECT("var") || EXPECT("proc") || EXPECT("include")))
            ISSUE_ERROR("Expected top-level declaration, got \"%s\" instead.",
                lx.accept().c_str());

        decl(lx);
        include(lx);
        procedure(lx);
    }
}

void include(lexer& lx) {
    string path = lx.accept();

    if(path != "include") {
        lx.unaccept();
        return;
    }
    
    path = lx.accept();

    string filepath = lx.file_path() + path.substr(1, path.length() - 2);
    lexer _module = lex_file((char*)filepath.c_str());

    program(_module);
}

void decl(lexer& lx) {
    string ident = lx.accept();

    if(ident != "var") {
        lx.unaccept();
        return;
    }

    while(ident != ";") {
        ident = lx.accept();
        symbol_table[ident] = { SYM_VARIABLE, ident, var_num };

        var_num++;
        if(EXPECT(",") || EXPECT(";"))
            ident = lx.accept();
        else
            ISSUE_ERROR("Expected separator or end of statement, got \"%s\" instead.", 
                lx.accept().c_str());
    }
}

void procedure(lexer& lx) {
    string ident = lx.accept();
    
    if(ident != "proc") {
        lx.unaccept();
        return;
    }

    size_s starting_addr = bytecode.size();

    ident = lx.accept();

    if(symbol_table[ident].stype == SYM_FUNCTION)
        ISSUE_ERROR("Error: Redefinition of function \"%s\".", ident.c_str());

    symbol_table[ident] = { SYM_FUNCTION, ident, starting_addr };

    block(lx);

    if(ident != ENTRY_POINT) 
        bytecode.push_back({ INST_NOARGS, OPC_RET });
}

void block(lexer& lx) {
    lx.expect("{");
    lx.accept();

    while(!EXPECT("}")) {
        statement(lx);

        if(lx.eof())
            ISSUE_ERROR("Expected '}', got 'eof' instead");
    }

    lx.accept();
}

void statement(lexer& lx) {
    assign(lx);
    cond_if(lx);
    cond_while(lx);
    call(lx);
    inpout(lx);

    lx.expect(";");
    lx.accept();
}

void assign(lexer& lx) {
    string ident = lx.accept();

    if(!EXPECT(":=")) {
        lx.unaccept();
        return;
    }

    symbol sym = symbol_table[ident];

    if(!sym.addr || sym.stype != SYM_VARIABLE)
        ISSUE_ERROR("Symbol \"%s\" is unreachable or unassignable.", ident.c_str());

    lx.accept();
    expr(lx);

    bytecode.push_back({ INST_ADDR, OPC_STORE, SC_INVALID, sym.addr });
}

void cond_if(lexer& lx) {
    if(!(EXPECT("if")))
        return;

    lx.accept();
    expr(lx);

    bytecode.push_back({ INST_ADDR, OPC_JEZ, SC_INVALID, 0x0 });
    size_t curr_sz = bytecode.size() - 1;

    block(lx);
    bytecode[curr_sz].addr = (size_s)bytecode.size();
}

void cond_while(lexer& lx) {
    if(!EXPECT("while"))
        return;

    lx.accept();
    size_t curr_addr = bytecode.size();

    expr(lx);
    block(lx);

    bytecode.push_back({ INST_ADDR, OPC_JNZ, SC_INVALID, (size_s)curr_addr });
}

void call(lexer& lx) {
    if(!EXPECT("call"))
        return;
    
    lx.accept();
    string ident = lx.accept();
    symbol func = symbol_table[ident];

    if(func.ident == string())
        ISSUE_ERROR("Undefined reference to function \"%s\".", ident.c_str());

    bytecode.push_back({ INST_ADDR, OPC_CALL, SC_INVALID, func.addr });
}

void inpout(lexer& lx) {
    if(!(EXPECT("?") || EXPECT("!")))
        return;

    instr in = instr();

    string op = lx.accept();
    string ident = lx.accept();

    symbol sym = symbol_table[ident];
    if(!sym.addr)
        ISSUE_ERROR("Undefined reference to variable \"%s\".", ident.c_str());

    if(op == "?") {
        in = { INST_ADDR, OPC_SYSCALL, SC_READ };
        in.addr = sym.addr;

        bytecode.push_back(in);
    }
    else if(op == "!") {
        in = { INST_ADDR, OPC_PUSH, SC_INVALID };
        in.addr = sym.addr;

        bytecode.push_back(in);
        bytecode.push_back({ INST_NOARGS, OPC_DEREF, SC_INVALID });
        bytecode.push_back({ INST_NOARGS, OPC_SYSCALL, SC_WRITELN });
    }
}

void expr(lexer& lx) {
    comp(lx);
    while(EXPECT("&&") || EXPECT("||")) {
        opcode opc = lx.accept() == "&&" ? OPC_AND : OPC_OR;
        comp(lx);

        bytecode.push_back({ INST_NOARGS, opc });
    }
}

void comp(lexer& lx) {
    term(lx);
    while(EXPECT(">")  || EXPECT("<")  || 
          EXPECT(">=") || EXPECT("<=") || 
          EXPECT("==") || EXPECT("!=")) 
    {
        opcode opc = match_comp(lx.accept());
        term(lx);

        bytecode.push_back({ INST_NOARGS, opc });
    }
}
void term(lexer& lx) {
    factor(lx);
    while(EXPECT("+") || EXPECT("-")) {
        opcode opc = lx.accept() == "+" ? OPC_ADDF : OPC_SUBF;
        factor(lx);

        bytecode.push_back({ INST_NOARGS, opc });
    }
}

void factor(lexer& lx) {
    value(lx);
    while(EXPECT("*") || EXPECT("/")) {
        opcode opc = lx.accept() == "*" ? OPC_MULTF : OPC_DIVF;
        value(lx);

        bytecode.push_back({ INST_NOARGS, opc });
    }
}

void value(lexer& lx) {
    string val = lx.accept();

    if(val == "(") {
        expr(lx);

        lx.expect(")");
        lx.accept();

        return;
    }

    if(is_numeric(val)) {
        double numval = atof(val.c_str());

        instr in = { INST_VAL, OPC_PUSH, SC_INVALID };
        in.val = numval;

        bytecode.push_back(in);
    } else {
        symbol var = symbol_table[val];
        bytecode.push_back({ INST_ADDR, OPC_PUSH, SC_INVALID, (size_s)var.addr });
        bytecode.push_back({ INST_NOARGS, OPC_DEREF, SC_INVALID });
    }
}

#undef EXPECT

int main(int argc, char **argv) {
    if(argc < 2)
        ISSUE_ERROR("Usage: ./vc <vm0_src_file>");

    lexer lx = lex_file(argv[1]);
    program(lx);

    symbol entry = symbol_table["main"];

    if(entry.ident == string() || entry.stype != SYM_FUNCTION)
        ISSUE_ERROR("No entry point provided.");

    string filepath = string(argv[1]);
    size_t off = filepath.find_first_of('.', 0);

    string output_filename = filepath.substr(0, off) + ".bc";

    cout << "Program size: " << bytecode.size() << " instructions" << endl;
    dump_program(output_filename, bytecode, entry.addr);

    return 0;
}
