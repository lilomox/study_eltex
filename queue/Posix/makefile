MAIN = server
CHILD = client
CFLAGS = -Wall

run: $(MAIN) $(CHILD)
	./$<

$(MAIN): $(MAIN).c
	$(CC) $(CFLAGS) -o $@ $^ -lrt

$(CHILD): $(CHILD).c
	$(CC) $(CFLAGS) -o $@ $^ -lrt
