#include <iostream>
#include <string>
#include "defs.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <vector>

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
    cout << "WORKER assigned " << trait_line << endl;
    
    for (int i = 0 ; i < USERS_NUM ; i++)
    {
        string file_name = "users-" + to_string(i) + ".csv";
        
        int fd[2];
        if (pipe(fd) == -1)
            error("ERROR: Worker can't create pipe()");
        
        int pid = fork();
        if(pid == 0) //child process
        {
            char outbuf[LINE_LEN];
            close(fd[WRITE]);
            read(fd[READ], outbuf, LINE_LEN);
            string msg = string(outbuf);
            close(fd[READ]);

            int len = users_path.size();
            char path[len + 1];
            for (int i = 0 ; i < len ; i++) path[i] = users_path[i];
            path[len] = '\0';

            char users_id[2] = {i + '0', '\0'}; 

            char* args[] = {"./min_euclidean.out", outbuf, path, users_id, NULL}; 
            execv("./min_euclidean.out", args); //example: ./min_euclidan 1,2,3,4,5 /inputFiles 2   (go read users-2.csv and compare each line to 1,2,3,4,5)
            
            
        }
        else //parent process
        {
            close(fd[READ]);
            write(fd[WRITE], trait_line.c_str(), LINE_LEN);
            close(fd[WRITE]);

            vector<string> candidates;

            int fifo_id = open(MYFIFO, O_RDONLY);
            char inbuf[ANS_LEN + 1];
            read(fifo_id, inbuf, ANS_LEN);
            inbuf[ANS_LEN] = '\0';
            string out = string(inbuf);
            cout << "WORKER received " << out << endl;
            candidates.push_back(out.substr(6, 9)); //only the 5 traits with commas
            close(fifo_id);
            wait(NULL);
            
        }
    }
    
    return 0;
}