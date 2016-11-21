LIB_NAME=libsimex.a
BUILD_DIR=$(CURDIR)/build
CHECKED_BUILD_DIR=$(BUILD_DIR)/checked
RELEASE_BUILD_DIR=$(BUILD_DIR)/release
TEST_BUILD_DIR=$(CHECKED_BUILD_DIR)/test
CHECKED_LIB=$(CHECKED_BUILD_DIR)/$(LIB_NAME)
RELEASE_LIB=$(RELEASE_BUILD_DIR)/$(LIB_NAME)
SRCDIR=$(PWD)/src
TESTDIR=$(PWD)/test
TESTDIRS=$(TESTDIR) $(TESTDIR)/sasm
DIRS=$(SRCDIR) $(SRCDIR)/Instruction $(SRCDIR)/sasm $(SRCDIR)/sasm/Filter \
     $(SRCDIR)/sasm/LineFilter $(SRCDIR)/sasm/WhitespaceFilter \
     $(SRCDIR)/sasm/PreprocessorLexer
CHECKED_DIRS=$(filter-out $(SRCDIR),$(patsubst $(SRCDIR)/%,$(CHECKED_BUILD_DIR)/%,$(DIRS)))
RELEASE_DIRS=$(filter-out $(SRCDIR),$(patsubst $(SRCDIR)/%,$(RELEASE_BUILD_DIR)/%,$(DIRS)))
TEST_DIRS=$(filter-out $(TESTDIR),$(patsubst $(TESTDIR)/%,$(TEST_BUILD_DIR)/%,$(TESTDIRS)))
SOURCES=$(foreach d,$(DIRS),$(wildcard $(d)/*.cpp))
STRIPPED_SOURCES=$(patsubst $(SRCDIR)/%,%,$(SOURCES))
CHECKED_OBJECTS=$(patsubst %.cpp,$(CHECKED_BUILD_DIR)/%.o,$(STRIPPED_SOURCES))
RELEASE_OBJECTS=$(patsubst %.cpp,$(RELEASE_BUILD_DIR)/%.o,$(STRIPPED_SOURCES))
TEST_SOURCES=$(foreach d,$(TESTDIRS),$(wildcard $(d)/*.cpp))
STRIPPED_TEST_SOURCES=$(patsubst $(TESTDIR)/%,%,$(TEST_SOURCES))
TEST_OBJECTS=$(patsubst %.cpp,$(TEST_BUILD_DIR)/%.o,$(STRIPPED_TEST_SOURCES))
TESTLIBSIMEX=$(CHECKED_BUILD_DIR)/testlibsimex

#Dependencies
GTEST_DIR=contrib/gtest/googletest
GTEST_OBJ=$(TEST_BUILD_DIR)/gtest-all.o

#Compilers
CHECKED_CXX=$(CXX)
RELEASE_CXX=$(CXX)
COMMON_CXXFLAGS=-std=c++14 -I $(PWD)/include $(CXXFLAGS)
CHECKED_CXXFLAGS=$(COMMON_CXXFLAGS) -O0 -fprofile-arcs -ftest-coverage
RELEASE_CXXFLAGS=$(COMMON_CXXFLAGS) -O2
TEST_CXXFLAGS=$(RELEASE_CXXFLAGS) -I $(GTEST_DIR) -I $(GTEST_DIR)/include

.PHONY: ALL lib.checked lib.release test clean

ALL: lib.checked lib.release
lib.checked: $(CHECKED_DIRS) $(CHECKED_LIB)
lib.release: $(RELEASE_DIRS) $(RELEASE_LIB)

$(RELEASE_DIRS) $(CHECKED_DIRS) $(TEST_DIRS) $(TEST_BUILD_DIR):
	mkdir -p $@

$(CHECKED_LIB) : $(CHECKED_OBJECTS)
	$(AR) rcs $@ $(CHECKED_OBJECTS)

$(RELEASE_LIB) : $(RELEASE_OBJECTS)
	$(AR) rcs $@ $(RELEASE_OBJECTS)

$(GTEST_OBJ): $(GTEST_DIR)/src/gtest-all.cc
	$(RELEASE_CXX) $(TEST_CXXFLAGS) -c -o $@ $<

$(TEST_BUILD_DIR)/%.o: $(TESTDIR)/%.cpp
	$(RELEASE_CXX) $(TEST_CXXFLAGS) -c -o $@ $<

$(CHECKED_BUILD_DIR)/%.o: $(SRCDIR)/%.cpp
	$(CHECKED_CXX) $(CHECKED_CXXFLAGS) -c -o $@ $<

$(RELEASE_BUILD_DIR)/%.o: $(SRCDIR)/%.cpp
	$(RELEASE_CXX) $(RELEASE_CXXFLAGS) -c -o $@ $<

test: $(TEST_BUILD_DIR) $(TEST_DIRS) lib.checked $(TESTLIBSIMEX)
	$(TESTLIBSIMEX)

$(TESTLIBSIMEX): $(CHECKED_OBJECTS) $(TEST_OBJECTS) $(GTEST_OBJ)
	find $(TEST_BUILD_DIR) -name "*.gcda" -exec rm {} \; -print
	rm -f gtest-all.gcda
	$(RELEASE_CXX) $(TEST_CXXFLAGS) -fprofile-arcs -o $@ $(TEST_OBJECTS) $(CHECKED_OBJECTS) $(GTEST_OBJ)

clean:
	rm -rf build
