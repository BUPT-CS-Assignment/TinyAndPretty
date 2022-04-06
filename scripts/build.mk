OBJS = $(subst cpp,o,$(subst src,$(OBJ_DIR),$(SOURCES)))

.DEFAULT_GOAL := root

root : .detect $(PACKAGE) 
	@echo "$(C_GREEN)[$(shell date +%F%n%T)] Compile Complete!$(C_END)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo + CXX $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(PACKAGE): $(OBJS)
	@echo + LD $@
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

.PHONY: run root

run : .detect $(PACKAGE) 
	@echo  "$(C_BLUE)[$(shell date +%F%n%T)] RUNNING...$(C_END)"
	@$(PACKAGE) $(MAINARGS)
	@echo  "$(C_GREEN)[$(shell date +%F%n%T)] SUCCESSFUL RUNNING$(C_END)"

