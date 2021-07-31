CC = gcc
CPP = g++
CFLAGS = -Wall -g

#######DIRECTORIES#######
BIN = bin/
INCDIR = include/
SRCLANG = src/lang/
SRCVM = src/vm/
#######DIRECTORIES#######

INCVM = $(INCDIR)vm_bytecode.h $(INCDIR)vm_defs.h $(INCDIR)common.h
INCLANG = $(INCDIR)lang_vm0.h $(INCDIR)common.h

OBJLANG = $(patsubst $(SRCLANG)%.cc, $(BIN)%.o, $(wildcard $(SRCLANG)*.cc)) 
OBJVM = $(patsubst $(SRCVM)%.c, $(BIN)%.o, $(wildcard $(SRCVM)*.c))
OBJDASM = $(BIN)disasm.o $(BIN)bytecode.o

all: vc vm dasm

vc: $(OBJLANG)
	$(CPP) $(CFLAGS) -o $(BIN)vc.exe $^

vm: $(OBJVM)
	$(CC) $(CFLAGS) -o $(BIN)vm.exe $^

dasm: $(OBJDASM)
	$(CC) $(CFLAGS) -o $(BIN)dasm.exe $^

$(BIN)%.o: $(SRCVM)%.c $(INCVM)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)%.o: $(SRCLANG)%.cc $(INCLANG)
	$(CPP) $(CFLAGS) -c $< -o $@

$(BIN)disasm.o: src/disasm.c $(INCVM)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean delobj delexe

clean:
	make delexe
	make delobj

delexe:
	rm $(BIN)*.exe

delobj:
	rm $(BIN)*.o