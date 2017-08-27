#!/bin/bash
bison -d calc.y
flex -ocalc.lex.c calc.l
