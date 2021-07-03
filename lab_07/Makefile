CXX = g++
CXXFLAGS = -Wall -O3 --std=c++17 -I ../include
executables := $(basename $(notdir $(wildcard src/ex*)))
libs := $(notdir $(wildcard ../include/*))

all : $(executables)
.PHONY : all
$(executables) : % : %.o random.o particle.o experiment.o canonicalMC.o moldyn.o libsim.o progress.o
	@echo Linking objects to produce executable $@
	@$(CXX) $^ -o $@ $(CXXFLAGS)
%.o : %.cpp $(libs)
	@echo Compiling $@
	@$(CXX) -c $< $(CXXFLAGS)
vpath %.h ../include
vpath %.cpp src:../src
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
	@echo Executing...
	@for x in $(executables); do \
		echo $$x; \
		./$$x; \
	done
