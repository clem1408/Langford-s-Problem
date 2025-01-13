Collaborators: elcald & joshua_bouchat

<hr></hr>

Langford's Problem
Here are all the implementations I have developed with elcald to solve Langford's Problem:

Langfordv01: [Sequential] Recursive solution (symmetries are not managed, resulting in double the number of solutions).
Langfordv02: [Sequential] Unoptimized solution.
Langfordv03: [Sequential] Optimized solution.
Langfordv04: [Sequential] Optimized with task pre-initialization.
Langfordv05: [Parallel] Langfordv04 with OpenMP.
Langfordv06: [Sequential] Bit-based version of Langford (fastest sequential version).
Langfordv07: [Parallel] Langfordv04 with MPI.
Langfordv08: [Parallel] A Rust version based on buffers for each step and inline fixed-size loops for improved speed, developed by joshua_bouchat.
(Best time: L(16,2) = 29s)
Langfordv09: [Parallel] Langfordv06 with MPI.
Langfordv10: [Parallel] Langfordv06 with OpenMP.
How to Use These Programs
To use any of the programs except the MPI versions, follow these steps:

Navigate to the desired folder.
Run the shell script.
View the results.
For example, to run Langfordv10:

bash
Copier le code
sh langfordv10.sh
cat result.txt
For MPI programs, specify the number of threads:

For example, to run Langfordv09 with 12 threads:

bash
Copier le code
sh langfordv09.sh 12
cat result.txt
Performance Example
Here's the result for L(19,2) on my computer (i7-12700K):


Acknowledgments
Special thanks to all collaborators on this project for the shared discussions and ideas to improve these implementations.
A heartfelt thanks to Mr. Jaillet, one of my professors at Université de Reims Champagne-Ardenne, for his guidance and support.
