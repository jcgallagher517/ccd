ccd: src/main.c
	cc src/main.c -o bin/ccd

clean:
	rm bin/ccd

run: test.sh
	./test.sh
