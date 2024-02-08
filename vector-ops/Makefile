.PHONY: all clean clean-all
all: main

clean:
	rm -rf main main.ll main.thorin main.thorin.json

clean-all: clean
	@make -C plugin/build clean

plugin/build:
	cd plugin; mkdir build; cd build; cmake ..

plugin/build/extension.so: plugin/build plugin/extension.cpp
	@make -C plugin/build all

RUNTIME=${THORIN_RUNTIME_PATH}/artic/runtime.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics_thorin.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics_rv.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics_math.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics.impala


main.ll: main.art plugin/build/extension.so
	artic \
		-O2 \
		--emit-llvm \
		--plugin plugin/build/extension.so \
		-o main \
		--log-level info \
		${RUNTIME} \
		main.art

main: main.ll
	clang++ -o main -O3 -g -L${THORIN_RUNTIME_PATH}/../build/lib -lruntime -lm $^
