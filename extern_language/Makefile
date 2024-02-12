.PHONY: all clean clean-all
all: main

clean:
	rm -rf main main.ll main.thorin main.thorin.json test.c

clean-all: clean
	@make -C plugin/build clean

plugin/build:
	cd plugin; mkdir build; cd build; cmake ..

plugin/build/extern_language.so: plugin/build plugin/extern_language.cpp
	@make -C plugin/build all

RUNTIME=${THORIN_RUNTIME_PATH}/artic/runtime.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics_thorin.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics_rv.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics_math.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics.impala


main.ll: main.art plugin/build/extern_language.so
	artic \
		-O2 \
		--emit-llvm \
		--plugin plugin/build/extern_language.so \
		-o main \
		--log-level info \
		${RUNTIME} \
		main.art

main: main.ll
	clang -o main -O3 -g -L${THORIN_RUNTIME_PATH}/../build/lib -lruntime -lm $^ test.c
