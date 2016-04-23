CXX = g++
CXXFLAGS = -O2 -std=c++11
BIN = ./bin/
SRC = ./src/

TASKS = a b c d e f g h

all: bin $(TASKS)

bin:
	mkdir -p ./bin/

%: $(BIN)%.o
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN)%.o: $(SRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TASKS)