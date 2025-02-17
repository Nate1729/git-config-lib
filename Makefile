CC=gcc
C_FLAGS=-Wall --std=c89

%.o: %.c
	$(CC) $(C_FLAGS) $^ -c

git_config_lib.a: git_config.o git_config_section.o
	ar rcs $@ $^
