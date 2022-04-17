OBJS = $(subst cpp,o,$(subst src,$(OBJ_DIR),$(SOURCES)))

.DEFAULT_GOAL := root

root : .detect $(PACKAGE) 
	@echo "$(C_GREEN)[$(shell date +%F%n%T)] Compile Complete!$(C_END)"

-include $(OBJS:.o=.d)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo + CXX $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $< 
	@$(CXX) $(INCLUDES) $< -MM -MF $(subst .o,.d,$@) -MT $@ -MT $(subst .o,.d,$@)

$(PACKAGE): $(OBJS)
	@echo + LD $@
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LDFLAGS) $(DATABASE)

.PHONY: run root 

run : .detect $(PACKAGE) 
	@echo  "$(C_BLUE)[$(shell date +%F%n%T)] RUNNING...$(C_END)"
	@$(PACKAGE) $(MAINARGS)
	@echo  "$(C_GREEN)[$(shell date +%F%n%T)] SUCCESSFUL RUNNING$(C_END)"

