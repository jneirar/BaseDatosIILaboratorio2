#ifndef RECORDSEQ_H
#define RECORDSEQ_H

#include "lib.h"
class RecordSeq{
    int codigo;
    char nombre[20];
    char carrera[15];
    int ciclo;
    long next = -1;
    char file = 'd';
public:
    void setData() {
        cout << "Código: ";cin >> codigo;
        cout << "Nombre: ";cin >> nombre;
        cout << "Carrera: ";cin >> carrera;
        cout << "Ciclo: ";cin >> ciclo;
        this->next = -1;
        char file = 'd';
    }
    void setData(int codigo, string nombre, string carrera, int ciclo){
        this->codigo = codigo;
        strcpy(this->nombre, nombre.c_str());
        strcpy(this->carrera, carrera.c_str());
        this->ciclo = ciclo;
        this->next = -1;
        char file = 'd';
    }
    void setData(int codigo){
        this->codigo = codigo;
        string nombre = "Alumno" + to_string(codigo);
        strcpy(this->nombre, nombre.c_str());
        string carrera = "Carrera" + to_string(codigo);
        strcpy(this->carrera, carrera.c_str());
        this->ciclo = codigo;
        this->next = -1;
        char file = 'd';
    }

    void showData(int i=0) {
        if(i){
            cout << "\nCodigo: " << codigo;
            cout << "\nNext : " << next;
            cout << "\nFile : " << file << "\n\n";
        }else{
            cout << "\nCodigo: " << codigo;
            cout << "\nNombre: " << nombre;
            cout << "\nCarrera: " << carrera;
            cout << "\nCiclo : " << ciclo;
            cout << "\nNext : " << next;
            cout << "\nFile : " << file << "\n\n";
        }
    }
    void setNext(long _next, char _file){
        next = _next;
        file = _file;
    }
    long getNext(){
        return next;
    }
    char getFile(){
        return file;
    }
    int getKey(){
        return codigo;
    }
    RecordSeq& operator =(const RecordSeq& a){
        this->codigo = a.codigo;
        for(int i = 0; i < 20; i++) this->nombre[i] = a.nombre[i];
        for(int i = 0; i < 15; i++) this->carrera[i] = a.carrera[i];
        this->ciclo = a.ciclo;
        this->next = a.next;
        this->file = a.file;
        return *this;
    }
};


#endif  //RECORDSEQ.H