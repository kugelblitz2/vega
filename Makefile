CFLAGS=-O3 -mtune=native -Wall -Wextra -lpci

all: vega

clean: 
	rm -f vega

vega: vega.c linux/linux.c linux/linux.h
	cc vega.c -o vega $(CFLAGS)
	strip vega
