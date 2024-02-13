.PHONY: all clean clean-all
all: main

clean:
	rm -rf main main.ll main.thorin main.thorin.json

clean-all: clean
	@make -C test_plugin/build clean

test_plugin/build:
	cd test_plugin; mkdir build; cd build; cmake ..

test_plugin/build/test.so: test_plugin/build test_plugin/test.cpp
	@make -C test_plugin/build

main.thorin: main.art test_plugin/build/test.so
	artic -O2 --emit-thorin --plugin test_plugin/build/test.so main.art

main.thorin.json: main.art
	artic --emit-json main.art

main.old.ll: main.art test_plugin/build/test.so
	artic -O2 --emit-llvm --plugin test_plugin/build/test.so main.art

main.ll: main.thorin.json test_plugin/build/test.so
	anyopt --emit-llvm --plugin test_plugin/build/test.so main.thorin.json

main: main.ll backend.c
	clang -O3 backend.c main.ll -o main
