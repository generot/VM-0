# VM/0
VM/0 is a simple procedural toy language, that is compiled to bytecode, which is then executed by the VM/0 virtual machine(similar to Java and Python).<br>
The VM/0 language is based on Niklaus Wirth's [PL/0 programming language](https://en.wikipedia.org/wiki/PL/0) with the sole purpose to immitate its simplicity for the sake of showcasing the virtual machine. That would pretty much make the virtual machine itself the primary interest of this project, hence why not much care was taken when I was writing the VM/0 compiler.

## Running a VM/0 program
Running a VM/0 program is a simple task. A few steps should be taken in the following order:<br>
- Compile the VM/0 source file to bytecode, using the VM/0 Compiler(VC), which on a Windows machine will be `vc.exe`.
- Pass the path to the resulting `.bc` file to the VM/0 Virtual Machine(VM), which, similar to the compiler, will be `vm.exe`.
- Profit!

## Disassembling VM/0 bytecode
For the sole purpose of debugging and readability, the VM/0 Disassembler(DASM) is provided(`dasm.exe`). If, for example, you have the bytecode file `exm.bc`, you can simply pass it as a command line argument to DASM and you will be given not only the disassembly of the bytecode, but also metadata about the program you've disassembled. In the context of a VM/0 program, the metadata is:
- The entry point's address
- The program's size(in instructions)
- A flag, specifying whether the stack size is predetermined or left to the default stack size, allocated by the virtual machine.
- Data segment size(if the stack size is predetermined)
- Code segment size(if the stack size is predetermined)

The a single instruction in the disassembly is in the following form: `[<INSTR> - <VAL_OR_ADDR_ENUM> - <SYSCALL> - (<VALUE> <ADDR>)]`.<br>
The `VAL_OR_ADDR` enum basically tells the virtual machine, when executing the program, whether it should use the `VALUE` or `ADDR` member of the instruction that is currently being performed. Since they both lie in a union(and 'cause I'm too lazy to check for that in DASM), the unused member is going to have some garbage value(obviously, since they share memory).

### Example
`input.vz` - basically a program that prints 15(stored in 'a':
```
var a;

proc main {
	a := 10 + 5;
	!a;
}
```
CMD: `./vc input.vz && ./dasm input.bc`<br>
Output:<br>
```
Program size: 7 instructions
Stack size: [Code segment: 0, Data segment: 0]
Entry point address: 0

[PUSH - INST_VAL - SC_INVALID - (10.000000 4621819117588971520)]
[PUSH - INST_VAL - SC_INVALID - (5.000000 4617315517961601024)]
[ADDF - INST_VAL - SC_WRITE - (0.000000 0)]
[STORE - INST_ADDR - SC_INVALID - (0.000000 1)]
[PUSH - INST_ADDR - SC_INVALID - (0.000000 1)]
[DEREF - INST_VAL - SC_INVALID - (0.000000 0)]
[SYSCALL - INST_VAL - SC_WRITELN - (0.000000 0)]
```

## Additional info
The compiler itself is written in C++(to save time), the virtual machine and the disassembler are written in C. No external libs were used here. The VM/0 grammar specification is represented(in BNF) in `grammar_spec.txt`. There are some more example VM/0 programs in `bin/misc`. 
