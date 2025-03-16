LIC=../../src/lic/lic
VM=../../src/vm/vm

all: clean

compile: if.li
	@$(LIC) if.li > /dev/null

run: compile
	@$(VM) if.lsm > if.output

test: run
	@diff if.output if.correct || (echo "\033[0;31mIf test failed!\033[0m" && exit 1)

clean: test
	@rm -f if.lsm if.output