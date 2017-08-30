- "times coursework.txt" contains all gathered data used within the report
- Both OMP and Hybrid are precompiled using 4 Threads for teh OMP part
	-if other thread number is to be executed
	 the defined thread number will have to be changed
	 and the program recompiled
-if a visual output is desired, the appropriate sections within the code have to be uncommented and teh program recompiled
	-in addition MPI and hybrid printing works in this setup only with Graphic output, if ASCII is selected a 	communication and blocking has to be coded to print in order of process id

-MPI and hybrid call with the method shown within the lecture slides for MPI
-OMP, MPI and Hybrid can run for any given number of threads and processes(tested for 1-8 for both)
-console execution follows this parameter scheme:
	Possible methods (0-Serial, 1-OMP, 2-MPI, 3-Hybrid, any othwe will trigger default serial function call)
	abc.exe 1 1000 1000	running selected method with given X and Y length
	abc.exe 1 1000		running selected method with given X length and default Y(500)
	abc.exe 1		running selected method with default grid size (500x500)