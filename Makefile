PHONY: build run all

build:
	gcc main.c priority_queue.c -o main

run:
	./main

all:
	make build && make run
