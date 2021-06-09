CXX = g++
CXXFLAGS = -Wall -O3 --std=c++17 -I ../include
executables := $(basename $(notdir $(wildcard src/ex*)))

all : $(executables)
.PHONY : all
$(executables) : % : %.o random.o libsim.o experiment.o lib01.o progress.o
	@echo Linking $^ to produce the executable $@
	@$(CXX) $^ -o $@ $(CXXFLAGS)
%.o : %.cpp random.h libsim.h block.h experiment.h lib01.h random.h
	@echo Compiling $@
	@$(CXX) -c $< $(CXXFLAGS)
vpath %.h ../include
vpath %.cpp ../src:src
.PHONY : cleanres cleanobj cleanexe clean
cleanobj :
	@echo Removing all object files
	@-rm *.o
cleanexe : 
	@echo Removing all executables
	@-rm ex*
cleanout :
	@echo Removing all output files and results
	@-rm *.out ../results/$(notdir $(PWD))/*
clean : cleanobj cleanexe cleanout
run: all
	@echo
	@echo Executing...
	@for x in $(executables); do \
		echo ; \
		echo $$x; \
		./$$x; \
	done
