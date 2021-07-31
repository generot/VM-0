#ifndef __VM_DEFS__
#define __VM_DEFS__

#include "common.h"
#include <stdint.h>

#define DEFAULT_STACK_SIZE 1024 * 2

#define CODE_SEG_SZ 512
#define DATA_SEG_SZ (DEFAULT_STACK_SIZE - CODE_SEG_SZ)

#ifndef __cplusplus
typedef enum _Opcode opcode;
typedef enum _Syscall syscall;
typedef enum _Ctype ctype;
typedef struct _Instruction instr;
#endif //__cplusplus

typedef long long int size_s;

#ifndef __cplusplus
typedef enum { false, true } bool;
#endif

enum _Opcode {
    OPC_PUSH = 0x10,
    OPC_POP,
    OPC_DEREF,
    OPC_STORE,
    OPC_ADDF,
    OPC_SUBF,
    OPC_MULTF,
    OPC_DIVF,
    OPC_AND,
    OPC_OR,
    OPC_GT,
    OPC_LT,
    OPC_GEQ,
    OPC_LEQ,
    OPC_EQ,
    OPC_NEQ,
    OPC_JMP,
    OPC_JNZ,
    OPC_JEZ,
    OPC_CALL,
    OPC_RET,
    OPC_SYSCALL,
    OPC_INVALID
};

enum _Syscall {
    SC_WRITE,
    SC_WRITELN,
    SC_READ,
    SC_INVALID
};

enum _Ctype {
    INST_VAL, 
    INST_ADDR
};

#ifdef __cplusplus
using opcode = _Opcode;
using syscall = _Syscall;
using ctype = _Ctype;
#endif //__cplusplus

struct _Instruction {
    ctype content_type;

    opcode op;
    syscall scall;

    union {
        size_s addr;
        double val;
    };
};


#ifdef __cplusplus
using instr = _Instruction;
#endif //__cplusplus

#ifndef __cplusplus
bool stack_push(double val);
double stack_pop(void);

void handle_instr(instr in, double *data_segment, size_s *ip);
void handle_syscall(instr in, double *data_segment);
#endif

#endif //__VM_DEFS__