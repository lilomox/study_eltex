MAIN = task
CFLAGS = -Wall 

run: $(MAIN)
	./$<

$(MAIN): $(MAIN).c
	$(CC) $(CFLAGS) -o $@ $<
