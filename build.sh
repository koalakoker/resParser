#!/bin/bash
bison -d -v calc.y
flex -ocalc.lex.c calc.l
