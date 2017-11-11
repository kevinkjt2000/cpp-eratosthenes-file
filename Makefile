FLAGS   = -Wall -Wextra -Werror -pedantic -pedantic-errors
SRC     = $(wildcard *.cpp)
OBJ     = $(patsubst %.cpp,%.o,$(SRC))
DEP     = $(patsubst %.cpp,%.d,$(SRC))

.PHONY: all
all: $(DEP) main

.PHONY: run
run: all
	./main

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
