CXX = g++-14

CXXFLAGS += -std=c++23
LDFLAGS ?=

BUILD_PATH ?= build

RELEASE ?= 0
ifeq ($(RELEASE), 0)
	CXXFLAGS += -g
else
	CXXFLAGS += -O3 -flto -DNDEBUG
endif

COMPILE = $(CXX) $(CXXFLAGS)
LINK = $(CXX) $(LDFLAGS)

EXPRESSIONS_IMPL = $(wildcard src/expressions/*.cpp) $(wildcard src/expressions/operators/*.cpp) $(wildcard src/expressions/functions/*.cpp)
EXPRESSIONS_IMPL_ = $(patsubst src/expressions/%, %, $(EXPRESSIONS_IMPL))

EXPRESSION_OUT_FILES = $(addprefix $(BUILD_PATH)/, $(EXPRESSIONS_IMPL_:.cpp=.o))

all: $(BUILD_PATH)/differentiator

differentiator: $(BUILD_PATH)/differentiator | $(BUILD_PATH)
	$(BUILD_PATH)/differentiator $(ARGS)

$(BUILD_PATH)/differentiator: $(BUILD_PATH)/lexer.o $(BUILD_PATH)/parser.o $(BUILD_PATH)/differentiator.o \
$(EXPRESSION_OUT_FILES) | $(BUILD_PATH)
	$(LINK) $^ -o $(BUILD_PATH)/differentiator

$(BUILD_PATH)/differentiator.o: src/differentiator.cpp | $(BUILD_PATH)
	$(COMPILE) src/differentiator.cpp -c -o $(BUILD_PATH)/differentiator.o

$(BUILD_PATH)/parser.o: src/parser/Parser.cpp | $(BUILD_PATH)
	$(COMPILE) src/parser/Parser.cpp -c -o $(BUILD_PATH)/parser.o

$(BUILD_PATH)/lexer.o: src/parser/Lexer.cpp | $(BUILD_PATH)
	$(COMPILE) src/parser/Lexer.cpp -c -o $(BUILD_PATH)/lexer.o

$(BUILD_PATH)/%.o: src/expressions/%.cpp | $(BUILD_PATH)
	$(COMPILE) $< -c -o $@

$(BUILD_PATH):
	@mkdir -p $(BUILD_PATH) $(BUILD_PATH)/operators $(BUILD_PATH)/functions

clean:
	rm -rf $(BUILD_PATH)

.PHONY: all differentiator clean