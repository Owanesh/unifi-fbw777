docs:
	doxygen .Doxyfile

docs-clean:
	@echo "Cleaning up..."
	rm -rf docs/

credits:
	@echo "Fly by wire System"
	@echo "> A project developed by Owanesh for University of Florence"
	@echo "> See more at https://github.com/Owanesh/unifi-fbw777"
        
install:
	@echo "Creating a tmp folder"
	mkdir -p .tmp/

clean:
	@echo "Cleaning up..."
	rm -rf .tmp/*

