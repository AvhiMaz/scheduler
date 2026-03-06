PHONY: build run all

build:
	gcc main.c -o main

run:
	./main

all:
	make build && make run
