If all files(shell.c, monteCarlo.c, and makefile) are in the same directory, then you can just enter "make" and it will create the executables "shell" and "monte". To run the simple shell program, you can just enter "./shell". This shell program takes an integer as the second argument to determine the precision. For example, "./shell 3" to run for the third precision. If no second argument are given then the precision will default to 3. If you run the Monte Carlo program alone by enter "./monte" on the original shell, you will be logged off of your VM or your window will close out. If you run the Monte Carlo program with the simple shell program; by entering for example, "./shell" and then "./monte", then the programs will work perfectly. This Monte Carlo takes a float as a second argument to determine how long this program should run before the program ends. For example, "./monte 3", this program will run for 3 seconds and then exit. If no second argument is given, then the program will run until the precision is found. To exit the shell program, you can just enter "exit". To end the Monte Carlo program while still running, you can just hit CTRL + C. 

If the makefile doesn't work then you can just type:
gcc shell.c -o shell
./shell {int}

Now you are in the shell program. Now enter this to run the Monte Carlo program:
gcc monteCarlo.c -o monte
./monte {float}


