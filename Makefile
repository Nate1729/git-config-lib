CC=gcc
C_FLAGS=-Wall --std=c99

%.o: src/%.c
	$(CC) $(C_FLAGS) $^ -c

library: git_config.o git_config_section.o
	ar rcs libgit_config.a $^

clean:
	rm *.o *.a run_tests


test: $(wildcard tests/*.c) git_config.o git_config_section.o
	$(CC) $^ -lcriterion -o run_tests -I src/ 
