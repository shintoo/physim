# physim
A parametric force application simulator

#synopsis
physim file

#building physim
compile with
gcc -std=c11 main.c -o physim -lm

#description
The purpos of this program is to graph the position of an object with mass over a time interval in which various forces are applied.
It takes input from <file>: mass, time range, number of forces, forces (vector, time applied), and window range.
It outputs a graph to a file.
