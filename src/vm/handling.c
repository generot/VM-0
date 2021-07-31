#include "../../include/vm_defs.h"

//Operations, for which the order of operands is important, are reversed.
void handle_instr(instr in, double *data_segment, size_s *ip) {
    double lhs = 0, rhs = 0;
    size_s addr;

    if(in.op >= OPC_ADDF && in.op <= OPC_NEQ) {
        lhs = stack_pop();
        rhs = stack_pop();
    }

    switch(in.op) {
    case OPC_PUSH:
        if(in.content_type == INST_ADDR)
            stack_push(in.addr);
        else
            stack_push(in.val);
        
        break;
    case OPC_POP:
        stack_pop();
        break;
    case OPC_DEREF:
        addr = (size_s)stack_pop();
        stack_push(data_segment[addr]);
        break;
    case OPC_STORE:
        data_segment[in.addr] = stack_pop();
        break;
    case OPC_ADDF:
        stack_push(lhs + rhs);
        break;
    case OPC_SUBF:
        stack_push(rhs - lhs);
        break;
    case OPC_MULTF:
        stack_push(lhs * rhs);
        break;
    case OPC_DIVF:
        stack_push(rhs / lhs);
        break;
    case OPC_AND:
        stack_push(lhs && rhs);
        break;
    case OPC_OR:
        stack_push(lhs || rhs);
        break;
    case OPC_GT:
        stack_push(lhs < rhs);
        break;
    case OPC_LT:
        stack_push(lhs > rhs);
        break;
    case OPC_GEQ:
        stack_push(lhs <= rhs);
        break;
    case OPC_LEQ:
        stack_push(lhs >= rhs);
        break;
    case OPC_EQ:
        stack_push(lhs == rhs);
        break;
    case OPC_NEQ:
        stack_push(lhs != rhs);
        break;
    case OPC_JMP:
        *ip = in.addr - 1;
        break;
    case OPC_JNZ:
        lhs = stack_pop();
        if(lhs != 0) *ip = in.addr - 1;

        break;
    case OPC_JEZ:
        lhs = stack_pop();
        if(lhs == 0) *ip = in.addr - 1;

        break;
    case OPC_CALL:
        if(in.content_type == INST_ADDR) {
            double ret_addr = (double)(*ip) + 1;
            // printf("Return address(from OPC_CALL): %lf\n", ret_addr);

            stack_push(ret_addr);
            *ip = in.addr - 1;
        }

        break;
    case OPC_RET:
        lhs = stack_pop();
        *ip = (size_s)lhs - 1;

        break;
    case OPC_SYSCALL:
        handle_syscall(in, data_segment);
        break;
    }
}

void handle_syscall(instr in, double *data_segment) {
    switch(in.scall) {
    case SC_READ:
        scanf("%lf", &data_segment[in.addr]);
        break;
    case SC_WRITE:
        printf("%lf", stack_pop());
        break;
    case SC_WRITELN:
        printf("%lf\n", stack_pop());
        break;
    }
}