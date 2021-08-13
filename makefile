assembler: assembler.o helpFuncs.o firstPass.o opcode.o guidances.o dataCodeImages.o instructions.o symbolTable.o
	gcc -ansi -Wall -pedantic -g helpFuncs.o assembler.o firstPass.o opcode.o guidances.o dataCodeImages.o instructions.o symbolTable.o -o assembler
helpFuncs.o: helpFuncs.c helpFuncs.h
	gcc -c -ansi -Wall -pedantic helpFuncs.c -o helpFuncs.o
firstPass.o: firstPass.c firstPass.h
	gcc -c -ansi -Wall -pedantic firstPass.c -o firstPass.o
opcode.o: opcode.c opcode.h
	gcc -c -ansi -Wall -pedantic opcode.c -o opcode.o
guidances.o: guidances.c guidances.h
	gcc -c -ansi -Wall -pedantic guidances.c -o guidances.o
dataCodeImages.o: dataCodeImages.c dataCodeImages.h
	gcc -c -ansi -Wall -pedantic dataCodeImages.c -o dataCodeImages.o
instructions.o: instructions.c instructions.h
	gcc -c -ansi -Wall -pedantic instructions.c -o instructions.o
symbolTable.o: symbolTable.c symbolTable.h
	gcc -c -ansi -Wall -pedantic symbolTable.c -o symbolTable.o
assembler.o: assembler.c globals.h helpFuncs.h firstPass.h opcode.h guidances.h dataCodeImages.h instructions.h symbolTable.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o

