#ifndef LIB_H
#define LIB_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <map>
#include <vector>
#include <cstring>
#include <algorithm>

#include <sys/stat.h>

using namespace std;

inline bool exists (const std::string& filename) {
    struct stat buffer;
    return (stat (filename.c_str(), &buffer) == 0);
}

void init(){
    std::ofstream ofs;
    if(exists("data.dat")){
        ofs.open("data.dat", std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }
    if(exists("index1.dat")){
        ofs.open("index1.dat", std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }
    if(exists("index2.dat")){
        ofs.open("index2.dat", std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }
    if(exists("index3.dat")){
        ofs.open("index3.dat", std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }
    if(exists("aux.dat")){
        ofs.open("aux.dat", std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }
}

template<typename T>
void write(fstream &f, T &t){f.write((char*) &t, sizeof(t));}

template<typename T>
void read(fstream &f, T &t){f.read((char*) &t, sizeof(t));}

#endif  //LIB.H