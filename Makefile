CC = gcc
NAME = daemon

build:
	${CC} -Wall ${NAME}.c -o ${NAME}