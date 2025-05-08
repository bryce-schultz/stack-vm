LIC=../../src/lic/lic
VM=../../src/vm/vm

all: clean

compile: basic_function.li arg_function.li return_arg_function.li recursive_function.li
	@$(LIC) basic_function.li > /dev/null
	@$(LIC) arg_function.li > /dev/null
	@$(LIC) return_arg_function.li > /dev/null
	@$(LIC) recursive_function.li > /dev/null
	

run: compile
	@$(VM) basic_function.lsm > basic_function.output
	@$(VM) arg_function.lsm > arg_function.output
	@$(VM) return_arg_function.lsm > return_arg_function.output
	@$(VM) recursive_function.lsm > recursive_function.output

test: run
	@diff basic_function.output basic_function.correct || (echo "\033[0;31mBasic Function test failed!\033[0m" && exit 1)
	@diff arg_function.output arg_function.correct || (echo "\033[0;31mArgument Function test failed!\033[0m" && exit 1)
	@diff return_arg_function.output return_arg_function.correct || (echo "\033[0;31mReturn Argument Function test failed!\033[0m" && exit 1)
	@diff recursive_function.output recursive_function.correct || (echo "\033[0;31mRecursive Function test failed!\033[0m" && exit 1)

clean: test
	@rm -f basic_function.lsm basic_function.output
	@rm -f arg_function.lsm arg_function.output
	@rm -f return_arg_function.lsm return_arg_function.output
	@rm -f recursive_function.lsm recursive_function.output