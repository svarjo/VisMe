#By default we copy setup.ini next to the binary
$(BIN_DIR)/setup.ini: $(BIN_DIR) 
	$(CP) $(PROJECT_DIR)/data/setup.ini $(BIN_DIR)/

#Operations we have to do in order to prepare copy
CopySettings: $(BIN_DIR)/setup.ini
