NAME = main
SRC = src
OBJ = obj
BIN = bin
TEST = test

CFLAGS = -Wall -Wextra -O3 -I$(SRC)
LDFLAGS =
LDLIBS =

EXE = $(BIN)/$(NAME)

BIN_SRCS = $(wildcard $(SRC)/*.c)
LIB_SRCS = $(wildcard $(SRC)/*/*.c)
ALL_SRCS = $(BIN_SRCS) $(LIB_SRCS)

LIB_OBJS = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(LIB_SRCS))
BIN_OBJS = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(BIN_SRCS))
BINS = $(patsubst $(SRC)/%.c,$(BIN)/%,$(BIN_SRCS))
DEPS = $(patsubst $(SRC)/%.c,$(OBJ)/%.d,$(ALL_SRCS))

TEST_SRCS = $(wildcard $(TEST)/*.c)
TEST_BINS = $(patsubst $(TEST)/%.c,$(BIN)/$(TEST)/%,$(TEST_SRCS))

all: $(BINS)

$(BIN)/%: $(OBJ)/%.o $(LIB_OBJS)
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -c -MMD -o $@

run: $(EXE)
	@$(EXE)

test: $(TEST_BINS)
	@$(TEST)/run_tests.sh $^

$(BIN)/$(TEST)/%: $(TEST)/%.c $(LIB_OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDFLAGS) $< $(LIB_OBJS) -MMD -o $@

hot:
	@find $(SRC) $(TEST) -type f | entr -c make test

clean:
	@rm -rf $(BIN) $(OBJ)

-include $(DEPS)

.PHONY: all run test hot clean
.SECONDARY: $(LIB_OBJS) $(BIN_OBJS)
