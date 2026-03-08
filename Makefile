PHONY: build run format all

build:
	gcc src/main.c src/priority_queue.c src/lock_map.c src/thread_pool.c -o main

run:
	./main

format:
	clang-format -i src/*.c src/*.h

all:
	make format && make build && make run
