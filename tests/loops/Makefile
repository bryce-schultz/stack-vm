LIC=../../src/lic/lic
VM=../../src/vm/vm

all: run

compile: main.li
	../../build
	$(LIC) main.li

run: compile
	$(VM) main.lsm

clean:
	rm -f main.lsm