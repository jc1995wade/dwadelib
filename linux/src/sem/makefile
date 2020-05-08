
all : wait post
.PHONY : all
CFLAGS = -pthread

wait : sem_v.c
	gcc -o wait sem_v.c $(CFLAGS)

post : sem_p.c
	gcc -o post sem_p.c $(CFLAGS)


clean:
	rm -rf wait post
