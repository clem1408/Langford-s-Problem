*Collaborators : [elcald](https://github.com/ElCald) & [joshua_bouchat](https://github.com/JoshuaBOUCHAT)*

# Langford-s-Problem
Here are all the implementations I have developed with [elcald](https://github.com/ElCald) to solve Langford's Problem.

Langfordv01: [Sequential], recursive solution (symmetries are not managed ==> double the number of solutions)
Langfordv02: [Sequential], unoptimized solution. <br>
Langfordv03: [Sequential], optimized solution. <br>
Langfordv04: [Sequential], with task pre-initialize tasks. <br>
langfordv05: [Parallel], Langfordv4 with openmp. <br>
Langfordv06: [Sequential], bit version of Langford (fastest sequential version). <br>
Langfordv07: [Parallel], Langfordv4 with mpi. <br>
Langfordv08: [Parallel], a rust version base on buffers for each step and inline fixed size for loop to go faster by [joshua_bouchat](https://github.com/JoshuaBOUCHAT) (best time : L(16,2)= 29s). <br>
Langfordv09: [Parallel], Langfordv6 with mpi. <br>
Langfordv10: [Parallel], Langfordv6 with openmp. <br>

Here's L(19,2) on my computer (i7 12700k): <br>
![image](https://github.com/user-attachments/assets/3ca94653-4edc-44bc-9313-49adcb1e816a)
<br>

Credits: Thanks to all collaborators of the project, as we discussed together about ways to implement those solutions to improve them, and special thanks to Mr. Jaillet, one of my teachers at Université de Reims Champagne-Ardenne.
