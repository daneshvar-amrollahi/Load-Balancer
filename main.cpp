#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
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
            char outbuf[LINE_LEN];
            close(fd[WRITE]);
            read(fd[READ], outbuf, LINE_LEN);
            close(fd[READ]);

            int len = users_path.size();
            char path[len + 1];
            for (int i = 0 ; i < len ; i++) path[i] = users_path[i];
            path[len] = '\0';

            char* args[] = {"./worker.out", outbuf, path, NULL}; 
            execv("./worker.out", args); //example: ./worker 1,2,3,4,5 (the line given to worker i is 1,2,3,4,5)

        }
        else //parent process
        {
            //give the i'th line of CSV file to child process
            close(fd[READ]);
            //write(fd[WRITE], lines[i].c_str(), strlen(lines[i].c_str()));
            string msg = lines[i];
            //cout << "This is parent. Writing " << msg << endl;
            write(fd[WRITE], msg.c_str(), LINE_LEN);
            wait(NULL);
            close(fd[WRITE]);
            
        }
        
    }

    return lines;
}

int main(int argc, char *argv[])
{
    string traits_path, users_path;
    get_file_paths(argc, argv, traits_path, users_path);
    
    cout << traits_path << " " << users_path << endl;

    vector<string> ans = findClosest(traits_path, users_path);


    
    return 0;
}