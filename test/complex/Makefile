LIC=../../src/lic/lic
VM=../../src/vm/vm

all: clean

compile: complex.li
	@$(LIC) complex.li > /dev/null

run: compile
	@$(VM) complex.lsm > complex.output

test: run
	@diff complex.output complex.correct || (echo "\033[0;31mComplex test failed!\033[0m" && exit 1)

clean: test
	@rm -f complex.lsm complex.output