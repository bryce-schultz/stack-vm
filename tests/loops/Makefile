LIC=../../src/lic/main
VM=../../src/vm/main

all: clean

build: ../../src/**/*.hpp ../../src/**/*.cpp ../../src/**/*.h ../../src/**/*.c
	../../build -j 32

compile: for.li while.li
	@$(LIC) for.li > /dev/null
	@$(LIC) while.li > /dev/null

run: compile
	@$(VM) for.lsm > for.output
	@$(VM) while.lsm > while.output

test: run
	@diff for.output for.correct || (echo "\033[0;31mFor test failed!\033[0m" && exit 1)
	@diff while.output while.correct || (echo "\033[0;31mWhile test failed!\033[0m" && exit 1)

clean: test
	@rm -f for.lsm for.output
	@rm -f while.lsm while.output