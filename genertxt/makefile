holdalldir = ../holdall/
hashtabledir = ../hashtable/
keylistdir = ../keylist/
valuelistdir = ../valuelist/

CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic -I$(hashtabledir) -I$(holdalldir) -I$(keylistdir) -I$(valuelistdir)
LDFLAGS =
VPATH = $(holdalldir) $(hashtabledir) $(keylistdir) $(valuelistdir)
objects = main.o $(holdalldir)holdall.o $(hashtabledir)hashtable.o $(keylistdir)keylist.o $(valuelistdir)valuelist.o genertxt.o
executable = genertxt

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)

$(holdalldir)holdall.o: holdall.c holdall.h
$(hashtabledir)hashtable.o: hashtable.c hashtable.h
$(keylistdir)keylist.o: keylist.c keylist.h
$(valuelistdir)valuelist.o: valuelist.c valuelist.h
genertxt.o: genertxt.c genertxt.h holdall.h hashtable.h keylist.h valuelist.h
main.o: main.c holdall.h hashtable.h keylist.h valuelist.h genertxt.h
