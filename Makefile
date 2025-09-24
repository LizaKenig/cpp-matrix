CC = g++
CCFL = -Wall -Werror -Wextra --std=c++17  
BUILD_LIBS = -lgtest -lgtest_main 

LIB_OBJ_DIR := .obj_lib
LIB_SRC_DIR := .
LIB_SRC_FILES := $(wildcard $(LIB_SRC_DIR)/*.cc)
LIB_OBJ_FILES := $(patsubst $(LIB_SRC_DIR)/%.cc,$(LIB_OBJ_DIR)/%.o,$(LIB_SRC_FILES))
TESTS_OBJ_DIR := .obj_tests
TESTS_SRC_DIR := tests
TESTS_SRC_FILES := $(wildcard $(TESTS_SRC_DIR)/*.cc)
TESTS_OBJ_FILES := $(patsubst $(TESTS_SRC_DIR)/%.cc,$(TESTS_OBJ_DIR)/%.o,$(TESTS_SRC_FILES))
TESTS_OBJ_FILES_VALGRIND := $(filter-out $(TESTS_OBJ_DIR)/t_bad_alloc.o, $(TESTS_OBJ_FILES))

OS := $(shell uname -s)
ifeq ($(OS), Darwin)
	CCFL += -DOS_MAC
endif
ifeq ($(OS), Linux)
	CCFL += -DOS_LINUX
	BUILD_LIBS += -lpthread
endif

all: s21_matrix.a

s21_matrix.a: $(LIB_OBJ_FILES)
	rm -f s21_matrix.a
	ar rcs $@ $(LIB_OBJ_FILES)

test: $(TESTS_OBJ_FILES) s21_matrix.a
	$(CC) $(CCFL) -o test.out $^ $(BUILD_LIBS) -fno-elide-constructors
	./test.out

valgrind: $(TESTS_OBJ_FILES_VALGRIND) s21_matrix.a
	$(CC) $(CCFL) -o test_valgrind.out $^ $(BUILD_LIBS)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log ./test_valgrind.out

gcov_report: clean
	$(CC) $(CCFL) -fprofile-arcs -ftest-coverage ./*.cc tests/*.cc $(TEST_FLAGS) -o test_report.out -lm $(BUILD_LIBS)
	./test_report.out
	lcov -t test_report -o s21_test.info -c -d . --no-external 
	genhtml -o report s21_test.info 

$(LIB_OBJ_DIR)/%.o: $(LIB_SRC_DIR)/%.cc prepare_l
	$(CC) $(CCFL) -c -o $@ $<

$(TESTS_OBJ_DIR)/%.o: $(TESTS_SRC_DIR)/%.cc prepare_t
	$(CC) $(CCFL) -c -o $@ $<

prepare_l:
	mkdir -p $(LIB_OBJ_DIR)

prepare_t:
	mkdir -p $(TESTS_OBJ_DIR)

clean:
	rm -rf .obj* s21_matrix.a *.out
	rm -rf *.gcda *.gcno *.info valgrind.log report

rebuild: clean all
