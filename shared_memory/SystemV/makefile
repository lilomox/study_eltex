MAIN = server
CHILD = client
CFLAGS = -Wall

run: $(MAIN) $(CHILD)
	./$<

$(MAIN): $(MAIN).c
	$(CC) $(CFLAGS) -o $@ $^

$(CHILD): $(CHILD).c
	$(CC) $(CFLAGS) -o $@ $^
