Collaborators: [elcald](https://github.com/ElCald) & [joshua_bouchat](https://github.com/JoshuaBOUCHAT)

<hr></hr>

# Langford's Problem
Here are all the implementations I have developed with elcald to solve Langford's Problem:

Langfordv01: [Sequential] Recursive solution (symmetries are not managed, resulting in double the number of solutions).<br>
Langfordv02: [Sequential] Unoptimized solution.<br>
Langfordv03: [Sequential] Optimized solution.<br>
Langfordv04: [Sequential] Optimized with task pre-initialization.<br>
Langfordv05: [Parallel] Langfordv04 with OpenMP.<br>
Langfordv06: [Sequential] Bit-based version of Langford (fastest sequential version).<br>
Langfordv07: [Parallel] Langfordv04 with MPI.<br>
Langfordv08: [Parallel] A Rust version based on buffers for each step and inline fixed-size loops for improved speed, developed by [joshua_bouchat](https://github.com/JoshuaBOUCHAT).<br>
Langfordv09: [Parallel] Langfordv06 with MPI.<br>
Langfordv10: [Parallel] Langfordv06 with OpenMP.<br>

<hr>

## How to Use These Programs

To use any of the programs **except the MPI versions**, follow these steps:

- Navigate to the desired folder.
- Run the shell script.
- View the results.
- For example, to run Langfordv10:

```
sh langfordv10.sh
cat result.txt
```

For MPI programs, specify the number of threads:

```
sh langfordv09.sh 12
cat result.txt
```

<hr>

## Performance Example

Here's the result for L(19,2) on my computer (i7-12700K):
![l19](https://github.com/user-attachments/assets/2653137b-9639-4719-b30c-f3fb964c0db4)

<hr>

## Acknowledgments
Special thanks to all collaborators on this project for the shared discussions and ideas to improve these implementations.
A heartfelt thanks to Mr. Jaillet, one of my professors at Université de Reims Champagne-Ardenne, for his guidance and support.
