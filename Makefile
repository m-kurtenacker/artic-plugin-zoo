.PHONY: all
all: main

.PHONY: test_plugin/build
test_plugin/build:
	@make -C test_plugin/build

main.thorin: main.art test_plugin/build
	artic -O2 --emit-thorin --plugin test_plugin/build/test.so main.art

main.ll: main.art test_plugin/build
	artic -O2 --emit-llvm --plugin test_plugin/build/test.so main.art

main: main.ll backend.c
	clang -O3 backend.c main.ll -o main
