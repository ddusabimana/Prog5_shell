#
# makefile for mysh
#
CC=gcc
CFLAGS=-g

mysh: smsh1.c splitline.c builtins.c execute.c
	gcc  smsh1.c splitline.c builtins.c execute.c -o mysh