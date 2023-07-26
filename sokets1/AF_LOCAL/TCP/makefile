PARENT = server
CHILD = client
CFLAGS = -Wall

run: $(PARENT) $(CHILD)

$(PARENT): $(PARENT).c
	$(CC) $(CFLAGS) -o $@ $<

$(CHILD): $(CHILD).c
	$(CC) $(CFLAGS) -o $@ $<
