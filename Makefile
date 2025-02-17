CC=gcc
C_FLAGS=-Wall --std=c89

%.o: src/%.c
	$(CC) $(C_FLAGS) $^ -c

git_config_lib.a: git_config.o git_config_section.o
	ar rcs $@ $^

clean:
	rm *.o
