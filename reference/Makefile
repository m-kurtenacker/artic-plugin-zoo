SUBDIRS = cached-compute

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
