
TAG1=recv
TAG2=send
res1=recv.c
res2=send.c


all : $(TAG1) $(TAG2)
.PHONY : all
CFLAGS = -I.

$(TAG1) : $(res1)
	gcc -o $(TAG1) $(res1) $(CFLAGS)

$(TAG2) : $(res2)
	gcc -o $(TAG2) $(res2) $(CFLAGS)


clean:
	rm -rf $(TAG1) $(TAG2)
