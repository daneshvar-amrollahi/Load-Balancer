This is the second computer assignment of the Operating Systems course in Spring 2021 at the University of Tehran. 

This is a simple load balancer distributing the computation of a program by assigning tasks to new processes created by ```fork()``` asking the child process to run another program using ```execv()```. Both **named pipes** and **unnamed pipes** are used for interprocess communication.


## How to run:

    
    $ make

    $ ./main-process.out inputFiles/ inputFiles/

## Summary of the program functionality:
There are some input files called "traits.csv" and "users-num.csv". Each containing a line consisting of 5 numbers representing a person's [5 main personality traits](https://en.wikipedia.org/wiki/Big_Five_personality_traits). A MAIN process assigns a new process for each of the lines in "traits.csv". The i'th child of the main-process (WORKER i), assigns a new process for each "user-num.csv". The task of this process (j'th child of WORKER i) is to find the person in "user-j.csv" having the minimum euclidean distance to the i'th person in "traits.csv". 


I am a newbie in writing README's, so sorry if you don't find this beautiful enough. 

Please email me if you have any questions.