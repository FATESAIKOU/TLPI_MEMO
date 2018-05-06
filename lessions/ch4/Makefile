run: build
	./t_freopen.build
	@echo "Run Done"

build: build_t_freopen
	@echo "Build Done"

build_t_freopen: t_freopen.c
	gcc -o t_freopen.build t_freopen.c

clean:
	rm -f t_freopen
