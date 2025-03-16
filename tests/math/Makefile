LIC=../../src/lic/main
VM=../../src/vm/main

all: clean

compile: math.li
	@$(LIC) math.li > /dev/null

run: compile
	@$(VM) math.lsm > math.output

test: run
	@diff math.output math.correct || (echo "\033[0;31mMath test failed!\033[0m" && exit 1)

clean: test
	@rm -f math.lsm math.output