#-------------------------------------------------------------------------------------------------------
#																									   #
#								Makefile for folderBox demo file 									   #
#																									   #
#-------------------------------------------------------------------------------------------------------

TARGET		    =   demo

CXX				=	g++

CXXFLAGS		=	-Werror -std=c++11
CXXFLAGS       += 	-Wall
CXXFLAGS	   +=   -I../src
#CXXFLAGS		+=  -g

OBJS		    = 	../src/syswatcher.o
OBJS		   += 	./main.o

#-------------------------------------------------------------------------------------------------------
#																									   #
#									        Make rules 									   		   	   #
#																									   #
#-------------------------------------------------------------------------------------------------------

.PHONY: all clean tags 

all:$(OBJS)
	$(CXX) $(shell find ../src -name "*.o") main.o $(LDFLAGS) -o $(TARGET).out

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

tags:
	@rm -rf ./tags
	ctags --exclude="docs"			\
		  --exclude="*.o"			\
		  --exclude="*.a"			\
		  --exclude="*.so"			\
		  --exclude="*.elf"			\
		  --exclude="*.map"			\
		  --exclude="*.lss" -R .

clean:
	@rm -rf $(shell find ./ -name "*.o")
	@rm -rf $(TARGET).*

