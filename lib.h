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
#define fopen(n) fstream f; f.open(n, ios::in|ios::out|ios::binary);
#define faopen(n) fstream fa; fa.open(n, ios::in|ios::out|ios::binary);
#define isfopen if(f.is_open())
#define isfaopen if(fa.is_open())

using namespace std;

inline bool exists (const std::string& filename) {
    struct stat buffer;
    return (stat (filename.c_str(), &buffer) == 0);
}

bool isFileEmpty (string fileName){
    fopen(fileName);
    isfopen{
        f.seekg(0, ios::beg);
        auto start = f.tellg();
        f.seekg(0, ios::end);
        auto end = f.tellg();
        return end == start;
        f.close();
    }cout << "No se abrió " << fileName << " en isFIleEmpty\n";
    return false;
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