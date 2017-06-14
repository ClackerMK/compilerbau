CC = g++
CCFLAGS = -g -W -Wall -std=c++11

FLEX = flex
FLEX_OPTS = -PCPP

BISON = bison
BISON_OPTS = -t -pCPP

OBJS = Absyn.o Lexer.o Parser.o Printer.o SymbolTableBuilder.o TypeChecker.o

all: TestCPP

clean:
	rm -f *.o TestCPP CPP.aux CPP.log CPP.pdf CPP.dvi CPP.ps CPP

TestCPP: ${OBJS} Test.o
	@echo "Linking TestCPP..."
	${CC} ${CCFLAGS} ${OBJS} Test.o -o tccpp

Absyn.o: Absyn.C Absyn.H
	${CC} ${CCFLAGS} -c Absyn.C

Lexer.o: Lexer.C Parser.H
	${CC} ${CCFLAGS} -c Lexer.C

Parser.o: Parser.C Absyn.H
	${CC} ${CCFLAGS} -c Parser.C

Printer.o: Printer.C Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Printer.C

Skeleton.o: Skeleton.C Skeleton.H Absyn.H
	${CC} ${CCFLAGS} -c Skeleton.C

Test.o: Test.C Parser.H Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Test.C

SymbolTableBuilder.o: SymbolTableBuilder.C SymbolTableBuilder.H
	${CC} ${CCFLAGS} -c SymbolTableBuilder.C

TypeChecker.o: TypeChecker.C TypeChecker.H
	${CC} ${CCFLAGS} -c TypeChecker.C
