LDFLAGS = -lglfw3 -lglad -lm -lpthread -ldl

OpenglTest: *.cpp *.hpp 
	c++  -Wall -O2 -o bin/OpenglTest *.cpp $(LDFLAGS)
	cp -r shaders -t bin/
	cp -r textures -t bin/
	cp -r models -t bin/

.Phony: test clean

test: OpenglTest
	./OpenglTest

clean:
	rm -f OpenglTest


