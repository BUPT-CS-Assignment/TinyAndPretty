NAME = main-service

WORK_DIR 	= $(shell pwd)
SRC_DIR  	= $(WORK_DIR)/src
BUILD_DIR 	= $(WORK_DIR)/build
INCLUDE_DIR = $(WORK_DIR)/include
PACKAGE 	= $(BUILD_DIR)/$(NAME) 
LIB			= $(BUILD_DIR)/libnedb.so
#LIB			= $(BUILD_DIR)/libnedb.a

CXXFLAGS += $(addprefix -I , $(INCLUDE_DIR)) -fPIC -O2 -Wall -Werror

MODULES = $(filter-out src,$(notdir $(shell find $(SRC_DIR) -type d ) ) )

OBJ_DIR = $(BUILD_DIR)/$(NAME)-obj

ifeq ($(MODS),)
SOURCES = $(shell find src -name "*.cpp" ! -name "*main.cpp")
else 
SOURCES = $(shell find $(addprefix src/,$(MODS)) -name "*.cpp" ! -name "*main.cpp")
endif


OBJS = $(subst cpp,o,$(subst src,$(OBJ_DIR),$(SOURCES)))

include scripts/colors.mk

root : .detect $(LIB) 
	@echo "$(C_GREEN)[$(shell date +%F%n%T)] Compile Complete!$(C_END)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo + CXX $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(LIB): $(OBJS)
	$(CXX) $(CXXFLAGS)  -shared -o $@ $^
#	ar cq $@ $^


.PHONY: clean .detect run root

run : .detect $(PACKAGE) 
	@echo  "$(C_BLUE)[$(shell date +%F%n%T)] RUNNING...$(C_END)"
	@$(PACKAGE) $(MAINARGS)
	@echo  "$(C_GREEN)[$(shell date +%F%n%T)] SUCCESSFUL RUNNING$(C_END)"


.detect :
	@echo "$(C_BLUE)Module <$(MODULES)> has been detected...$(C_END)"


clean :
	-rm -rf $(BUILD_DIR)
