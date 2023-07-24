PARENT = srv
CHILD = cln
CFLAGS = -Wall

run: $(PARENT) $(CHILD)

$(PARENT): $(PARENT).c
	$(CC) $(CFLAGS) -o $@ $<

$(CHILD): $(CHILD).c
	$(CC) $(CFLAGS) -o $@ $<
