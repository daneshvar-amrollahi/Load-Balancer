#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>

using namespace std;

#define TRAITS_FILENAME "traits.csv"

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

vector<string> findClosest(const string& traits_path, const string& users_path)
{
    vector<string> ans;

    ifstream fin;
    fin.open(traits_path);

    string line;
    
    fin >> line;
    ans.push_back(line);
    
    fin >> line;
    ans.push_back(line);

    fin.close();

    return ans;
}

int main(int argc, char *argv[])
{
    string traits_path, users_path;
    get_file_paths(argc, argv, traits_path, users_path);
    
    cout << traits_path << " " << users_path << endl;

    vector<string> ans = findClosest(traits_path, users_path);

    cout << ans[0] << "\n" << ans[1] << endl;

    
    return 0;
}