TARGET = cw

GXX = g++

PREF_SRC = ./source/
PREF_OBJ = ./obj/

SRC = $(wildcard $(PREF_SRC)*.cpp)
OBJ = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.o, $(SRC))
DEPS = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.d, $(SRC))

$(TARGET): $(OBJ)
	$(GXX) $(OBJ) -o $(TARGET)

$(PREF_OBJ):
	mkdir -p $(PREF_OBJ)

$(PREF_OBJ)%.o: $(PREF_SRC)%.cpp | $(PREF_OBJ)
	$(GXX) -MMD -MP -c $< -o $@

clean:
	rm -f $(TARGET) $(PREF_OBJ)*.o $(PREF_OBJ)*.d
	rm -d obj 

docs:
	doxygen Doxyfile

.PHONY: run clean docs

-include $(DEPS)