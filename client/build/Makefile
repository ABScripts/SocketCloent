#compiler variables
CFLAGS = -pthread
CC = g++

#Files and pathes variables
OWN = main.o client.o
 # files used specially for that sub-project (server)
OWNFOLDR = ../src
 # folder for "own" files

DEP = connection.o logger.o peermanager.o socket.o
# dependemcies 
DEPPATH = ../../shared/build
# dependencie`s path

ifeq ($(OS), Windows_NT)
	DEP += winsock.o
else
	UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	DEP += unixsock.o
endif
endif

OWNFULL = $(foreach file,$(OWN),$(addprefix $(OWNFOLDR),/$(file)))
DEPFULL = $(foreach file,$(DEP),$(addprefix $(DEPPATH),/$(file)))

a.out: $(OWNFULL)
	@cd $(DEPPATH) && $(MAKE) --no-print-directory
    # run "make" for dependecie files
	@$(CC) -o $@ $(OWN) $(DEPFULL) $(CFLAGS)

%.o:
	@$(CC) -c $(subst .o,.cpp,$@) $(CFLAGS)

clean:
	@rm $(OWN)
