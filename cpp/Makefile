.PHONY: all clean headers

SRC = src
OBJ = obj
BIN = bin

EXE = $(BIN)/console_app

HDRFILES = $(wildcard $(SRC)/*.h)
SRCFILES = $(filter %.cpp, $(wildcard $(SRC)/*))
OBJFILES = $(patsubst $(SRC)/%, $(OBJ)/%.o, $(basename $(SRCFILES)))

CPPLANGV = -std=c++20
WARNINGS = -Wall -Wextra -Wpedantic -Wconversion
BUILDOBJ = g++ $(CPPLANGV) $(WARNINGS) -o $@ $< -c
BUILDEXE = g++ $(CPPLANGV) $(WARNINGS) -o $@ $^ -ltbb12
BUILDHDR = g++ $(CPPLANGV) $(WARNINGS) -o /dev/null $(HDRFILES)

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
