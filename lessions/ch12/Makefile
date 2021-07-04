OBJECTS = $(patsubst %.c,%.build,$(wildcard *.c))

run: $(OBJECTS)
	@echo $t
	./$<
	@echo "Run Done"

build: $(OBJECTS)
	@echo "Build Done"

%.build: %.c
	gcc -g -o $@ $< -lcrypt

clean:
	rm -f *.build
