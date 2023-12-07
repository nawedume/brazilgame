EXE = build.out
LIB_DIR = lib
BUILD_DIR = build/
$(shell mkdir -p $(BUILD_DIR))

SRC_DIR = src
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
LIBS = $(LIB_DIR)/glad.cpp

OBJS = $(addprefix $(BUILD_DIR), $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

CXXFLAGS = -std=c++17 -lglfw -lassimp -I./lib/glm -I./lib -I./include -Wall -g 
DEBUG_FLAGS = -D DEBUG

ECHO_MESSAGE = "Linux"

# rules

$(BUILD_DIR)%.o:$(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS) $(DEBUG_FLAGS)

clean:
	rm -rf $(EXE) $(BUILD_DIR)

test:
	make && ./build.out