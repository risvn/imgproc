OUT = bin_img_prc

SRC = img_prc.c

CC = gcc

CFLAGS = -Wall -I. -lm

build:$(OUT)

$(OUT): $(SRC)
	 $(CC) -o $(OUT) $(SRC) $(CFLAGS)

clean:
	rm -f $(OUT)

run: $(OUT)
		./$(OUT)
