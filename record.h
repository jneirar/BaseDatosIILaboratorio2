#ifndef RECORD_H
#define RECORD_H

#include "lib.h"
class Record{
    int codigo;
    char nombre[20];
    char carrera[15];
    int ciclo;
public:
    void setData() {cout << "Código: ";cin >> codigo;cout << "Nombre: ";cin >> nombre;cout << "Carrera: ";cin >> carrera;cout << "Ciclo: ";cin >> ciclo;}
    void setData(int codigo, string nombre, string carrera, int ciclo){
        this->codigo = codigo;
        strcpy(this->nombre, nombre.c_str());
        strcpy(this->carrera, carrera.c_str());
        this->ciclo = ciclo;
    }
    void setData(int codigo){
        this->codigo = codigo;
        string nombre = "Alumno" + to_string(codigo);
        strcpy(this->nombre, nombre.c_str());
        string carrera = "Carrera" + to_string(codigo);
        strcpy(this->carrera, carrera.c_str());
        this->ciclo = codigo;
    }

    void showData() {
        cout << "\nCodigo: " << codigo;
        cout << "\nNombre: " << nombre;
        cout << "\nCarrera: " << carrera;
        cout << "\nCiclo : " << ciclo << "\n\n";
    }

    int getKey(){
        return codigo;
    }
    Record& operator =(const Record& a){
        this->codigo = a.codigo;
        for(int i = 0; i < 20; i++) this->nombre[i] = a.nombre[i];
        for(int i = 0; i < 15; i++) this->carrera[i] = a.carrera[i];
        this->ciclo = a.ciclo;
        return *this;
    }
};


#endif  //RECORD.H