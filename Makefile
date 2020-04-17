#-------------------------------------------------------------------------------------------------------
#																									   #
#								Makefile for syswatcher source file 									   #
#																									   #
#-------------------------------------------------------------------------------------------------------

TARGET		    =   syswatcher
PROJECT			=   lib$(TARGET)
VERSION			=	0.1

CXX				=	g++

CXXFLAGS		=	-Werror -std=c++11
CXXFLAGS       += 	-Wall
CXXFLAGS	   +=   -I$(CURDIR)/src
CXXFLAGS	   +=	-I$(CURDIR)/src/util
#CXXFLAGS		+=  -g

OBJS		    = 	src/syswatcher.o

#-------------------------------------------------------------------------------------------------------
#																									   #
#									        Make rules 									   		   	   #
#																									   #
#-------------------------------------------------------------------------------------------------------

.PHONY: all clean install tags 

all:$(OBJS)
	ar -rcs $(PROJECT).a $(shell find ./src -name "*.o")
	$(CXX) -fPIC -shared $(CXXFLAGS) $(shell find ./src -name "*.cpp") -o $(PROJECT).so.$(VERSION)
	ln -s $(PROJECT).so.$(VERSION) $(PROJECT).so

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

install:
	$(shell mkdir ./install)
	@mv ./$(PROJECT).* ./install
	$(shell cp `find ./ -name "*.hpp"` ./install)

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
	@rm -rf $(TARGET).* $(PROJECT).*
	@rm -rf install

