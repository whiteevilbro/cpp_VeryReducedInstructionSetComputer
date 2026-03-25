define uniq =
  $(eval seen :=)
  $(foreach _,$1,$(if $(filter $_,${seen}),,$(eval seen += $_)))
  ${seen}
endef

CC = clang++

COMPILER_FLAGS = -std=c++20 -O0 -g3
LIB_FLAGS = -lgtest -lgmock -pthread
WARNING_FLAGS = -Wall -Wextra -Wpedantic
SANITIZER_FLAGS = -fsanitize=address

CPP_SOURCES = $(shell find -L ./$(SOURCE_DIR) -iname "*.cpp")
CPP_HEADERS = $(shell find -L ./$(SOURCE_DIR) -iname "*.h")
HEADER_FLAGS += $(addprefix -I, $(call uniq,$(dir $(CPP_HEADERS))))

FLAGS = $(HEADER_FLAGS) $(COMPILER_FLAGS) $(WARNING_FLAGS) $(SANITIZER_FLAGS) $(LIB_FLAGS)

EXE = exec

all: clean test

$(EXE): $(CPP_SOURCES) $(CPP_HEADERS)
	$(CC) $(CPP_SOURCES) -o $@ $(FLAGS)

test: $(EXE)
	./$(EXE)

clean:
	rm -f $(EXE) && true