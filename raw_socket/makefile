NAME = sniffer
CFLAGS = -Wall

run: $(NAME)
	sudo ./$^

$(NAME): $(NAME).c
	$(CC) $(CFLAGS) -o $@ $^
