CC 		:= gcc
CCFLAGS	:= -O3 -Wall
SRC		:= main.c

all:
	$(CC) $(CCFLAGS) $(SRC)
clean:
	rm a.out