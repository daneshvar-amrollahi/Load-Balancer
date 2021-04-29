#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "defs.h"

using namespace std;



void error(const char *msg)
{
    perror(msg);
    exit(0);
}


void get_file_paths(const int &argc, char* argv[], string &traits_path, string &users_path)
{
    if (argc != 3)
        error("ERROR: File paths not provided correctly\n");

    traits_path = string(argv[1]) + TRAITS_FILENAME;
    users_path = string(argv[2]);
}

vector<string> getLines(const string& traits_path, const string& users_path)
{
    vector<string> ans;
    ifstream fin;
    fin.open(traits_path);

    string line;
    
    while (getline(fin, line))
        ans.push_back(line);

    fin.close();
    return ans;
}


vector<string> findClosest(const string& traits_path, const string& users_path)
{
    vector<string> lines = getLines(traits_path, users_path);
    vector<string> result;

    for (int i = 0 ; i < lines.size() ; i++)
    {
        int fd[2];
        if (pipe(fd) == -1)
            error("ERROR on pipe()");
        

        int pid = fork();
        if (pid < 0)
            error("ERROR on fork()");

        if (pid == 0) //child process
        {
            close(fd[WRITE]);
            char inbuf[LINE_LEN + 1];
            bzero(inbuf, LINE_LEN + 1);
            read(fd[READ], inbuf, LINE_LEN + 1);
            close(fd[READ]);

            int len = users_path.size();
            char path[len + 1];
            for (int i = 0 ; i < len ; i++) path[i] = users_path[i];
            path[len] = '\0';

            
            char* args[] = {"./worker.out", inbuf, path, NULL}; 
            execv("./worker.out", args); //example: ./worker 1,2,3,4,5 (the line given to worker i is 1,2,3,4,5)

        }
        else //parent process
        {
            //give the i'th line of CSV file to child process
            close(fd[READ]);
            //write(fd[WRITE], lines[i].c_str(), strlen(lines[i].c_str()));
            string msg = lines[i];

            if (mkfifo(FIFO_MAIN, 0777) == -1)
            {
                if (errno != EEXIST)
                    error("ERROR on mkfifo\n");
            }

            cout << "MAIN writing " << msg << " for WORKER\n";

            write(fd[WRITE], msg.c_str(), LINE_LEN + 1);
            
            close(fd[WRITE]);

            
            int fifo_id = open(FIFO_MAIN, O_RDONLY);
            char inbuf[ANS_LEN + 1];
            bzero(inbuf, ANS_LEN + 1);
            read(fifo_id, inbuf, ANS_LEN);
            inbuf[ANS_LEN] = '\0';
            string ans = string(inbuf);
            cout << "MAIN received " << ans << " from worker " << endl << endl << endl;
            result.push_back(ans);   
            
            
            

            wait(NULL);
        }
        
    }

    return result;
}

void outputToFile(const vector<string>& result)
{
    ofstream fout;
    fout.open(OUTPUT_FILENAME);
    for (auto line: result)
        fout << line << endl;
    fout.close();
}

int main(int argc, char *argv[])
{
    string traits_path, users_path;
    get_file_paths(argc, argv, traits_path, users_path);
    


    vector<string> result = findClosest(traits_path, users_path);

    outputToFile(result);
    
    return 0;
}