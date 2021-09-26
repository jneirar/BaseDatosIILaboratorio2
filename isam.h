#ifndef ISAM_H
#define ISAM_H

#include "lib.h"
#include "record.h"

//Supoción 1: Cada page acepta K registros y M índices
//Supoción 2: Los keys corresponden con la llave primaria, y no se repiten
//Supoción 3: Cada page acepta L datapages linkeados a él
//Supoción 4: Los datapages linkeados tienen registros, pero no están ordenados entre sí

#define K 3 //Cantidad de registros por page
#define M 5 //Cantidad de índices por page
#define L 2 //Cantidad máx de linked datapages por datapage

bool comp (Record& a, Record& b){
    return  a.getKey() < b.getKey();
}

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
    int search_page_idx(int key){
        if(size == 1)   return 0;
        for(int i=0; i<size-1; i++){
            if(key < keys[i])   return i;
        }
        return size - 1;
    }
    bool search_final_page(int key){
        if(size == 1)   return true;
        return keys[size-2] < key;
    }
};

class ISAM{
private:
    string fileName;
    string indexName;
    IndexPage index1;
    
    int levels = 1; //Para saber cuántos niveles existen, mínimo hay un nivel, y se llama indexName + 1 + .data
    bool needRefactor = false;
    bool needRefactorIndex = false;
    void refactor(){
        fopen(fileName);
        isfopen{
            long page = 0;
            vector<Record> records;
            DataPage dataPage;
            for(int idxPage = 0; idxPage < index1.size; idxPage++){
                f.seekg(index1.pages[idxPage], ios::beg);
                read(f, dataPage);
                for(int i=0; i<dataPage.size; i++)
                    records.push_back(dataPage.records[i]);
                
                long linkedPage = dataPage.next;
                DataPage linkedDataPage;
                while(linkedPage != -1){
                    f.seekg(linkedPage, ios::beg);
                    read(f, linkedDataPage);
                    for(int i=0; i<linkedDataPage.size; i++)
                        records.push_back(linkedDataPage.records[i]);
                    linkedPage = linkedDataPage.next;
                }
            }
            f.close();
            f.open(fileName, std::ofstream::out | std::ofstream::trunc);
            f.close();
            f.open(indexName + "1.dat", std::ofstream::out | std::ofstream::trunc);
            f.close();
            readIndex();
            
            sort(records.begin(), records.end(), comp);
            index1.size = 0;
            for(Record record : records)
                add(record);
        }else cout << "Error al abrir data en refactor\n";
    }
    void refactorIndex(){
        if(levels == 1){
            //Dividir el index1
            IndexPage index2a;
            IndexPage index2b;
            index2a.insert_page(index1.pages[0]);
            index2b.insert_page(index1.pages[index1.size/2]);
            for(int i=0; i<index1.size-1; i++){
                if(i < index1.size/2)
                    index2a.insert_page(index1.pages[i+1], index1.keys[i]);
                else
                    index2b.insert_page(index1.pages[i+1], index1.keys[i]);
            }
            index1.size = 0;
            index1.insert_page(0);
            fopen(indexName+"2.dat");
            isfopen{
                f.seekp(0, ios::beg);
                write(f, index2a);
                f.seekp(0, ios::end);
                index1.insert_page(f.tellp(), index2b.keys[0]);
                write(f, index2b);
                f.close();
            }else cout << "Error al abrir index 2 en refactor Index\n";
            
            std::ofstream ofs;
            ofs.open("index1.dat", std::ofstream::out | std::ofstream::trunc);
            ofs.close();

            f.open(indexName+"1.dat", ios::in|ios::out|ios::binary);
            isfopen{
                f.seekp(0, ios::beg);
                write(f, index1);
                f.close();
            }else cout << "Error al abrir index1 en refactor Index\n";

        }else{
            //TODO
        }
    }
    long searchIndex(int key){
        if(levels == 1){
            return index1.search_page(key);
        }else if(levels == 2){
            auto pageIndex = index1.search_page(key);
            IndexPage index2;
            fopen(indexName + "2.dat");
            isfopen{
                f.seekp(pageIndex);
                read(f, index2);
                f.close();
                return index2.search_page(key);
            }else cout << "Error al abrir index2.dat\n";
        }else{
            //TODO
        }
        return 0;
    }
    int searchIndexIdx(int key){
        if(levels == 1){
            return index1.search_page_idx(key);
        }else if(levels == 2){
            auto pageIndex = index1.search_page(key);
            IndexPage index2;
            fopen(indexName + "2.dat");
            isfopen{
                f.seekp(pageIndex);
                read(f, index2);
                f.close();
                return index2.search_page_idx(key);
            }else cout << "Error al abrir index2.dat\n";
        }else{
        }
        return 0;
    }
public:
    ISAM(string _fileName, string _indexName) {
        this->fileName = _fileName + ".dat";
        this->indexName = _indexName;
        if(exists(_indexName + "2.dat"))   
            if(!isFileEmpty(_indexName + "2.dat"))
                levels += 1;
        if(exists(_indexName + "3.dat"))   
            if(!isFileEmpty(_indexName + "3.dat"))
                levels += 1;
        cout << "Levels: " << levels << "\n";
        readIndex();
    }
    
