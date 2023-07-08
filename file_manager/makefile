NAME_OF_FILE = file_manage

CFLAGS = -Wall

run: $(NAME_OF_FILE)
	./$^

$(NAME_OF_FILE): $(NAME_OF_FILE).o
	$(CC) $(CFLAGS) -o $@ $^ -lncurses

$(NAME_OF_FILE).o: $(NAME_OF_FILE).c
	$(CC) -c $^
