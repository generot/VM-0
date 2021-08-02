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

void display_instr(instr *prog, size_t prog_sz, size_t entry) {
    for(size_t i = 0; i < prog_sz; i++) {
        const char *instr_to_display = prog[i].op == OPC_SYSCALL
            ? syscalls[prog[i].scall]
            : opcodes[prog[i].op - 0x10];

        if(i == entry)
            puts("----ENTRY POINT----");

        printf("0x%.2X: [ %s", (unsigned)i, instr_to_display);

        switch(prog[i].content_type) {
        case INST_ADDR:
            printf(" <0x%.2X>", (unsigned)prog[i].addr);
            break;
        case INST_VAL:
            printf(" %.3lf", prog[i].val);
            break;
        case INST_NOARGS:
            break;
        }
        
        puts(" ]");
    }
}

int main(int argc, char **argv) {
    if(argc < 2)
        ISSUE_ERROR("Usage: ./dasm <bytecode_src>");

    bcode_file fl = read_bytecode(argv[1]);

    bool stack_size_provided = fl.metadata.stack_size_spec == STACK_SIZE_SPECIFIED;

    size_t code_seg_sz = !stack_size_provided ? CODE_SEG_SZ : fl.metadata.code_seg_sz;
    size_t data_seg_sz = !stack_size_provided ? DATA_SEG_SZ : fl.metadata.data_seg_sz;

    printf(
        "Program size: %I64u instructions\n"
        "Stack size%s: [Code segment: %I64u, Data segment: %I64u]\n"
        "Entry point address: 0x%.2X\n\n",
        fl.metadata.program_size,
        !stack_size_provided ? "(default)" : "",
        code_seg_sz, data_seg_sz,
        (unsigned)fl.metadata.entry_point_addr
    );

    display_instr(fl.prog, fl.metadata.program_size, 
        fl.metadata.entry_point_addr);

    return 0;
}