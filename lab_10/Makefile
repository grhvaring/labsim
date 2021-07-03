CXX = g++
CXXFLAGS = -Wall -O3 --std=c++17 -I ../include
libs := $(notdir $(wildcard ../include/*))

all : ex_10.1 ex_10.2
.PHONY : all
ex_10.1 : ex_10.1.o random.o tempering_tsp.o libsim.o progress.o
	@echo Linking objects to produce executable $@
	@$(CXX) $^ -o $@ $(CXXFLAGS)
%.o : %.cpp $(libs)
	@echo Compiling $@
	@$(CXX) -c $< $(CXXFLAGS)

ex_10.2 : ex_10.2.o random.o genetic_tsp.o libsim.o progress.o
	mpicxx $^ -o $@ $(CXXFLAGS)

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
