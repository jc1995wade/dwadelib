
INC = include


SOURCES = src/dw_time.c
SOURCES += example/dw_time_test.c

OBJ = bin/dw_test

all:$(SOURCES)
	gcc -g $(SOURCES) -I$(INC) -o $(OBJ)

debug:
	gcc -g $(SOURCES) -I$(INC) -o $(OBJ) -DEBUG


clean:
	rm -rf $(OBJ)
	rm -rf obj/*
