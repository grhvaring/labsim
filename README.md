# Numerical Simulation Laboratory 2021

This repository contains my worked out solutions to the exercises assigned during a lab on numerical simulation I attended in the summer semester of 2021.

Each lab session consisted of solving a number of exercises, and then report the results obtained.  Generally I divided my workflow in two parts:
- `C++` programs for the most computationally expensive work
- `Python 3` jupyter notebooks for data visualization and remarks on the results

What follows is an overview on how the repository is organized and how the different parts depend on one another.

## Visualization

The jupyter notebooks are located in the folder `notebooks/` and contain the presentation of each problem, followed by the strategy I employed to solve it and a visual representation of the results I obtained.

The naming is straightforward: the notebook corresponding to the exercises of the 1st lab session is called `lab_01.ipynb`, the one corresponding to the 2nd lab session `lab_02.ipynb`, and so on. 

Although the notebooks can be previewed directly from GitHub, I included in them some interactive features and a couple of aesthetic changes which are not available in the default _static_ rendering of the notebooks provided by GitHub. In other words, I warmly suggest to clone the repository and open the notebooks locally for a better experience.

In the notebooks I do not go into the details of the `C++` code I developed for solving the problems, nonetheless, all the source files are present in the repository, organized by lab session. The next section goes into the details of how to access, compile and execute the source code.

## Source code

For each exercise I put together a `C++` project to complete the "heavy lifting" part, and for most of them I used an object-oriented approach, and one of the best-practices with OOP is to divide the project in different source files, which are then linked at compile time. I did exactly that, following this general structure:
- main source file (`ex_x.y.cpp`, where `x` = \# lab session and`y` = \# exercise), located in `lab_x/src/`
- header (`.h`) files for functions/classes declarations, located in `include/` (possibly used by more than one exercise)
- source (`.cpp`) files for functions/classes implementations, located in `src/`
- `Makefile`, located in each `lab_x` directory (different labs can have different `Makefile`s).

ADD A WORD ON THE FILES SEED.IN AND PRIMES

**Remark:** **The** implementations of **functions/classes** used in the exercises **are not in the same directory as** the source code of **the exercises**, this way they're _globally_ accessible to all projects. What **needs** to be present in each lab directory is the `Makefile`, which contains all the _specific_ dependencies for the projects in that lab.

The executables and object files are not present in the repository, due to possible incompatibilities with the user's machine, but they can be obtained with the procedure explained right below.

The compilation of each lab folder is straightforward: just write the command `make` from a Terminal located in the folder where the `Makefile` is and press `Enter`. The command for running the executables is just as simple: `make run`.

That's it, enjoy!