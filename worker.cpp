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
#include <strings.h>

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

vector<int> separateByComma(string line)
{
    vector<int> ans;

    string cur = "";
    for (auto c: line)
    {
        if (c == ',')
        {
            ans.push_back(stoi(cur));
            cur = "";
        }
        else
            cur += c;
    }
    ans.push_back(stoi(cur));

    return ans;
}

int getDistance(vector<int> first, vector<int> second)
{
    int res = 0;
    for (int i = 0 ; i < first.size() ; i++)
        res += (first[i] - second[i]) * (first[i] - second[i]);
    return res;
}


string getMin(const string& trait_line, const vector<string>& candidates)
{
    vector<int> trait = separateByComma(trait_line);
    int mn = INF;
    string ans;
    for (auto candidate: candidates)
    {
        int current_distance = getDistance(trait, separateByComma(candidate.substr(6, 9)));
        if (current_distance < mn)
        {
            mn = current_distance;
            ans = candidate;
        }
    }
    return ans;
}

int main(int argc, char *argv[])
{
    string trait_line = string(argv[1]);
    string users_path = string(argv[2]);
    cout << "WORKER assigned " << trait_line << endl << endl;
    
    vector<string> candidates;
    for (int i = 0 ; i < USERS_NUM ; i++)
    {
        string file_name = "users-" + to_string(i) + ".csv";
        
        int fd[2];
        if (pipe(fd) == -1)
            error("ERROR: Worker can't create pipe()");
        
        int pid = fork();
        
        if(pid == 0) //child process
        {
            char inbuf[LINE_LEN];
            bzero(inbuf, LINE_LEN);
            close(fd[WRITE]);
            read(fd[READ], inbuf, LINE_LEN);
            string msg = string(inbuf);
            close(fd[READ]);

            int len = users_path.size();
            char path[len + 1];
            for (int i = 0 ; i < len ; i++) path[i] = users_path[i];
            path[len] = '\0';

            char users_id[2] = {i + '0', '\0'}; 

            char* args[] = {"./min_euclidean.out", inbuf, path, users_id, NULL}; 
            execv("./min_euclidean.out", args); //example: ./min_euclidan 1,2,3,4,5 /inputFiles 2   (go read users-2.csv and compare each line to 1,2,3,4,5)
            
            
        }
        else //parent process
        {
            close(fd[READ]);
            if (mkfifo(FIFO_WORKER, 0777) == -1)
            {
                if (errno != EEXIST)
                    error("ERROR on mkfifo\n");
            }
            write(fd[WRITE], trait_line.c_str(), LINE_LEN);
            close(fd[WRITE]);

            
            int fifo_id = open(FIFO_WORKER, O_RDONLY);
            
            char inbuf[ANS_LEN + 1];
            bzero(inbuf, ANS_LEN + 1);
            read(fifo_id, inbuf, ANS_LEN);
            inbuf[ANS_LEN] = '\0';
            string out = string(inbuf);
            cout << "WORKER received " << out << endl;
            candidates.push_back(out); //only the 5 traits with commas
            
            close(fifo_id);
            
            
            wait(NULL);
            
        }
    }   
    
    

    string ans = getMin(trait_line, candidates);

    int fifo_id = open(FIFO_MAIN, O_WRONLY);
    cout << endl << "WORKER writing " << ans << " (min ans) for MAIN process " << endl;
    write(fifo_id, ans.c_str(), ANS_LEN);
    close(fifo_id);
        

    return 0;
}