############################ -*- Mode: Makefile -*- ###########################
## Makefile --- 
###############################################################################


#------------------
# cc = g++
cc = g++

# Fortran compilation options
#-----------------------------
CFLAGS = -std=gnu++11 -I ../lib/eigen/

# Linker options
#---------------
LDFLAGS = `pkg-config --cflags --libs mogal`

LDFLAGSDEBUG = $(LDFLAGS)

# Sources directories
#--------------------
SRCDIR = ../src

# Objects
#------------
OBJ = mingenome.o mingafactoryparams.o utils.o test.o mingafactory.o

all: test #debug

test:  $(OBJ)
	$(cc) -o test $(OBJ) $(LDFLAGS)

debug:	$(OBJ)
	$(cc) -o testdebug $(OBJ) $(LDFLAGSDEBUG)

clean:
	rm -f *.o $(OBJ) test testdebug
	rm -f ../output/dose*
	rm -f ../output/bixelweights*

%.o:    $(SRCDIR)/%.cpp
	$(cc) $(CFLAGS) -c $<



