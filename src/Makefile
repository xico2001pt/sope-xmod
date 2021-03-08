all: xmod

xmod: change.c change.h regfile.c regfile.h xmod_info.c xmod_info.h xmod.c
	cc xmod.c regfile.c xmod_info.c -Wall -o xmod

clean:
	rm -f xmod