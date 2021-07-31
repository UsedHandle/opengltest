LDFLAGS = -lstdc++ -lglfw3 -lglad -lm -lpthread -ldl

OpenglTest: *.cpp *.hpp 
	cc  -Wall -O2 -o bin/OpenglTest *.cpp $(LDFLAGS)
	cp -r shaders -t bin/

.Phony: test clean

test: OpenglTest
	./OpenglTest

clean:
	rm -f OpenglTest


