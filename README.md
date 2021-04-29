This is the second computer assignment of the Operating Systems course in Spring 2021 at the University of Tehran. 

This is a simple load balancer distributing the computation of a program by assigning tasks to new processes created by ```fork()``` asking the child process to run another program using ```execv()```. Both **named pipes** and **unnamed pipes** are used for interprocess communication.


## How to run:

    
    $ make

    $ ./main-process.out inputFiles/ inputFiles/

## Summary of the program functionality:
There are some input files called "traits.csv" and "users-num.csv". Each containing a line consisting of 5 numbers representing a person's [5 main personality traits](https://en.wikipedia.org/wiki/Big_Five_personality_traits). A MAIN process assigns a new process for each of the lines in "traits.csv". The i'th child of the main-process (WORKER i), assigns a new process for each "user-num.csv". The task of this process (j'th child of WORKER i) is to find the person in "user-j.csv" having the minimum euclidean distance to the i'th person in "traits.csv". 


## Sample Test:
    
    $ ./main-process.out inputFiles/ inputFiles/
    MAIN writing 0,0,0,0,0 for WORKER
    WORKER assigned 0,0,0,0,0

    MIN_EUCLIDEAN writing users-0,0,0,0,0,0
    WORKER received users-0,0,0,0,0,0
    MIN_EUCLIDEAN writing users-1,1,1,1,1,1
    WORKER received users-1,1,1,1,1,1
    MIN_EUCLIDEAN writing users-2,2,2,2,2,2
    WORKER received users-2,2,2,2,2,2

    WORKER writing users-0,0,0,0,0,0 (min ans) for MAIN process 
    MAIN received users-0,0,0,0,0,0 from worker 


    MAIN writing 9,9,9,9,9 for WORKER
    WORKER assigned 9,9,9,9,9

    MIN_EUCLIDEAN writing users-0,6,6,6,6,6
    WORKER received users-0,6,6,6,6,6
    MIN_EUCLIDEAN writing users-1,7,7,7,7,7
    WORKER received users-1,7,7,7,7,7
    MIN_EUCLIDEAN writing users-2,8,8,8,8,8
    WORKER received users-2,8,8,8,8,8

    WORKER writing users-2,8,8,8,8,8 (min ans) for MAIN process 
    MAIN received users-2,8,8,8,8,8 from worker 


    MAIN writing 1,2,3,4,5 for WORKER
    WORKER assigned 1,2,3,4,5

    MIN_EUCLIDEAN writing users-0,3,3,3,3,3
    WORKER received users-0,3,3,3,3,3
    MIN_EUCLIDEAN writing users-1,4,4,4,4,4
    WORKER received users-1,4,4,4,4,4
    MIN_EUCLIDEAN writing users-2,2,2,2,2,2
    WORKER received users-2,2,2,2,2,2

    WORKER writing users-1,4,4,4,4,4 (min ans) for MAIN process 
    MAIN received users-1,4,4,4,4,4 from worker 
    
I am a newbie in writing README's, so sorry if you don't find this beautiful enough. 

Please email me if you have any questions.