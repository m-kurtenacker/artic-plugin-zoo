SUBDIRS = cached-compute cuda_reductions dynamic-arrays extern_language ir-extension plugin-memory recorder reflection test-plugin vector-ops

.PHONY: all $(SUBDIRS) clean
all: $(SUBDIRS)

clean:
	rm -rf */build

$(SUBDIRS):
	mkdir -p $@/build
	cd $@/build && cmake .. -DCMAKE_BUILD_TYPE=DEBUG
	make -C $@/build

.PHONY: test %.test
%.test: % %/run.args %/run.res
	$</build/main `cat $</run.args` | diff -q - $</run.res

test: $(SUBDIRS:=.test)
