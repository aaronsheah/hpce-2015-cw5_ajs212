SHELL=/bin/bash

CPPFLAGS += -std=c++11 -W -Wall  -g
CPPFLAGS += -O3
CPPFLAGS += -I include

##### TBB Dependencies #####

LDLIBS = -ltbb

# TBB_INC_DIR ?= /usr/local/include
TBB_INC_DIR ?= /usr/include/

# TBB_LIB_DIR ?= /usr/local/lib
TBB_LIB_DIR ?= /usr/lib/

CPPFLAGS += -I $(TBB_INC_DIR)
LDFLAGS += -L $(TBB_LIB_DIR)


######### OpenCL #########


LDLIBS += -lOpenCL

##########################

lib/libpuzzler.a : provider/*.cpp provider/*.hpp
	cd provider && $(MAKE) all

bin/% : src/%.cpp lib/libpuzzler.a
	-mkdir -p bin
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS) -Llib -lpuzzler

all : bin/execute_puzzle bin/create_puzzle_input bin/run_puzzle bin/compare_puzzle_output
