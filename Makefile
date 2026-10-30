CC ?= clang
NAME = main
SRC = src
OBJ = obj
BIN = bin
TEST = test

# NOTE(Giovanni): platform-specific commands & extensions
ifeq ($(OS),Windows_NT)
	CC = clang
	EXT = .exe
    MKDIR = @if not exist "$(subst /,\,$(@D))" mkdir "$(subst /,\,$(@D))"
    CLEAN_CMD = @if exist $(BIN) rmdir /s /q $(BIN) & if exist $(OBJ) rmdir /s /q $(OBJ)
else
    EXT =
    MKDIR = @mkdir -p $(@D)
    CLEAN_CMD = @rm -rf $(BIN) $(OBJ)
endif

CFLAGS = -Wall -Wextra -O3 -I$(SRC)
LDFLAGS =
LDLIBS =

EXE = $(BIN)/$(NAME)$(EXT)

BIN_SRCS = $(wildcard $(SRC)/*.c)
LIB_SRCS = $(wildcard $(SRC)/*/*.c)
ALL_SRCS = $(BIN_SRCS) $(LIB_SRCS)

LIB_OBJS = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(LIB_SRCS))
BIN_OBJS = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(BIN_SRCS))
BINS = $(patsubst $(SRC)/%.c,$(BIN)/%$(EXT),$(BIN_SRCS))
DEPS = $(patsubst $(SRC)/%.c,$(OBJ)/%.d,$(ALL_SRCS))

TEST_SRCS = $(wildcard $(TEST)/*.c)
TEST_OBJS = $(patsubst $(TEST)/%.c,$(OBJ)/$(TEST)/%.o,$(TEST_SRCS))
TEST_BINS = $(patsubst $(TEST)/%.c,$(BIN)/$(TEST)/%$(EXT),$(TEST_SRCS))
TEST_DEPS = $(patsubst $(TEST)/%.c,$(OBJ)/$(TEST)/%.d,$(TEST_SRCS))

all: $(BINS)

$(BIN)/%$(EXT): $(OBJ)/%.o $(LIB_OBJS)
	$(MKDIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(MKDIR)
	$(CC) $(CFLAGS) $< -c -MMD -o $@

$(OBJ)/$(TEST)/%.o: $(TEST)/%.c
	$(MKDIR)
	$(CC) $(CFLAGS) $< -c -MMD -o $@

$(BIN)/$(TEST)/%$(EXT): $(OBJ)/$(TEST)/%.o $(LIB_OBJS)
	$(MKDIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

run: $(EXE)
	@$(EXE)

test: $(TEST_BINS)
	@bash $(TEST)/run_tests.sh $^

hot:
	@find $(SRC) $(TEST) -type f | entr -c make test

clean:
	$(CLEAN_CMD)

-include $(DEPS) $(TEST_DEPS)

.PHONY: all run test hot clean
