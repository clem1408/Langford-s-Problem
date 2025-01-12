*Collaborators : [elcald](https://github.com/ElCald) & [joshua_bouchat](https://github.com/JoshuaBOUCHAT)*

# Langford-s-Problem
Here are all the implementations I have developed to solve Langford's Problem.

Langfordv1: [Sequential], unoptimized solution. <br>
Langfordv2: [Sequential], optimized solution. <br>
Langfordv3: [Sequential], with task pre-initialize tasks. <br>
Langfordv4: [Parallel], Langfordv3 with openmp. <br>
langfordv5: [Sequential], bit version of Langford (fastest sequential version). <br>
Langfordv6: [Parallel], Langfordv3 with mpi. <br>
Langfordv7: [Parallel], a rust version base on buffers for each step and inline fixed size for loop to go faster by [joshua_bouchat](https://github.com/JoshuaBOUCHAT) (best time : L(2,16)= 29s). <br>
Langfordv8: [Parallel], Langfordv5 with mpi. <br>
Langfordv9: [Parallel], Langfordv5 with openmp. <br>

Credits: Thanks to all collaborators of the project, as we discussed together about ways to implement those solutions to improve them, and special thanks to Mr. Jaillet, one of my teachers at Université de Reims Champagne-Ardenne.
