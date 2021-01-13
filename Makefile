docs:
	doxygen .Doxyfile

docs-clean:
	@echo "Cleaning up docs"
	rm -rf docs/

credits:
	@echo "Fly by wire System"
	@echo "> A project developed by Owanesh for University of Florence"
	@echo "> See more at https://github.com/Owanesh/unifi-fbw777"
        
install:
	@echo "Creating a tmp folder"
	mkdir -p .tmp/
	mkdir -p .log/
	mkdir -p .bin/

clean:
	@echo "Cleaning up tmpfolder"
	rm -rf .tmp/*
	rm -rf .log/*
	rm -rf .bin/*

clean_all: clean docs-clean

#compile:
#	aeroplanetty: main.o pfc wes.o transducers.o pfcDisconnectSwitch.o utilities.o signals.o
#		cc -o "aeroplanetty" main.o pfc.o wes.o transducers.o pfcDisconnectSwitch.o utilities.o signals.o
#	main.o: main.c   header/utilities.h
#		cc -c main.c
#	pfc.o: src/pfc.c src/header/pfc.h src/util/headers/utilities.h
#		cc -c pfc.c
#	wes.o: src/wes.c src/header/wes.h src/headers/pfcDisconnectSwitch.h src/util/headers/constant.h src/util/header/utilities.h
#		cc -c wes.c
#	failureGenerator.o: src/failureGenerator.c src/headers/failureGenerator.c src/util/header/utilities.h
#		cc -c failureGenerator.c
#	transducers.o: src/transducers.c src/header/transducers.h src/headers/pfc.h
#		cc -c transducers.c
#	pfcDisconnectSwitch.o: src/pfcDisconnectSwitch.c src/headers/pfcDisconnectSwitch.h src/headers/pfc.h
#		cc -c pfcDisconnectSwitch.c
#	utilities.o: src/util/utilities.c src/util/headers/utilities.h
#		cc -c utilities.c
#	signals.o: src/util/signals.c src/util/headers/signals.h
#		cc -c signals.c
