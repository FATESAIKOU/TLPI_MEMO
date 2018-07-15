OBJECTS = $(patsubst %.c,%.build,$(wildcard *.c))

run: $(OBJECTS)
	@echo $t
	./$<
	@echo "Run Done"

build: $(OBJECTS)
	@echo "Build Done"

e_cp.build: e_cp.c
	gcc -D_GNU_SOURCE -o e_cp.build e_cp.c

%.build: %.c
	gcc -o $@ $<

clean:
	rm -f *.build
