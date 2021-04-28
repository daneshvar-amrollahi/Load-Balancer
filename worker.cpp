#include <iostream>
#include <string>
#include "defs.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    string trait_line = string(argv[1]);
    string users_path = string(argv[2]);
    cout << "This is a worker. The line assigned to me is: " << trait_line << endl;
    
    for (int i = 0 ; i < USERS_NUM ; i++)
    {
        string file_name = "users-" + to_string(i) + ".csv";
        
        int fd[2];
        if (pipe(fd) == -1)
            error("ERROR: Worker can't create pipe()");
        
        int pid = fork();
        if(pid == 0) //child process
        {
            char buf[MSG_LEN];
            close(fd[WRITE]);
            read(fd[READ], buf, MSG_LEN);
            string msg = string(buf);
            //cout << "This is a child of worker. I am assigned to user " << i << endl;
            //cout << "The message I got from my parent is: " << msg << endl;
            close(fd[READ]);

            
            int len = users_path.size();
            char path[len + 1];
            for (int i = 0 ; i < len ; i++) path[i] = users_path[i];
            path[len] = '\0';

            char users_id[2] = {i + '0', '\0'}; 

            char* args[] = {"./min_euclidean.out", buf, path, users_id, NULL}; 
            execv("./min_euclidean.out", args); //example: ./min_euclidan 1,2,3,4,5 /inputFiles 2   (go read users-2.csv and compare each line to 1,2,3,4,5)

        }
        else //parent process
        {
            close(fd[READ]);
            write(fd[WRITE], trait_line.c_str(), MSG_LEN);
            wait(NULL);
            close(fd[WRITE]);
        }
    }
    
    return 0;
}