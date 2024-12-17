SRCS = $(wildcard sourse/*.cpp)

OBJS = $(SRCS:.cpp=.o)

all: hello 

hello: $(OBJS)
	g++ $(OBJS) main.o -o hell.exe

main.o: main.cpp
	g++ -I ./headers main.cpp

%.o: %.cpp
	g++ -I ./headers -c $< -o $@

clean:
	DEL *.o
	DEL *.exe