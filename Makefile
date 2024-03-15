CC:=gcc
CFLAGS:=-g
MAIN_S=main.c
MAIN_O=$(MAIN_S:.c=.o)

main: $(MAIN_O)
	$(CC) $(CFLAGS) $^ -o $@

$(MAIN_O): $(MAIN_S)
	$(CC) $(CFLAGS) -c $^ -o $@