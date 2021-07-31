#ifndef __PROGRAMS_H__
#define __PROGRAMS_H__

#include "vm_defs.h"

#ifdef PROG1
    instr prog[] = {
        { .op = OPC_PUSH, .val = 10 },
        { .op = OPC_STORE, .addr = 0x09 },
        { .op = OPC_PUSH, .addr = 0x09, .content_type = INST_ADDR },
        { .op = OPC_DEREF },
        { .op = OPC_PUSH, .addr = 0x00, .content_type = INST_ADDR },
        { .op = OPC_DEREF },
        { .op = OPC_PUSH, .val = 2 },
        { .op = OPC_ADDF },
        { .op = OPC_STORE, .addr = 0x00 },
        { .op = OPC_PUSH, .val = -1 },
        { .op = OPC_ADDF },
        { .op = OPC_STORE, .addr = 0x09 },
        { .op = OPC_PUSH, .addr = 0x09, .content_type = INST_ADDR },
        { .op = OPC_DEREF },
        { .op = OPC_JNZ, .addr =  0x02 },
        { .op = OPC_PUSH, .content_type = INST_ADDR, .addr = 0x00 },
        { .op = OPC_DEREF },
        { .op = OPC_SYSCALL, .scall = SC_WRITE }
    };
#endif

#ifdef PROG2
    instr prog[] = {
        { .op = OPC_PUSH, .content_type = INST_ADDR, .addr = 0x04 },
        { .op = OPC_DEREF },
        { .op = OPC_PUSH, .val = 10 },
        { .op = OPC_DIVF },
        { .op = OPC_STORE, .addr = 0x12 },
        { .op = OPC_RET },
        //Entry point - Addr: 6
        { .op = OPC_SYSCALL, .scall = SC_READ, .addr = 0x04},
        { .op = OPC_CALL, .content_type = INST_ADDR, .addr = 0x00 },
        { .op = OPC_PUSH, .content_type = INST_ADDR, .addr = 0x12 },
        { .op = OPC_DEREF },
        { .op = OPC_SYSCALL, .scall = SC_WRITE },
    };
#endif

//This goes in an endless loop.
#ifdef PROG3
    instr prog[] = {
        { .op = OPC_PUSH, .val = 10.56 },
        { .op = OPC_SYSCALL, .scall = SC_WRITELN },
        { .op = OPC_PUSH, .val = 22.56 },
        { .op = OPC_SYSCALL, .scall = SC_WRITELN },
        { .op = OPC_JMP, .addr = 0x00 }
    };
#endif

#endif //__PROGRAMS_H__