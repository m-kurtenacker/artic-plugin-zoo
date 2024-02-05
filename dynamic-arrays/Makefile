.PHONY: all clean clean-all
all: main

clean:
	rm -rf main main.ll main.thorin main.thorin.json

clean-all: clean
	@make -C plugin/build clean

plugin/build/dynamic_arrays.so: plugin/dynamic_arrays.cpp
	@make -C plugin/build all

RUNTIME=${THORIN_RUNTIME_PATH}/artic/runtime.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics_thorin.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics_rv.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics_math.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics.impala

main.ll: main.art plugin/build/dynamic_arrays.so
	artic \
		-O2 \
		--emit-llvm \
		--plugin plugin/build/dynamic_arrays.so \
		-o main \
		--log-level info \
		${RUNTIME} \
		main.art

main: main.ll util.cpp
	clang++ -o main -O3 -g -L${THORIN_RUNTIME_PATH}/../build/lib -lruntime -lm $^
