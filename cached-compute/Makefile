.PHONY: all clean clean-all
all: main

clean:
	rm -f *.ll *.thorin.json main

clean-all: clean
	@make -C plugin/build clean

plugin/build:
	cd plugin; mkdir build; cd build; cmake ..

plugin/build/cache.so: plugin/build plugin/cache.cpp
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

main-cached.thorin.json: main.thorin.json plugin/build/cache.so
	anyopt \
		main.thorin.json \
		--pass cleanup_world \
		--pass pe \
		--pass plugin_execute \
		--pass cleanup_world \
		--plugin plugin/build/cache.so \
		--emit-json \
		--log-level info \
		-o main-cached

main-cached.ll: main.art plugin/build/cache.so
	artic \
		${RUNTIME} \
		main.art \
		--plugin plugin/build/cache.so \
		--emit-llvm \
		--log-level info \
		-o main-cached

main: main-cached.ll cache-util.cpp
	#clang++ -o main -O3 -L${THORIN_RUNTIME_PATH}/../build/lib -lruntime -lm $^
	clang++ -o main -Og -g -L${THORIN_RUNTIME_PATH}/../build/lib -lruntime -lm $^
