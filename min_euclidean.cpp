#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    string trait_line = string(argv[1]);
    string users_path = string(argv[2]);
    int users_id = stoi(argv[3]);
    cout << "User assigned to me: " << users_id << endl;
    return 0;
}