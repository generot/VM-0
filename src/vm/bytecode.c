#include "../../include/vm_bytecode.h"

void dump_bytecode(const char *filename, instr *program, size_t prog_sz, size_t entry_addr) {
    FILE *f = fopen(filename, "wb");

    bcode_header prog_metadata = {
        .program_size = prog_sz,
        .entry_point_addr = entry_addr,
        .stack_size_spec = STACK_SIZE_DEFAULT
    };

    fwrite(&prog_metadata, sizeof(bcode_header), 1, f);
    fwrite(program, sizeof(instr), prog_metadata.program_size, f);

    fclose(f);
}

bcode_file read_bytecode(const char *filename) {
    FILE *f = fopen(filename, "rb");

    bcode_file new_file = {0};
    fread(&new_file.metadata, sizeof(bcode_header), 1, f);

    new_file.prog = calloc(new_file.metadata.program_size, sizeof(instr));
    fread(new_file.prog, sizeof(instr), new_file.metadata.program_size, f);

    fclose(f);

    return new_file;
}