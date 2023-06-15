.PHONY: all
all: main

.PHONY: clean
clean:
	rm -rf main main.ll main.thorin main.thorin.json

.PHONY: test_plugin/build
test_plugin/build:
	@make -C test_plugin/build

main.thorin: main.art test_plugin/build
	artic -O2 --emit-thorin --plugin test_plugin/build/test.so main.art

main.thorin.json: main.art
	artic --emit-json main.art

main.old.ll: main.art test_plugin/build
	artic -O2 --emit-llvm --plugin test_plugin/build/test.so main.art

main.ll: main.thorin.json test_plugin/build
	anyopt --emit-llvm --plugin test_plugin/build/test.so main.thorin.json

main: main.ll backend.c
	clang -O3 backend.c main.ll -o main
