
CSPATH = ./src
BINPATH = ./bin
LIBPATH = ./lib

AR = ar
ARFLAGS = rvs
CC          = gcc
CLINKER     = gcc
CXX = g++
CXXFLAGS = -O
CCFLAGS = -O
LIB_DIRS =
LIBS = -lm #-lefence
LIB_LIST    =  -lGL -lglut -lGLU -lm -lpthread -lXi -lXmu
LIB_PATH    = -L/usr/X11R6/lib

# the source files 

visual_sources = visual.cc \
		 trackball.cc \
	         result_manip.cc \
	         Graph.cc \
		 GraphPos.cc \
		 GraphExtra.cc \
		 pq_vertex.cc \
		 ListNode.cc \
		 General.cc \
		 name_table.cc \
		 List.cc \
		 Point.cc \
	         vis_interface.cc \

sources_cc = main_mcg.cc \
	     common.cc \

sources_gr =    General.cc \
	        Point.cc \
		Graph.cc \
		GraphPos.cc \
		GraphExtra.cc \
		GraphEval.cc \
		pq_vertex.cc \
		name_table.cc \
		ListNode.cc \
		List.cc

types_sources = CrossMethod.cc \
		RunInfo.cc \
		GenInfo.cc \
		EvalInfo.cc \
		Individual.cc \
		Generation.cc \

evaluation_sources = GenEval.cc	\
	     	     PyCommunicate.cc

operation_sources = GenOper.cc

wiener_sources = wiener.cc \
		 common.cc \
	         Graph.cc \
		 GraphPos.cc \
		 GraphExtra.cc \
	         Point.cc \
		 ListNode.cc \
		 General.cc \
		 name_table.cc \
		 List.cc \
		 pq_vertex.cc \

ga_sources = $(types_sources) $(evaluation_sources) $(operation_sources) 

sources1=$(addprefix src/,$(sources_cc)) 
sources2=$(addprefix src/,$(sources_gr)) 
sources3=$(addprefix src/,$(ga_sources)) 

# objects are the same as the source files with .cc replaced with .o

objects1 = $(addprefix lib/,$(sources_cc:.cc=.o))
objects2 = $(addprefix lib/,$(sources_gr:.cc=.o))
objects  = $(objects1) $(objects2)

objects3   = $(addprefix src/,$(ga_sources)) 
objects_ga = $(objects2) $(objects3)
visual_obj = $(addprefix lib/,$(visual_sources:.cc=.o))
wiener_obj = $(addprefix lib/,$(wiener_sources:.cc=.o))

default: mcg

# make all will run commands regardless of whether there is a file "all"
all:	bin/mcg ga view wiener

mcg:	bin/mcg

bin/mcg:	$(objects) 
	$(CXX) $(OPTFLAGS) -o bin/mcg $(objects) $(LIB_DIRS) $(LIBS)

ga:	bin/gadg

bin/gadg:	$(objects_ga) lib/main.o 
	$(CXX) $(OPTFLAGS) -o bin/gadg lib/main.o -g $(objects_ga) $(LIB_DIRS) $(LIBS)

vis:	bin/vis

bin/vis:	$(visual_obj)
	$(CXX) $(OPTFLAGS) -o bin/vis -g $(visual_obj) $(LIB_PATH) $(LIBS) $(LIB_LIST)

wiener: bin/wiener

bin/wiener : 	$(wiener_obj)
	$(CXX) $(OPTFLAGS) -o bin/wiener -g $(wiener_obj) $(LIB_PATH) $(LIBS) $(LIB_LIST)

# make clean will run commands regardless of whether there is a file "clean"
clean:
	rm -rf lib/*.o
	rm -f bin/mcg
	rm -f bin/vis
	rm -f bin/gadg

lib/%.o : src/%.c
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< $(OUTPUT_OPTION)

lib/%.o : src/%.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDE_DIRS) $(TARGET_ARCH) -c $< -o $@

lib/%.o : src/%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDE_DIRS) $(TARGET_ARCH) -c $< -o $@








