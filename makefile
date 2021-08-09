assembler: assembler.o helpFuncs.o firstPass.o opcode.o instructions.o
	gcc -ansi -Wall -pedantic -g helpFuncs.o assembler.o firstPass.o opcode.o instructions.o -o assembler
helpFuncs.o: helpFuncs.c helpFuncs.h
	gcc -c -ansi -Wall -pedantic helpFuncs.c -o helpFuncs.o
firstPass.o: firstPass.c firstPass.h
	gcc -c -ansi -Wall -pedantic firstPass.c -o firstPass.o
opcode.o: opcode.c opcode.h
	gcc -c -ansi -Wall -pedantic opcode.c -o opcode.o
instructions.o: instructions.c instructions.h
	gcc -c -ansi -Wall -pedantic instructions.c -o instructions.o
assembler.o: assembler.c globals.h helpFuncs.h firstPass.h opcode.h instructions.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o
