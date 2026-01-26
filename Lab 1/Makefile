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

ENV_VARS = \
	ASAN_OPTIONS=abort_on_error=1:detect_leaks=1 \
	LSAN_OPTIONS=suppressions=suppr.txt:print_suppressions=false

example_main: example.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

example_tests: example.cpp example_tests.cpp
	$(CXX) $(CXXFLAGS) $^ -DCOMPILED_FOR_GTEST -lgtest -lgtest_main -o $@

run_example: example_main
	./$<

test_example: example_tests
	$(ENV_VARS) ./$<

bomb_main: bomb.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

run_bomb: bomb_main
	$(ENV_VARS) ./$<

clean:
	rm -f bomb_main lab1 lab1_tests
	# MacOS symbol cleanup
	rm -rf *.dSYM

.PHONY: clean example_main example_tests lab1_main lab1_tests bomb_main
