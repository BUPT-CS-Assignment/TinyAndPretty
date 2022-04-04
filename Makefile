NAME = main-service

WORK_DIR 	= $(shell pwd)
SRC_DIR  	= $(WORK_DIR)/src
BUILD_DIR 	= $(WORK_DIR)/build
INCLUDE_DIR = $(WORK_DIR)/include

PACKAGE 	= $(BUILD_DIR)/$(NAME) 

CXXFLAGS += $(addprefix -I , $(INCLUDE_DIR)) -Wall -std=c++17 -g

MODULES = $(filter-out src,$(notdir $(shell find $(SRC_DIR) -type d ) ) )

OBJ_DIR = $(BUILD_DIR)/$(NAME)-obj

ifeq ($(MODS),)
SOURCES = $(shell find src -name "*.cpp" ! -name "*main.cpp")
else 
SOURCES = $(shell find $(addprefix src/,$(MODS)) -name "*.cpp" ! -name "*main.cpp")
endif


ifeq ($(MODE),debug)
CXXFLAGS += -D DEBUG -g
SOURCES += src/test-main.cpp
else
SOURCES += src/main.cpp
endif

OBJS = $(subst cpp,o,$(subst src,$(OBJ_DIR),$(SOURCES)))

include scripts/colors.mk

root : .detect $(PACKAGE) 
	@echo "$(C_GREEN)[$(shell date +%F%n%T)] Compile Complete!$(C_END)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo + CXX $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(PACKAGE): $(OBJS)
	@echo + LD $@
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

.PHONY: clean .detect run root

run : .detect $(PACKAGE) 
	@echo  "$(C_BLUE)[$(shell date +%F%n%T)] RUNNING...$(C_END)"
	@$(PACKAGE) $(MAINARGS)
	@echo  "$(C_GREEN)[$(shell date +%F%n%T)] SUCCESSFUL RUNNING$(C_END)"


.detect :
ifeq ($(MODE),debug)
	@echo "$(C_YELLOW)Debug mode start up$(C_END)"
endif
	@echo "$(C_BLUE)Module <$(MODULES)> has been detected...$(C_END)"


clean :
	-rm -rf $(BUILD_DIR)
