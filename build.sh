#!/bin/bash
bison -d -v calc.y
flex -ocalc.lex.c calc.l
qmake
make
./resParser.app/Contents/MacOS/resParser