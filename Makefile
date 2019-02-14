CXX=mpic++
CXXFLAGS=-g -O3 -pedantic -std=c++11 -Wall -Wextra
INCLUDE=-I.

SRC=main.cpp polgenerator.cpp polmodtree.cpp mathextra.cpp

OBJ_DIR=./build
OBJ=$(SRC:%.cpp=$(OBJ_DIR)/%.o)

RM=rm -v

TARGETS=congr
DEFAULT_TARGET=congr


default: $(DEFAULT_TARGET)

congr: $(OBJ)

$(TARGETS):
	$(CXX) -o $@ $(OBJ) $(INCLUDE) $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: $(SHARE_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	$(RM) $(OBJ)

cleanall:
	$(RM) $(TARGETS) $(OBJ)
