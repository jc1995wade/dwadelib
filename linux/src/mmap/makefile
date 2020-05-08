
TAG1=normal1
TAG2=normal2
res1=map_normal1.c
res2=map_normal2.c


all : $(TAG1) $(TAG2)
.PHONY : all
CFLAGS = -I.

$(TAG1) : $(res1)
	gcc -o $(TAG1) $(res1) $(CFLAGS)

$(TAG2) : $(res2)
	gcc -o $(TAG2) $(res2) $(CFLAGS)


clean:
	rm -rf $(TAG1) $(TAG2)
