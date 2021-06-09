# Numerical Simulation Laboratory 2021

This repository contains my worked out solutions to the exercises assigned during a lab on numerical simulation I attended in the summer semester of 2021.

Each lab session consisted of solving a number of exercises, and then report the results obtained.  Generally I divided my workflow in two parts:
- `C++` programs for the most computationally expensive work
- `python 3` jupyter notebooks for data visualization and remarks on the results

What follows is an overview on how the repository is organized and how the different parts depend on one another.

## Visualization

The jupyter notebooks are located in the folder `notebooks/` and contain the presentation of each problem, followed by the strategy I employed to solve it and a visual representation of the results I obtained.

The naming is straightforward: the notebook corresponding to the exercises of the 1st lab session is called `lab_01.ipynb`, the one corresponding to the 2nd lab session `lab_02.ipynb`, and so on. 

Each notebook has as first command `from libsim import *` which imports the content of a small module I wrote for this lab; right after that, I specify where to draw the data from (the folder `results/lab_x/`) and then I run the command `HTML(custom_css())`, which makes some small aesthetic changes to the notebook. This concludes the initial settings of the notebooks.

The complete `C++` solutions are not explained in detail in the notebooks but, if needed, all the source files are present in the repository, organized by lab session. The next section goes into the details on how to access, compile and execute them.

## Source code

The executables and object files are not present in the repository, due to possible incompatibilities with the user's machine, but they can be obtained with the procedure explained at the end of this section.

For each exercise I put together a `C++` project to complete the "heavy lifting" part, and for most of them I used an object-oriented approach. Considering that one of the best-practices with OOP is to divide the project in different source files, which are then linked at compile time, I separated my code in different source files, following this general structure:
- main source file (`ex_x.y.cpp`, `x` = lab session \#, `y` = exercise \#), located in `lab_x/src/`
- header (`.h`) files for function/class declarations, located in `include/` (possibly used by more than one exercise)
- source (`.cpp`) files for function/class implementations, located in `src/`
- `Makefile`, located in each `lab_x` directory (different labs can have different `Makefile`s).

ADD A WORD ON THE FILES SEED.IN AND PRIMES

**Remark:** **The** implementations of **functions/classes** used in the exercises **are not in the same directory as** the source code of **the exercises**, this way they're _globally_ accessible to all projects. What **needs** to be present in each lab directory is the `Makefile`, which contains all the _specific_ dependencies for the projects in that lab.

The compilation of each lab folder is straightforward: just write the command `make` from a Terminal located in the folder where the `Makefile` is and press `Enter`. The command for running the executables is just as simple: `make run`.

That's it, enjoy!