.PHONY: all clean clean-all
all: memory

clean:
	rm -f *.ll *.thorin.json memory

clean-all:
	@make -C plugin/build clean

plugin/build:
	cd plugin; mkdir build; cd build; cmake ..

plugin/build/memory.so: plugin/build plugin/memory.cpp
	@make -C plugin/build all

RUNTIME=${THORIN_RUNTIME_PATH}/artic/runtime.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics_thorin.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics_rv.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics_math.impala \
	${THORIN_RUNTIME_PATH}/artic/intrinsics.impala

main.thorin.json: main.art
	artic \
		${RUNTIME} \
		$^ \
		--emit-json \
		--log-level info \
		-o main

main-memory.thorin.json: main.thorin.json plugin/build/memory.so
	anyopt \
		main.thorin.json \
		--pass cleanup_world \
		--pass pe \
		--pass plugin_execute \
		--pass cleanup_world \
		--plugin plugin/build/memory.so \
		--emit-json \
		--log-level info \
		-o main-memory

main-memory.ll: main.art plugin/build/memory.so
	artic \
		${RUNTIME} \
		main.art \
		--plugin plugin/build/memory.so \
		--emit-llvm \
		--log-level info \
		-o main-memory

memory: main-memory.ll
	#clang++ -o memory -O3 -L${THORIN_RUNTIME_PATH}/../build/lib -lruntime -lm $^
	clang++ -o memory -Og -g -L${THORIN_RUNTIME_PATH}/../build/lib -lruntime -lm $^
