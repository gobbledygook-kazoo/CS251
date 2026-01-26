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


# === References ===
references: references.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

run_references: references
	$(ENV_VARS) ./$<

# === Dedup Debugging ===
dedup: dedup.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

run_dedup: dedup
	$(ENV_VARS) ./$<

# === Programming I ===
lab2: lab2.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

lab2_tests: lab2.cpp lab2_tests.cpp
	$(CXX) $(CXXFLAGS) $^ -DCOMPILED_FOR_GTEST -lgtest -lgmock -lgtest_main -o $@

run_lab2: lab2
	$(ENV_VARS) ./$<

test_lab2: lab2_tests
	$(ENV_VARS) ./$< --gtest_color=yes

# === Programming II ===
concat: concat_files.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

concat_tests: concat_files.cpp concat_files_tests.cpp
	$(CXX) $(CXXFLAGS) $^ -DCOMPILED_FOR_GTEST -lgtest -lgmock -lgtest_main -o $@

run_concat: concat
	$(ENV_VARS) ./$<

test_concat: concat_tests
	$(ENV_VARS) ./$<

clean:
	rm -f references lab2_tests lab2 concat_tests concat dedup
	# MacOS symbol cleanup
	rm -rf *.dSYM

.PHONY: clean references dedup lab2 lab2_tests concat concat_tests
