FLAGS   = -Wall -Wextra -Werror -pedantic -pedantic-errors
SRC     = $(wildcard *.cpp)
OBJ     = $(patsubst %.cpp,%.o,$(SRC))
DEP     = $(patsubst %.cpp,%.d,$(SRC))

.PHONY: all
all: catch $(DEP) main

.PHONY: run
run: all
	./main

.PHONY: catch
catch:
	wget -nc https://github.com/catchorg/Catch2/releases/download/v2.0.1/catch.hpp

%.d:
	$(CXX) -MM $*.cpp > $*.d

%.o: %.cpp
	$(CXX) $(FLAGS) -c $*.cpp

main: $(OBJ)
	$(CXX) $(OBJ) -o main

.PHONY: clean
clean:
	rm -f main *.a *.o *.d

-include $(DEP)
