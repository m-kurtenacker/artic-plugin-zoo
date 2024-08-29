SUBDIRS = cached-compute cuda_reductions dynamic-arrays extern_language ir-extension plugin-memory recorder reflection test-plugin shared_offset merge_sort select_on_user

.PHONY: all $(SUBDIRS) clean
all: $(SUBDIRS)

clean:
	rm -rf */build
	rm -f complexity.txt

$(SUBDIRS):
	mkdir -p $@/build
	cd $@/build && cmake .. -DCMAKE_BUILD_TYPE=DEBUG
	make -C $@/build

.PHONY: test %.test
%.test: % %/run.args %/run.res
	$</build/main `cat $</run.args` | diff -q - $</run.res

test: $(SUBDIRS:=.test)

.PHONY: complexity %.compexity
%.complexity: %/main.art
	artic --log-level info --halstead $<
	artic --log-level info --cyclomatic $<
	cat $< | sed '/^\s*$$/d' | wc -l
	echo "$* `artic --halstead $<` `artic --cyclomatic $<` `cat $< | sed '/^\s*$$/d' | wc -l`" >> complexity.txt

complexity: complexity.txt $(SUBDIRS:=.complexity)

complexity.txt:
	echo "file halstead cyclomatic loc" >> complexity.txt

.PHONY: reference
reference:
	@make -C reference

.PHONY: reference-%
reference-%:
	@make -C reference $*
