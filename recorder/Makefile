.PHONY: all clean clean-all
all: main

clean:
	rm -rf main main.ll main.thorin main.thorin.json

clean-all: clean
	@make -C plugin/build clean

plugin/build/recorder.so: plugin/recorder.cpp
	@make -C plugin/build all

RUNTIME=${THORIN_RUNTIME_PATH}/artic/runtime.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics_thorin.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics_rv.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics_math.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics.impala


main.ll: main.art plugin/build/recorder.so
	artic \
		-O2 \
		--emit-llvm \
		--plugin plugin/build/recorder.so \
		-o main \
		--log-level info \
		${RUNTIME} \
		main.art

main: main.ll
	clang++ -o main -O3 -g -L${THORIN_RUNTIME_PATH}/../build/lib -lruntime -lm $^
