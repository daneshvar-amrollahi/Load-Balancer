#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

#define TRAITS_FILENAME "traits.csv"
#define READ 0
#define WRITE 1
#define MSG_LEN 9

void error(const char *msg)
{
    perror(msg);
    exit(0);
}


void get_file_paths(const int &argc, char* argv[], string &traits_path, string &users_path)
{
    if (argc != 3)
    {
        error("ERROR: File paths not provided correctly\n");
        exit(0);
    }

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
/*
vector<vector<int> > getTraits(vector<string> lines)
{
    vector<vector<int> > ans;
    for (auto line: lines)
        ans.push_back(separateByComma(line));
    return ans;
}
*/

vector<string> findClosest(const string& traits_path, const string& users_path)
{
    vector<string> lines = getLines(traits_path, users_path);
    //vector<vector<int> > traits = getTraits(lines);

    //for (int i = 0 ; i < lines.size() ; i++)
    //{
        int fd[2];
        if (pipe(fd) == -1)
            error("ERROR on pipe()");
        

        int pid = fork();
        if (pid < 0)
            error("ERROR on fork()");

        if (pid == 0) //child process
        {
            char buf[MSG_LEN];
            close(fd[WRITE]);
            read(fd[READ], buf, MSG_LEN);
            string msg = string(buf);
            cout << "This is child. Reading " << msg << endl;
            close(fd[READ]);
            //exec()

        }
        else //parent process
        {
            //give the i'th line of CSV file to child process
            close(fd[READ]);
            //write(fd[WRITE], lines[i].c_str(), strlen(lines[i].c_str()));
            string msg = "1,2,3,4,5";
            cout << "This is parent. Writing " << msg << endl;
            write(fd[WRITE], msg.c_str(), MSG_LEN);
            wait(NULL);
            close(fd[WRITE]);
            
        }
        
    //}

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