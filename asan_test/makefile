all:
	#g++ test.c -o test
	gcc test.c -fsanitize=address -fsanitize-recover=address -fno-stack-protector -fno-omit-frame-pointer -o test
clean:
	rm test
