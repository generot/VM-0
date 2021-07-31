#include "../include/vm_bytecode.h"

const char *opcodes[] = {
    "PUSH",
    "POP",
    "DEREF",
    "STORE",
    "ADDF",
    "SUBF",
    "MULTF",
    "DIVF",
    "AND",
    "OR",
    "GT",
    "LT",
    "GEQ",
    "LEQ",
    "EQ",
    "NEQ",
    "JMP",
    "JNZ",
    "JEZ",
    "CALL",
    "RET",
    "SYSCALL",
    "INVALID"
}, *syscalls[] = {
    "SC_WRITE",
    "SC_WRITELN",
    "SC_READ",
    "SC_INVALID"
}, *cont_types[] = {
    "INST_VAL", 
    "INST_ADDR"
};

void display_instr(instr *prog, size_t prog_sz) {
    for(size_t i = 0; i < prog_sz; i++) {
        printf("[%s - %s - %s - (%lf %I64u)]\n",
              opcodes[prog[i].op - 0x10], cont_types[prog[i].content_type],
              syscalls[prog[i].scall], prog[i].val, prog[i].addr);
    }
}

int main(int argc, char **argv) {
    if(argc < 2)
        ISSUE_ERROR("Usage: ./dasm <bytecode_src>");

    bcode_file fl = read_bytecode(argv[1]);

    printf(
        "Program size: %I64u instructions\n"
        "Stack size: [Code segment: %I64u, Data segment: %I64u]\n"
        "Entry point address: %I64u\n\n",
        fl.metadata.program_size,
        fl.metadata.code_seg_sz, fl.metadata.data_seg_sz,
        fl.metadata.entry_point_addr
    );

    display_instr(fl.prog, fl.metadata.program_size);

    return 0;
}