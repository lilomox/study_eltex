PARENT = server
CHILD = client
CHILD2 = client2
CFLAGS = -Wall

run: $(PARENT) $(CHILD) $(CHILD2)

$(PARENT): $(PARENT).c
	$(CC) $(CFLAGS) -o $@ $<

$(CHILD): $(CHILD).c
	$(CC) $(CFLAGS) -o $@ $<

$(CHILD2): $(CHILD2).c
	$(CC) $(CFLAGS) -o $@ $<