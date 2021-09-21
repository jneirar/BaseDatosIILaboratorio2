#ifndef ISAM_H
#define ISAM_H

#include "lib.h"
#include "record.h"

//Supoción 1: Cada page acepta K registros y M índices
//Supoción 2: Los keys corresponden con la llave primaria, y no se repiten

#define K 3 //Cantidad de registros por page
#define M 3 //Cantidad de índices por page
#define L 1 //Cantidad máx de linked datapages por datapage

struct DataPage{
	Record records[K];
    int size = 0;
    long next = -1;
    int linked_pages = 0;
    bool is_empty(){return size == 0;}
    bool is_full(){return size == K;}
    void insert_record(Record &record){
        if(is_full()){cout << "No se puede agregar más al datapage\n"; return;}
        if(is_empty()){records[size++] = record; return;}
        size++;
        for(int i=size-1; i>0; i--){
            if(records[i-1].getKey() < record.getKey()){
                records[i] = record;
                return;
            }else{
                records[i] = records[i-1];
            }
        }
        records[0] = record;
    }
};
struct IndexPage{
	int keys[M];
	long pages[M+1];
    int size = 0;   //size keys and size+1 pages //Size cuenta los pages.
    bool is_empty(){return size == 0;}
    bool is_full(){return size-1 == M;}
    void insert_page(long page, int key = 0){
        if(is_full()){cout << "Index lleno\n"; return;}
        if(size == 0){size = 1; pages[0] = page; return;}
        if(size == 1){size++; keys[0] = key; pages[1] = page; return;}
        //Insertar ordenado si hay espacio
        size++;
        for(int i=size-2; i>0; i--){
            if(keys[i-1] < key){keys[i] = key; pages[i+1] = page; return;}
            else {keys[i] = keys[i-1]; pages[i+1] = pages[i];}
        }
        keys[0] = key; pages[1] = page;
    }
    long search_page(int key){
        if(size == 1)   return pages[0];
        for(int i=0; i<size-1; i++){
            if(key < keys[i])   return pages[i];
        }
        return pages[size - 1];
    }
};

class ISAM{
private:
    string fileName;
    string indexName;
    IndexPage index1;
    
    int levels = 1; //Para saber cuántos niveles existen, mínimo hay un nivel, y se llama indexName + 1 + .data

public:
    ISAM(string _fileName, string _indexName) {
        this->fileName = _fileName + ".dat";
        this->indexName = _indexName;
        if(exists(_indexName + "2.dat"))   levels += 1;
        if(exists(_indexName + "3.dat"))   levels += 1;
        readIndex();
    }
    
    ~ISAM(){
        writeIndex();
    }

    //leer el indice desde disco
    void readIndex(){
        fstream f;
        f.open(indexName + "1.dat", ios::in | ios::binary);
        if(f.is_open()){
            f.read((char*) &index1, sizeof(index1));
            f.close();
        }else   cout << "Error al abrir index1.dat en readIndex\n";
    }
    
    //Regresa el indice al disco
    void writeIndex(){
        fstream f;
        f.open(indexName + "1.dat", ios::out | ios::binary);
        if(f.is_open()){
            f.write((char*) &index1, sizeof(index1));
            f.close();
        }else   cout << "Error al abrir index1.dat en writeIndex\n";
    }

    void printIndex1(){
        auto size = index1.size;
        auto keys = index1.keys;
        auto pages = index1.pages;
        cout << "Size: " << size << "\n";
        if(size > 0){
            cout << "key  : null ";
            for(int i = 0; i < index1.size - 1; i++)
                cout << keys[i] << " ";
            cout << "\npages: ";
            for(int i = 0; i < index1.size; i++)
                cout << pages[i] << " ";
            cout << "\n";
        }cout << "\n";
    }
    void showDataPage(int pos){
        if(pos >= index1.size){cout << "ShowData No válido\n"; return;}
        cout << "Datapage " << pos << ":\n";
        fstream f;
        f.open(fileName, ios::in | ios::binary);
        DataPage dataPage;
        f.seekg(index1.pages[pos], ios::beg);
        f.read((char*) &dataPage, sizeof(dataPage));
        cout << "Size: " << dataPage.size << "\n";
        for(int i=0; i<dataPage.size; i++)
            cout << "Record" << i << ": " << dataPage.records[i].getKey() << "\n";
        cout << "\n";
    }
    void add(Record &record){
        //Agrega en nivel 1
        if(index1.is_empty()){
            index1.insert_page(0);
            fstream f;
            f.open(fileName, ios::out | ios::binary);
            if(f.is_open()){
                DataPage dataPage;
                dataPage.insert_record(record);
                f.write((char*) &dataPage, sizeof(dataPage));
                f.close();
            }else   cout << "Error al abrir data.dat en add - index1 vacio\n";   
        }else{
            //Tiene datos, insertar en datapage si hay disponible, sino crear un nuevo index
            long page = index1.search_page(record.getKey());
            if(levels == 1){
                DataPage dataPage;
                fstream f;
                f.open(fileName, ios::in | ios::out | ios::binary);
                if(f.is_open()){
                    f.seekg(page, ios::beg);
                    f.read((char*) &dataPage, sizeof(dataPage));
                    if(dataPage.is_full()){
                        //dataPage lleno, si tiene linked, buscar insertarlo ahí
                        //Si el linked está lleno, reorganización de dataPages
                        //Si no tiene linked, creamos un linked
                        
                        
                        // si el nuevo record es mayor al último, entonces :
                        //  1. Si tiene linked, buscamos insertarlo ahí
                        //            Si está lleno, hacemos reorganización de archivo. (máximo 1 linked)
                        //  2. Si no tiene linked, 
                        //Sino verificamos si tiene linked:
                        //  Si tiene, 
                        //  Si no tiene, creamos un datapage linkeado a dataPage
                        DataPage dataPageNew;
                        f.seekp(0, ios::end);
                        long pageNew = f.tellp();
                        if(dataPage.records[dataPage.size-1].getKey() < record.getKey()){
                            //Agregar dataPage al final y a index
                            if(index1.is_full()){
                                //Reorganizar levels
                                //reorganizate(record);



                            }else{
                                dataPageNew.insert_record(record);
                                index1.insert_page(pageNew, record.getKey());     
                                f.seekp(0, ios::end);
                                f.write((char*) &dataPageNew, sizeof(dataPageNew));
                            }
                        }else{
                            //Trasladar 1 registro al nuevo para dar espacio al nuevo
                            dataPageNew.insert_record(dataPage.records[dataPage.size-1]);
                            for(int i=dataPage.size-1; i>0; i--){
                                if(dataPage.records[i-1].getKey() < record.getKey()){
                                    dataPage.records[i] = record;
                                    break;
                                }
                                else{
                                    dataPage.records[i] = dataPage.records[i-1];
                                }
                            }
                            dataPage.linked_pages++;
                            dataPage.next = pageNew;
                            f.write((char*) &dataPageNew, sizeof(dataPageNew));
                            f.seekp(page, ios::beg);
                            f.write((char*) &dataPage, sizeof(dataPage));
                        }
                    }else{
                        dataPage.insert_record(record);
                        f.seekp(page, ios::beg);
                        f.write((char*) &dataPage, sizeof(dataPage));
                    }
                    f.close();
                }else   cout << "Error al abrir data.dat en add - 1 level\n";   
            }else{
                //Si hay más de un nivel
            }
            
            
        }
    }
    
