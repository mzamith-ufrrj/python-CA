################################################################################
#
#
#  Universidade Federal Rural do Rio de Janeiro
#  Instituto Multidisciplinar - Departamento de Ciencia da Computacao
#  por: Marcelo Zamith - zamith.marcelo@gmail.com
#  para compilar com target release use o comando: make target=release + rápido
#
#  versão de on-ramp
#
################################################################################
EXEFILE      = ../TModelCA++.exec
#LIBFILE      = ../libTModelCA++.so
APPLICATION  = -D_APPLICATION=\"$(EXEFILE)\"
#CPUCC       = /opt/intel/bin/icpc
#CPUCC       = g++ #/opt/gnu-gcc/bin/g++
#CPUCC       = /home/mzamith/Apps/gcc/bin/g++
CPUCC       = g++
CPPFLAGS    = --std=c++11 -m64 -Wno-reorder #     -Wunused-variable #-Wno-conversion-null -Wdelete-non-virtual-dtor
DEFS        = $(APPLICATION)  $(VERSION) -DALIGN=64
INCLUDES    =
LIBDIR      =
#LIBDIR      = -L/opt/gnu-gcc/lib64
LIBS     =  -lm -ljsoncpp
LINK     =  $(LIBDIR) $(LIBS)
CPU_COMPILE = $(CPUCC) $(DEFS) $(INCLUDES) $(CPPFLAGS)

ifeq ($(target), release)
  DEFS      +=
  CPPFLAGS  +=      -O2
else
  DEFS     += -D_DEBUG
  CPPFLAGS += -g -Wall -O0
endif

all: exec 
exec:	Grid 		          	\
	 	TModel                  \
	 	CellularAutomata        \
		Statistic               \
        MovementSensor	     	\
		Entity 		            \
		Configure               \
		main

	$(CPU_COMPILE)  obj/Grid.o             \
			        obj/TModel.o           \
			        obj/CellularAutomata.o \
			        obj/Statistic.o   \
			        obj/MovementSensor.o   \
			        obj/Entity.o           \
			        obj/Configure.o        \
			        obj/main.o             \
	$(LINK) -o $(EXEFILE)

main:
	$(CPU_COMPILE) -c main.cpp -o obj/main.o

Configure:
	$(CPU_COMPILE) -c Configure.cpp -o obj/Configure.o

CellularAutomata:
	$(CPU_COMPILE) -c CellularAutomata.cpp -o obj/CellularAutomata.o

TModel:
	$(CPU_COMPILE) -c TModel.cpp -o obj/TModel.o

Grid:
	$(CPU_COMPILE) -c Grid.cpp -o obj/Grid.o

Entity:
	$(CPU_COMPILE) -c Entity.cpp -o obj/Entity.o

MovementSensor:
	$(CPU_COMPILE) -c MovementSensor.cpp -o obj/MovementSensor.o

Statistic:
	$(CPU_COMPILE) -c Statistic.cpp -o obj/Statistic.o

clean:
	rm obj/*.o; rm *.txt; rm *.dat; rm obj/*.o; rm $(EXEFILE)


	
