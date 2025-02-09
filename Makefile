OBJDIR = .obj/
LOGDIR = .log/
DOCDIR = docs/
TARGET = aeroplanetty
CC = cc
CFLAGS = -std=gnu99
LFLAGS =
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
LFLAGS = -lm
else
LFLAGS = # MacOS, Darwin
endif

all: clean prepare $(TARGET) credits

docs:
	@printf "\e[32m📄 Making the magic with docs\e[0m\n"
	doxygen .Doxyfile

docs-clean:
	@printf "\e[1;33m🧹 Cleaning up documentation\e[0m\n"
	rm -rf $(DOCDIR)
	
help:
	@echo "Fly by wire System"
	@echo "\tmake docs\tcompile docs and it will be avaiable at docs/html/index.html"
	@echo "\tmake clean_all\tremoves docs and binaries/log folder of project"
	@echo "\tmake clean\tremoves binaries and log folder only"
	@echo "\tmake \t\tcompiles project and generate an executable ./aeroplanetty"
	@echo "\tmake help\tprint this menu"
       

credits:
	@echo ""
	@echo "Fly by wire System"
	@echo "> A project developed by Owanesh for University of Florence"
	@echo "> See more at https://github.com/Owanesh/unifi-fbw777"
	@echo "> Check the LICENSE https://github.com/Owanesh/unifi-fbw777/blob/main/LICENSE"
        
clean:
	@printf "\e[1;33m🧹 Cleaning up\e[0m\n"
	rm -rf $(LOGDIR)
	rm -rf $(OBJDIR)

clean_all: clean docs-clean

prepare:
	@printf "\e[1;33m🗜 Preparing the enviroment\e[0m\n"
	mkdir -p $(OBJDIR)
	mkdir -p $(LOGDIR)
	rm -f $(TARGET)
	@printf "\e[32m✨ Ready to compile\e[0m\n"



# compile
$(TARGET): main.o wes.o failureGenerator.o transducers.o pfcDisconnectSwitch.o pfc.o utilities.o signals.o
	$(CC) -o "aeroplanetty" $(OBJDIR)main.o $(OBJDIR)wes.o $(OBJDIR)failureGenerator.o \
	$(OBJDIR)transducers.o $(OBJDIR)pfcDisconnectSwitch.o $(OBJDIR)pfc.o \
	$(OBJDIR)utilities.o $(OBJDIR)signals.o  $(LFLAGS)
main.o: 
	$(CC) $(CFLAGS) -c  src/main.c -o $(OBJDIR)main.o $(LFLAGS)
wes.o: src/headers/wes.h
	$(CC) $(CFLAGS) -c  src/wes.c -o $(OBJDIR)wes.o $(LFLAGS)
failureGenerator.o: src/headers/failureGenerator.h
	$(CC) $(CFLAGS) -c  src/failureGenerator.c -o $(OBJDIR)failureGenerator.o $(LFLAGS)
transducers.o: src/headers/transducers.h
	$(CC) $(CFLAGS) -c  src/transducers.c -o $(OBJDIR)transducers.o $(LFLAGS)
pfcDisconnectSwitch.o: src/headers/pfcDisconnectSwitch.h
	$(CC) $(CFLAGS) -c  src/pfcDisconnectSwitch.c -o $(OBJDIR)pfcDisconnectSwitch.o $(LFLAGS)
pfc.o: src/headers/pfc.h
	$(CC) $(CFLAGS) -c  src/pfc.c -o $(OBJDIR)pfc.o $(LFLAGS)
utilities.o: src/util/headers/utilities.h
	$(CC) $(CFLAGS) -c  src/util/utilities.c -o $(OBJDIR)utilities.o $(LFLAGS)
signals.o: src/util/headers/signals.h
	$(CC) $(CFLAGS) -c  src/util/signals.c -o $(OBJDIR)signals.o $(LFLAGS)

