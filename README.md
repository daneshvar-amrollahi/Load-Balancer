This is the second computer assignment of the Operating Systems course in Spring 2021 at the University of Tehran. 

This is a simple load balancer distributing the computation of a program by assigning tasks to new processes created by ```fork()``` asking the child process to run another program using ```execv()```. Both **named pipes** and **unnamed pipes** are used for interprocess communication.


## How to run:

    
    $ make

    $ ./main-process.out inputFiles/ inputFiles/

## Summary of the program functionality:
There are some input files called "traits.csv" and "users-num.csv". Each containing a line consisting of 5 numbers representing a person's [5 main personality traits](https://en.wikipedia.org/wiki/Big_Five_personality_traits). A MAIN process assigns a new process for each of the lines in "traits.csv". The i'th child of the main-process (WORKER i), assigns a new process for each "user-num.csv". The task of this process (j'th child of WORKER i) is to find the person in "user-j.csv" having the minimum euclidean distance to the i'th person in "traits.csv". 


## Sample Test:
    
    $ ./main-process.out inputFiles/ inputFiles/
    MAIN writing 1,4,3,4,2 for WORKER
    WORKER assigned 1,4,3,4,2

    MIN_EUCLIDEAN writing users-1,2,5,5,5,2
    WORKER received users-1,2,5,5,5,2
    MIN_EUCLIDEAN writing users-2,2,1,2,3,4
    WORKER received users-2,2,1,2,3,4
    MIN_EUCLIDEAN writing users-3,2,3,4,5,3
    WORKER received users-3,2,3,4,5,3
    MIN_EUCLIDEAN writing users-4,4,5,5,2,1
    WORKER received users-4,4,5,5,2,1

    WORKER writing users-3,2,3,4,5,3 (min ans) for MAIN process 
    MAIN received users-3,2,3,4,5,3 from worker 


    MAIN writing 4,2,5,2,3 for WORKER
    WORKER assigned 4,2,5,2,3

    MIN_EUCLIDEAN writing users-1,2,2,2,4,1
    WORKER received users-1,2,2,2,4,1
    MIN_EUCLIDEAN writing users-2,5,5,4,2,2
    WORKER received users-2,5,5,4,2,2
    MIN_EUCLIDEAN writing users-3,2,1,5,1,4
    WORKER received users-3,2,1,5,1,4
    MIN_EUCLIDEAN writing users-4,5,2,5,2,2
    WORKER received users-4,5,2,5,2,2

    WORKER writing users-4,5,2,5,2,2 (min ans) for MAIN process 
    MAIN received users-4,5,2,5,2,2 from worker 


    MAIN writing 1,3,1,3,5 for WORKER
    WORKER assigned 1,3,1,3,5

    MIN_EUCLIDEAN writing users-1,2,2,2,4,1
    WORKER received users-1,2,2,2,4,1
    MIN_EUCLIDEAN writing users-2,2,1,2,3,4
    WORKER received users-2,2,1,2,3,4
    MIN_EUCLIDEAN writing users-3,2,3,4,5,3
    WORKER received users-3,2,3,4,5,3
    MIN_EUCLIDEAN writing users-4,5,1,1,5,3
    WORKER received users-4,5,1,1,5,3

    WORKER writing users-2,2,1,2,3,4 (min ans) for MAIN process 
    MAIN received users-2,2,1,2,3,4 from worker 

I am a newbie in writing README's, so sorry if you don't find this beautiful enough. 

Please email me if you have any questions.