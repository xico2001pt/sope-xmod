all: xmod

xmod: src/signal.h src/signal.c src/change.c src/change.h src/regfile.c src/regfile.h src/xmod_info.c src/xmod_info.h src/xmod.c src/xmod.h 
	cc src/xmod.c src/regfile.c src/xmod_info.c src/change.c src/signal.c -Wall -o xmod

cpplint:
	cpplint --filter=-whitespace,-legal/copyright,-readability/check,-readability/casting,-build/header_guard,-build/include --recursive src

valgrind:
	valgrind -q --gen-suppressions=yes $(CMD)

infer:
	infer run -- make

clean:
	rm -f xmod