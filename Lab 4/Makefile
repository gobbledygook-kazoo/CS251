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

build/%.o: %.cpp
	mkdir -p build && $(CXX) $(CXXFLAGS) -c $< -o $@

# === Lab 4 targets ===

# Vector votes test executable
votes_tests: build/vector_votes.o build/map_votes.o build/votes_tests.o
	$(CXX) $(CXXFLAGS) $^ -DCOMPILED_FOR_GTEST -lgtest -lgtest_main -o $@

# Build and execute anagrams tests
test_vector_votes: votes_tests
	./$< --gtest_color=yes --gtest_filter="VectorVotes.*"

# Build and execute anagrams tests
test_map_votes: votes_tests
	./$< --gtest_color=yes --gtest_filter="MapVotes.*"

# Build and execute vector votes
test_all_votes: votes_tests
	./$< --gtest_color=yes

# Build votes_main
votes_main: build/vector_votes.o build/map_votes.o build/votes_main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

#Build and execute votes_main
run_votes: votes_main
	./$<

clean:
	rm -f votes_tests votes_main build/*
	# MacOS symbol cleanup
	rm -rf *.dSYM

.PHONY: clean 
