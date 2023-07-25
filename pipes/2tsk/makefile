NAME1 = 1tsk
NAME2 = 2tsk
CFLAGS = -Wall -g -O0

run: $(NAME1) $(NAME2)
	./$<

$(NAME1): $(NAME1).c
	$(CC) $(CFLAGS) -o $@ $^ 

$(NAME2): $(NAME2).c
	$(CC) $(CFLAGS) -o $@ $^
