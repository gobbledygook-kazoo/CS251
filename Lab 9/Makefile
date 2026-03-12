CXX = clang++
# Unused: warn, but annoying to block compilation on
# Sign compare: noisy
# Command line arg: noisy, not relevant to students
CXXFLAGS = \
	-Wall -Wextra -Werror \
	-Wno-error=unused-function \
	-Wno-error=unused-parameter \
	-Wno-error=unused-variable \
	-Wno-error=unused-but-set-variable \
	-Wno-error=unused-value \
	-Wno-sign-compare \
	-Wno-unused-command-line-argument \
	-std=c++2a -I. -g -fno-omit-frame-pointer \
	-fsanitize=address,undefined

ENV_VARS = ASAN_OPTIONS=detect_leaks=1 LSAN_OPTIONS=suppressions=suppr.txt:print_suppressions=false

# On Ubuntu and WSL, googletest is installed to /usr/include or
# /usr/local/include, which are used by default.

# On Mac, we need to manually include them in our path. Brew installs to
# different locations on Intel/Silicon, so ask brew where things live.
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	GTEST_PREFIX := $(shell brew --prefix googletest)
	LLVM_PREFIX := $(shell brew --prefix llvm)
	CXX := $(LLVM_PREFIX)/bin/clang++
	CXXFLAGS += -I$(GTEST_PREFIX)/include
	CXXFLAGS += -L$(GTEST_PREFIX)/lib
	CXXFLAGS += -L$(LLVM_PREFIX)/lib/c++
	CXXFLAGS += -Wno-character-conversion
endif

build/%.o: %.cpp string_tree.h
	mkdir -p build && $(CXX) $(CXXFLAGS) -c $< -o $@

string_tree_tests: build/string_tree.o build/string_tree_tests.o
	$(CXX) $(CXXFLAGS) $^ -lgtest -lgmock -lgtest_main -o $@

test_ctor: string_tree_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="StringTreeCtor*"

test_serialize: string_tree_tests
	$(ENV_VARS) ./$< --gtest_color=yes --gtest_filter="StringTreeSerialize*"

test_all: string_tree_tests
	$(ENV_VARS) ./$< --gtest_color=yes

clean:
	rm -f string_tree_tests build/*
	rm -f *.dSYM
