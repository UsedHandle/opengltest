LDFLAGS = -lstdc++ -lglfw3 -lglad -lm -lpthread -ldl

OpenglTest: *.cpp *.hpp 
	gcc  -Wall -O2 -o OpenglTest *.cpp $(LDFLAGS)

.Phony: test clean

test: OpenglTest
	./OpenglTest

clean:
	rm -f OpenglTest


