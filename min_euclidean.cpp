#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include "defs.h"
#include <errno.h>

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

#define INF 2e9
string getMin(const string& trait_line, const string& file_path)
{
    ifstream fin;
    fin.open(file_path);

    string line;
    int mn = INF;
    string ans;
    vector<int> trait = separateByComma(trait_line);
    while (getline(fin, line))
    {
        int cur_distance = getDistance(trait, separateByComma(line));
        if (cur_distance < mn)
        {
            mn = cur_distance;
            ans = line;
        }
    }

    fin.close(); 

    return ans; 
}

int main(int argc, char *argv[])
{
    string trait_line = string(argv[1]);
    string users_path = string(argv[2]);
    int users_id = stoi(argv[3]);

    string file_path = users_path + "users-" + argv[3] + ".csv";
    string ans = "users-" + string(argv[3]) + "," + getMin(trait_line, file_path);
    cout << "MIN_EUCLIDEAN writing " << ans << endl;

    

    
    if (mkfifo(MYFIFO, 0777) == -1)
    {
        if (errno != EEXIST)
            error("ERROR on mkfifo\n");
    }
    
    int fifo_fd = open(MYFIFO, O_WRONLY);
    //cout << "min_euclidean writing to FIFO\n";
    write(fifo_fd, ans.c_str(), ANS_LEN);
    //cout << "min_euclidean wrriten to FIFO\n";
    close(fifo_fd);
    
    return 0;
}