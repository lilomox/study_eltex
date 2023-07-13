NAME = shop
DLIBS = -lpthread
CFLAGS = -Wall

run: $(NAME)
	./$^

$(NAME): $(NAME).o
	$(CC) $(CFLAGS) -o $@ $^ $(DLIBS)

$(NAME).o: $(NAME).c
	$(CC) -c $^
