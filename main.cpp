#include <iostream>
#include <sys/stat.h>

using namespace std;

inline bool exists (const std::string& filename) {
    struct stat buffer;
    return (stat (filename.c_str(), &buffer) == 0);
}

int main(){
    if(exists("index1.dat"))   cout << "Y\n";
    else        cout << "N\n";
    return 0;
}