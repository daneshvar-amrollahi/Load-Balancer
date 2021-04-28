#include <iostream>
#include <string>

#define USERS_NUM 3

using namespace std;

int main(int argc, char *argv[])
{
    string line = string(argv[1]);

    cout << "This is a worker." << endl;
    cout << "Line: " << line << endl;
    cout << endl;
    return 0;
}