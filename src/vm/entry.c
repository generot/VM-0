#include "../../include/vm_defs.h"
#include "../../include/vm_bytecode.h"

double *memory = NULL;

double *code_seg = NULL;
double *data_seg = NULL;

size_s stack_sz = 0,
       stack_ptr = 0;

bool stack_push(double val) {
    if(stack_ptr >= stack_sz) {
        ISSUE_ERROR("Stack overflow. Maximum segment size reached(%I64d)", 
            stack_sz);
    }

    code_seg[stack_ptr++] = val;
    return true;
}

double stack_pop(void) {
    if(stack_ptr <= 0) {
        ISSUE_ERROR("Stack underflow. Out of bounds(stack pointer: %I64d).", stack_ptr);
    }

    double ret_val = code_seg[--stack_ptr];
    code_seg[stack_ptr] = 0;

    return ret_val;
}

int main(int argc, char **argv) {
    if(argc < 2) ISSUE_ERROR("Usage: ./vm <bytecode_file>");

    bcode_file bcode = read_bytecode(argv[1]);

    bool stack_size_specified = bcode.metadata.stack_size_spec == STACK_SIZE_SPECIFIED;

    size_t mem_sz = stack_size_specified
                    ? bcode.metadata.code_seg_sz + bcode.metadata.data_seg_sz
                    : DEFAULT_STACK_SIZE; 

    memory = calloc(mem_sz, sizeof(double));

    stack_sz = stack_size_specified
               ? bcode.metadata.code_seg_sz
               : CODE_SEG_SZ;

    data_seg = memory;
    code_seg = &memory[stack_size_specified ? bcode.metadata.data_seg_sz : DATA_SEG_SZ];

    size_s entry_point_addr = (size_s)bcode.metadata.entry_point_addr;
    size_s prog_sz = (size_s)bcode.metadata.program_size;
    
    for(size_s i = entry_point_addr; i < prog_sz; i++) {
        handle_instr(bcode.prog[i], data_seg, &i);
    }

    free(memory);
    free(bcode.prog);

    return 0;
}