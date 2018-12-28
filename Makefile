CXX=g++
CXXFLAGS=`pkg-config sdl2 --cflags`
LDFLAGS=`pkg-config sdl2 --libs`
OBJS=main.o Color.o Rasterizer.o

triangleraster:	$(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o triangleraster

clean:
	rm -f triangleraster
	rm -f $(OBJS)

main.o: main.cpp
Color.o: Color.cpp
Rasterizer.o: Rasterizer.cpp