    Record* search(int key){
        if(index1.is_empty()){  cout << "No hay datos\n";   return nullptr;}
        long page = index1.search_page(key);
        if(levels == 1){
            DataPage dataPage;
            fstream f;
            f.open(fileName, ios::in | ios::binary);
            if(f.is_open()){
                f.seekg(page, ios::beg);
                f.read((char*) &dataPage, sizeof(dataPage));
                
                Record* result = new Record;
                //Busco la key en el DataPage
                for(int i=0; i<dataPage.size; i++){
                    if(dataPage.records[i].getKey() == key){
                        (*result) = dataPage.records[i];
                        f.close();
                        return result;
                    }
                }
                cout << "No se encontró en primera. Buscando en linked pages\n";
                //Buscar en pages siguientes.
                int linked_pages = dataPage.linked_pages;
                if(linked_pages > 0){
                    for(int p=0; p<linked_pages; p++){
                        f.seekg(dataPage.next, ios::beg);
                        f.read((char*) &dataPage, sizeof(dataPage));
                        //Busco la key en el DataPage linkeado
                        for(int i=0; i<dataPage.size; i++){
                            if(dataPage.records[i].getKey() == key){
                                (*result) = dataPage.records[i];
                                f.close();
                                return result;
                            }
                        }
                    }
                    cout << "No existe el registro ni en los pages linkeados\n";
                }else{
                    cout << "No existe el registro\n";
                }
                f.close();
                return nullptr;
            }else   cout << "Error al abrir data.dat en search level1";
        }else{
            //Si hay más de un nivel

        }
        return nullptr;
    }



    /*
    //Escribe el registro al final del archivo de datos. Se actualiza el indice. 
    void write_record(Record record) {
        if(index.count(record.getKey())){
            cout << "Ya existe la key, no se agrega\n";
        } else{
            ofstream f;
            f.open(fileName + ".dat", ios::binary | ios::app);
            if(f.is_open()){
                f.seekp(0, ios::end);
                long posFisica = f.tellp();
                f.write((char*) &record, sizeof(record));
                f.close();
                index[record.getKey()] = posFisica;
                return;
            }
            cout << "Error al abrir\n";
        }
    }

    //Busca un registro que coincida con la key
    Record* search(string key) {
        Record* result = nullptr;
        if(index.count(key) == 0)   return result;
        long pos = index[key];
        fstream f;
        f.open(fileName + ".dat", ios::in | ios::binary);
        result = new Record;
        if(f.is_open()){
            f.seekg(pos, ios::beg);
            f.read((char*) result, sizeof(*result));
            f.close();
        }
        return result;
    }

    //Muestra todos los registros de acuerdo como fueron insertados en el archivo de datos
    void scanAll() {
        long pos = 0;
        fstream f;
        f.open(fileName + ".dat", ios::in | ios::binary);
        Record r;
        cout << "\nSize: " << index.size() << "\n";
        while(!f.eof()){
            f.seekg(pos, ios::beg);
            f.read((char*) &r, sizeof(r));
            cout << "\nRegistro: " << i+1;
            r.showData();
            pos += sizeof(r);
        }
        f.close();
    }

    
    //Muestra todos los registros de acuerdo a como estan ordenados en el indice
    void scanAllByIndex() {
        long pos;
        fstream f;
        f.open(fileName + ".dat", ios::in | ios::binary);
        Record r;
        cout << "\nSize: " << index.size() << "\n";
        for(auto it = index.begin(); it != index.end(); it++){
            pos = (*it).second;
            f.seekg(pos, ios::beg);
            f.read((char*) &r, sizeof(r));
            cout << "\nRegistro: ";
            r.showData(); 
        }
        f.close();
    }
    */
};
#endif  //ISAM.H
