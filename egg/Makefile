OBJS=Head.o TexLoader.o

all: $(OBJS)
	g++ -o Head $(OBJS) -lGL -lGLU -lglut

Head.o : Head.cpp
	g++ -c $<

TexLoader.o : TexLoader.cpp
	g++ -c $<

Head.cpp: texheader.h

TexLoader.cpp: texheader.h

clean:
	rm -f *.o Head

run: all
	./Head
