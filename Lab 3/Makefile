CXX = clang++
CXXFLAGS = \
	-Wall -Wextra -Werror \
	-Wno-error=unused-function \
	-Wno-error=unused-parameter \
	-Wno-error=unused-variable \
	-Wno-error=unused-but-set-variable \
	-Wno-error=unused-value \
	-Wno-sign-compare \
	-Wno-unused-command-line-argument \
	-std=c++2a -I. -g -fno-omit-frame-pointer

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

# === Lab 3 targets ===

# map reverse executable
billboard: billboard.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

# Build and execute map reverse
run_billboard: billboard
	./$<

# map reverse executable
map_reverse: map_reverse.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

# Build and execute map reverse
run_map_reverse: map_reverse
	./$<

# Anagrams executable
anagrams: anagrams.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

# Anagrams test executable
anagrams_tests: anagrams_tests.cpp anagrams.cpp
	$(CXX) $(CXXFLAGS) $^ -DCOMPILED_FOR_GTEST -lgtest -lgtest_main -o $@

# Build and execute anagrams executable
run_anagrams: anagrams
	./$<

# Build and execute anagrams tests
test_anagrams: anagrams_tests
	./$<

clean:
	rm -f billboard map_reverse anagrams anagrams_tests

.PHONY: clean run_map_reverse run_anagrams test_anagrams
