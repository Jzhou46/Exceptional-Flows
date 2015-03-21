all: shell monte

shell:
	gcc shell.c -o shell 
	
monte:
	gcc monteCarlo.c -o monte

clean: 
	 rm -rf *o all

