#-------------------------------------------------------------------------------------------------------
#																									   #
#								Makefile for folderBox source file 									   #
#																									   #
#-------------------------------------------------------------------------------------------------------

TARGET		    =   folderBox
PROJECT			=   lib$(TARGET)

CXX				=	g++

CXXFLAGS		=	-Werror -std=c++11
CXXFLAGS       += 	-Wall
CXXFLAGS	   +=   -I$(CURDIR)/src
#CXXFLAGS		+=  -g

OBJS		    = 	src/folderBox.o

#-------------------------------------------------------------------------------------------------------
#																									   #
#									        Make rules 									   		   	   #
#																									   #
#-------------------------------------------------------------------------------------------------------

.PHONY: all clean tags 

all:$(OBJS)
	ar -rcs $(PROJECT).a $(shell find ./src -name "*.o")
	$(CXX) -fPIC -shared $(CXXFLAGS) $(shell find ./src -name "*.cpp") -o $(PROJECT).so

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

