OUT = bin

SRC = main.c

CC = gcc

CFLAGS = -I. -lm

build:$(OUT)

$(OUT): $(SRC)
	 $(CC) -o $(OUT) $(SRC) $(CFLAGS)

clean:
	rm -f $(OUT)

run: $(OUT)
		./$(OUT)