    ~ISAM(){
        writeIndex();
    }

    //leer el indice desde disco
    void readIndex(){
        fopen(indexName + "1.dat");
        isfopen{
            read(f, index1);
            f.close();
        }else cout << "Error al abrir index1.dat en readIndex\n";
    }
    
    //Regresa el indice al disco
    void writeIndex(){
        fopen(indexName + "1.dat");
        isfopen{
            write(f, index1);
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
        fopen(fileName);
        isfopen{
            DataPage dataPage;
            f.seekg(index1.pages[pos], ios::beg);
            read(f, dataPage);
            cout << "Size: " << dataPage.size << "\n";
            cout << "Next: " << dataPage.next << "\n";
            cout << "Linked Size: " << dataPage.linked_pages << "\n";
            for(int i=0; i<dataPage.size; i++)
                cout << "Record" << i << ": " << dataPage.records[i].getKey() << "\n";
            cout << "\n";
            if(dataPage.next != -1){
                cout << "\nTiene linked pages\n";
                DataPage linkedDataPage;
                long linkedPage = dataPage.next;
                while(linkedPage != -1){
                    f.seekg(linkedPage, ios::beg);
                    read(f, linkedDataPage);
                    cout << "\tSize: " << linkedDataPage.size << "\n";
                    cout << "\tNext: " << linkedDataPage.next << "\n";
                    for(int i=0; i<linkedDataPage.size; i++)
                        cout << "\tRecord" << i << ": " << linkedDataPage.records[i].getKey() << "\n";
                    cout << "\n";
                    linkedPage = linkedDataPage.next;
                }
            }
            f.close();
        }else cout << "Error al abrir data en showDataPage\n";
    }
    void add(Record &record){
        //Agrega en nivel 1
        if(index1.is_empty()){
            index1.insert_page(0);
            fopen(fileName);
            isfopen{
                DataPage dataPage;
                dataPage.insert_record(record);
                write(f, dataPage);
                f.close();
            }else   cout << "Error al abrir data.dat en add - index1 vacio\n";   
        }else{
            //Tiene datos, insertar en datapage si hay disponible, sino crear un nuevo index
            long page = searchIndex(record.getKey());
            fopen(fileName);
            isfopen{
                DataPage dataPage;
                f.seekg(page, ios::beg);
                read(f, dataPage);
                if(!dataPage.is_full()){
                    //Hay espacio en el dataPage
                    dataPage.insert_record(record);
                    f.seekp(page, ios::beg);
                    write(f, dataPage);
                }else{
                    //Preguntar si el dataPage es el último en index
                    //El último datapage no tiene linked, sino no sería último
                    if(index1.search_final_page(record.getKey())){
                        DataPage newDataPage;
                        //Si record >= último registro de dataPage, agregarlo al final
                        if(record.getKey() >= dataPage.records[dataPage.size - 1].getKey()){
                            newDataPage.insert_record(record);
                        }else{
                            //Mover el mayor registro al nuevo dataPage e insertar record
                            newDataPage.insert_record(dataPage.records[dataPage.size - 1]);
                            dataPage.size--;
                            dataPage.insert_record(record);
                            f.seekp(page, ios::beg);
                            write(f, dataPage);
                        }
                        //Agregarlo al final
                        f.seekp(0, ios::end);
                        long pageNew = f.tellp();
                        write(f, newDataPage);
                        //Agregar al index un nuevo elemento
                        index1.insert_page(pageNew, newDataPage.records[0].getKey());
                        if(index1.is_full())
                            needRefactorIndex = true;
                    }else{
                        //Si tiene linkeds, hay que buscar espacio en ellos, sino 
                        //Crear nuevo datapage y linkearlo
                        long linkedPage = dataPage.next;
                        bool inserted = false;
                        DataPage linkedDataPage;
                        bool flag = false;
                        if(linkedPage == -1)    flag = true;
                        while(linkedPage != -1){
                            f.seekg(linkedPage, ios::beg);
                            read(f, linkedDataPage);
                            if(!linkedDataPage.is_full()){
                                linkedDataPage.insert_record(record);
                                f.seekp(linkedPage, ios::beg);
                                write(f, linkedDataPage);
                                inserted = true;
                                break;
                            }
                            if(linkedDataPage.next == -1)   break;
                            linkedPage = linkedDataPage.next;
                        }
                        if(!inserted){
                            
                            //Insertar nuevo linkedDataPage
                            DataPage newDataPage;
                            newDataPage.insert_record(record);
                            f.seekp(0, ios::end);
                            long newPage = f.tellp();
                            write(f, newDataPage);

                            if(flag){
                                //dataPage no tiene linkeds
                                dataPage.next = newPage;
                            }else{ 
                                linkedDataPage.next = newPage;
                                f.seekp(linkedPage, ios::beg);
                                write(f, linkedDataPage);
                            }
                            dataPage.linked_pages++;
                            f.seekp(page, ios::beg);
                            write(f, dataPage);
                            
                            if(dataPage.linked_pages > L)
                                needRefactor = true;
                        }
                    }
                } 
                f.close();
            }else cout << "Error al abrir data en add\n";
            
        }
        if(needRefactor){
            cout << "\nRefactor\n\n";
            needRefactor = false;
            refactor();
        }
        if(needRefactorIndex){
            cout << "\nRefactor Index\n\n";
            needRefactorIndex = false;
            refactorIndex();
        }
    }
    
    Record* search(int key){
        if(index1.is_empty()){  cout << "No hay datos\n";   return nullptr;}
        long page = searchIndex(key);
        
        DataPage dataPage;
        fopen(fileName);
        isfopen{
            f.seekg(page, ios::beg);
            read(f, dataPage);
            
            Record* result = new Record;
            //Busco la key en el DataPage
            for(int i=0; i<dataPage.size; i++){
                if(dataPage.records[i].getKey() == key){
                    (*result) = dataPage.records[i];
                    f.close();
                    return result;
                }
            }
            cout << "\nNo se encontró en primera. Buscando en linked pages";
            long linkedPage = dataPage.next;
            DataPage linkedDataPage;
            while(linkedPage != -1){
                f.seekg(linkedPage, ios::beg);
                read(f, linkedDataPage);
                for(int i=0; i<linkedDataPage.size; i++){
                    if(linkedDataPage.records[i].getKey() == key){
                        (*result) = linkedDataPage.records[i];
                        f.close();
                        return result;
                    }
                }
                linkedPage = linkedDataPage.next;
            }
            cout << "\nNo existe el registro ni en los pages linkeados\n";
            f.close();
            return nullptr;
        }else   cout << "Error al abrir data.dat en search";
    
        return nullptr;
    }

    vector<Record> search(int start, int end){
        vector<Record> result;
        if(index1.is_empty()){  cout << "No hay datos\n";   return result;}
        long page;
        int idxPage = searchIndexIdx(start);
        
        DataPage dataPage;
        fopen(fileName);
        isfopen{
            while(idxPage != index1.size){
                page = index1.pages[idxPage];
                f.seekg(page, ios::beg);
                read(f, dataPage);
                if(dataPage.records[0].getKey() > end)   break;
                for(int i=0; i<dataPage.size; i++){
                    if(start <= dataPage.records[i].getKey() &&  dataPage.records[i].getKey() <= end)
                        result.push_back(dataPage.records[i]);
                }

                long linkedPage = dataPage.next;
                DataPage linkedDataPage;
                while(linkedPage != -1){
                    f.seekg(linkedPage, ios::beg);
                    read(f, linkedDataPage);
                    for(int i=0; i<linkedDataPage.size; i++){
                        if(start <= linkedDataPage.records[i].getKey() &&  linkedDataPage.records[i].getKey() <= end)
                            result.push_back(linkedDataPage.records[i]);
                    }
                    linkedPage = linkedDataPage.next;
                }
                idxPage++;
            }
            
            f.close();
            sort(result.begin(), result.end(), comp);
            return result;
        }else   cout << "Error al abrir data.dat en search";

        return result;
    }


};
#endif  //ISAM.H
