NAME1 = server
NAME2 = client
CFLAGS = -Wall

all: $(NAME1) $(NAME2)

$(NAME1): $(NAME1).c
	$(CC) $(CFLAGS) -o $@ $<

$(NAME2): $(NAME2).c
	$(CC) $(CFLAGS) -o $@ $<
