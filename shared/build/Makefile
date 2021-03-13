#compiler variables
CC = g++
CFLAGS = -pthread

#Files and pathes variables
COMM = connection.o logger.o peermanager.o
#common files
NTWR = socket.o
#network specific files
COMMPATH = ../src/
NTWRPATH = ../src/network

ALL = $(foreach file,$(COMM),$(addprefix $(COMMPATH),/$(file)))
ALL += $(foreach file,$(NTWR),$(addprefix $(NTWRPATH),/$(file)))

ifeq ($(OS), Windows_NT)
	NTWR += winsock.o
	ALL += $(addprefix $(NTWRPATH), /winsock.o)
else
	UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	NTWR += unixsock.o
	all += $(addprefix $(NTWRPATH), /unixsock.o)
endif
endif

all: $(ALL)
	@:		

%.o:
	@$(CC) -c $(subst .o,.cpp,$@) $(CFLAGS)

	
