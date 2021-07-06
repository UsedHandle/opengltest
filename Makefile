LDFLAGS = -lstdc++ -lglfw3 -lglad -lm -lpthread -ldl

OpenglTest: *.cpp *.hpp 
	g++ $(CFLAGS) -o OpenglTest *.cpp $(LDFLAGS)

.Phony: test clean

test: OpenglTest
	./OpenglTest

clean:
	rm -f OpenglTest


