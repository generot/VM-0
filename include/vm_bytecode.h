#ifndef __VM_BYTECODE__
#define __VM_BYTECODE__

#include "vm_defs.h"

#ifndef __cplusplus
typedef struct _Bytecode_Header bcode_header;
typedef struct _Bytecode_File bcode_file;
#endif //__cplusplus

struct _Bytecode_Header {
    size_t program_size;
    size_t entry_point_addr;

    enum { STACK_SIZE_SPECIFIED, STACK_SIZE_DEFAULT } stack_size_spec;

    //Optional
    size_t code_seg_sz, data_seg_sz;
};

#ifdef __cplusplus
using bcode_header = _Bytecode_Header;
#endif

struct _Bytecode_File {
    instr *prog;
    bcode_header metadata;
};

#ifdef __cplusplus
using bcode_file = _Bytecode_File;    
#endif

#ifndef __cplusplus
void dump_bytecode(const char *filename, instr *program, size_t prog_sz, size_t entry_addr);
bcode_file read_bytecode(const char *filename);
#endif //__cplusplus

#endif //__VM_BYTECODE__