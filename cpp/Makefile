.PHONY: all clean headers

SRC = src
OBJ = obj
BIN = bin

EXE = $(BIN)/console_app

HDRFILES = $(wildcard $(SRC)/*.h)
SRCFILES = $(filter %.cpp, $(wildcard $(SRC)/*))
OBJFILES = $(patsubst $(SRC)/%, $(OBJ)/%.o, $(basename $(SRCFILES)))

CPPFLAGS = -DUNICODE -D_UNICODE
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -Wconversion
BUILDOBJ = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $< -c
BUILDEXE = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ -ltbb12
BUILDHDR = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -o /dev/null $(HDRFILES)

VPATH = $(SRC)

all: $(EXE)

clean:
	rm -rf $(OBJ)
	rm -rf $(BIN)

headers:
	$(BUILDHDR)

$(OBJFILES): | $(OBJ)

$(OBJ):
	mkdir $(OBJ)

$(BIN):
	mkdir $(BIN)

$(OBJ)/gp_chart.o: gp_chart.cpp gp_chart.h
	$(BUILDOBJ)

$(OBJ)/gp_proc.o: gp_proc.cpp gp_proc.h
	$(BUILDOBJ)

$(OBJ)/main.o: main.cpp gp_chart.h gp_proc.h
	$(BUILDOBJ)

$(EXE): $(OBJFILES) | $(BIN)
	$(BUILDEXE)
