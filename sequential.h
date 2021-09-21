#ifndef SEQ_H
#define SEQ_H

#include "recordSeq.h"

#define fopen(n) fstream f; f.open(n, ios::in|ios::out|ios::binary);
#define faopen(n) fstream fa; fa.open(n, ios::in|ios::out|ios::binary);
#define isfopen if(f.is_open())
#define isfaopen if(fa.is_open())

#define K 5

long sizeFile(string name){
    fopen(name);
    isfopen{
        f.seekg(0, ios::beg);
        auto start = f.tellg();
        f.seekg(0, ios::end);
        auto end = f.tellg();
        f.close();
        return (long)(end - start)/sizeof(RecordSeq);
    }else{
        cout << "No se pudo obtener tamanio del archivo " << name << "\n";
        return 0;
    }
}

bool comp (RecordSeq& a, RecordSeq& b){
    return  a.getKey() < b.getKey();
}

class Sequential{
private:
    string fileName;
    string auxName;
    long sizeData = 0;
    long sizeAux = 0;

    const long sizeRecord = sizeof(RecordSeq);
    
    void refactor(){
        //Sacar todos los registros y guardar ordenados
        fopen(fileName);
        isfopen{
            faopen(auxName);
            isfaopen{
                vector<RecordSeq> vtmp;
                RecordSeq tmp;
                f.seekg(0, ios::beg);
                read(f, tmp);
                vtmp.push_back(tmp);
                long next = tmp.getNext();
                char file = tmp.getFile();
                
                while(next != -1){
                    if(file == 'd'){
                        f.seekg(next, ios::beg);
                        read(f, tmp);
                    }else if(file == 'a'){
                        fa.seekg(next, ios::beg);
                        read(fa, tmp);
                    }else{ cout << "Algó falló\n"; f.close(); fa.close(); return;}
                    vtmp.push_back(tmp);
                    next = tmp.getNext();
                    file = tmp.getFile();
                }
                fa.close();
                fa.open(auxName, std::ofstream::out | std::ofstream::trunc);
                fa.close();
                f.close();
                f.open(fileName, std::ofstream::out | std::ofstream::trunc);
                f.close();
                sizeAux = 0;
                sizeData = 0;
                insertAll(vtmp);
            }   else cout << "Error al abrir aux en refact\n";
            f.close();
        }else cout << "Error al abrir data en refact\n";

    }

    long binarySearch(){
        return 0;
    }

    long linealSearch(int key){
        long res = 0;
        fopen(fileName);
        isfopen{
            RecordSeq tmp;
            for(int i=sizeData-1; i>=0; i--){
                f.seekg(sizeRecord * i);
                read(f, tmp);
                if(tmp.getKey() <= key){
                    res = f.tellg();
                    f.close();
                    return sizeRecord * i;
                }  
            }
            f.close();
            return res;
        }else   cout << "Error al abrir data en linealSearch\n";
        return res;
    }
    
public:
    Sequential(string _fileName, string _auxName) {
        this->fileName = _fileName + ".dat";
        this->auxName = _auxName + ".dat";
        //Tamaño en data.dat, tamaño en aux.dat
        sizeData = sizeFile(fileName);
        sizeAux = sizeFile(auxName);
    }
    
    ~Sequential(){
        
    }

    void insertAll(vector<RecordSeq> records){
        sort(records.begin(), records.end(), comp);
        for(RecordSeq record : records){
            add(record);
        }
    }

    void showAll(){
        cout << "SizeData: " << sizeData << "\n";
        cout << "SizeAux: " << sizeAux << "\n";
        fopen(fileName);
        isfopen{
            faopen(auxName);
            isfaopen{
                RecordSeq tmp;
                f.seekg(0, ios::beg);
                read(f, tmp);
                long next = tmp.getNext();
                char file = tmp.getFile();
                cout << "\n" << "data:" << ":";
                tmp.showData(1);
                
                while(next != -1){
                    if(file == 'd'){
                        f.seekg(next, ios::beg);
                        read(f, tmp);
                        cout << "\n" << "data:" << ":";
                    }else if(file == 'a'){
                        fa.seekg(next, ios::beg);
                        read(fa, tmp);
                        cout << "\n" << "aux:" << ":";
                    }else{ cout << "Algó falló\n"; f.close(); fa.close(); return;}
                    tmp.showData(1);
                    next = tmp.getNext();
                    file = tmp.getFile();
                }
                fa.close();
            }   else cout << "Error al abrir aux en showAll\n";
            f.close();
        }else cout << "Error al abrir data en showAll\n";
    }

    void add (RecordSeq &record){
        record.setNext(-1, 'd');
        if(sizeData == 0){
            fopen(fileName);
            isfopen{
                record.setNext(-1, 'd');
                f.seekp(0, ios::beg);
                write(f, record);
                sizeData++;
                f.close();
            }else cout << "Error al abrir archivo data en add, sizeData = 0\n";
        }else{
            //Hay datos, ubicar la posicion
            long pos = linealSearch(record.getKey());
            fopen(fileName);
            isfopen{
                RecordSeq tmp;
                f.seekg(pos);
                read(f, tmp);

                if(pos == 0 && record.getKey() < tmp.getKey()){
                    //Debe estar antes del primero, entonces mando el primero a aux y dejo en su lugar a record
                    //Escribir en región auxiliar si tiene linkeds
                    if(tmp.getNext() == -1){
                        f.seekp(0, ios::end);
                        auto finalPos = f.tellp();
                        record.setNext(finalPos, 'd');
                        f.seekp(pos, ios::beg);
                        write(f, record);
                        f.seekp(finalPos, ios::beg);
                        write(f, tmp);
                    }else{
                        faopen(auxName);
                        isfaopen{
                            fa.seekp(0, ios::end);
                            auto finalPosition = fa.tellp();
                            record.setNext(finalPosition, 'a');
                            write(fa, tmp);
                            
                            f.seekp(pos, ios::beg);
                            write(f, record);

                            sizeAux++;
                            fa.close();
                            if(sizeAux == K){
                                f.close();
                                refactor();
                            }    
                        }else cout << "Error al abrir archivo aux en add record < pos0\n";
                    }
                }else{
                    if(tmp.getNext() == -1){
                        //Es el último, actualiza tmp y escribe rec
                        f.seekp(0, ios::end);
                        auto finalPosition = f.tellp();
                        write(f, record);

                        tmp.setNext(finalPosition, 'd');
                        f.seekp(pos);
                        write(f, tmp);
                        
                        sizeData++;
                    }else{
                        //Escribir en región auxiliar
                        faopen(auxName);
                        isfaopen{
                            fa.seekp(0, ios::end);
                            auto finalPosition = fa.tellp();
                            record.setNext(tmp.getNext(), tmp.getFile());
                            write(fa, record);

                            tmp.setNext(finalPosition, 'a');
                            f.seekp(pos, ios::beg);
                            write(f, tmp);

                            sizeAux++;
                            fa.close();

                            if(sizeAux == K){
                                f.close();
                                refactor();
                            }    
                        }else cout << "Error al abrir archivo aux en add\n";
                    }
                }
                f.close();
            }else cout << "Error al abrir archivo data en add\n";
        }
    }

    RecordSeq search(int key){
        RecordSeq dump;
        long pos = linealSearch(key);
        fopen(fileName);
        isfopen{
            f.seekg(pos, ios::beg);
            read(f, dump);
            if(dump.getKey() == key){
                f.close();
                return dump;
            }
            if(dump.getNext() == -1){
                cout << "No existe el registro\n";
                f.close();
                return dump;
            }
            //Buscar en aux
            faopen(auxName);
            isfaopen{
                long posAux = dump.getNext();
                char posFile = dump.getFile();
                while(posFile == 'a'){
                    fa.seekg(posAux, ios::beg);
                    read(fa, dump);
                    if(dump.getKey() == key){
                        fa.close();
                        f.close();
                        return dump;
                    }
                    posAux = dump.getNext();
                    posFile = dump.getFile();
                }
                fa.close();
            }else cout << "Error al abrir aux en search\n";

            f.close();
        }else cout << "Error al abrir data en search\n";
        cout << "No existe el registro\n";
        return dump;
    }
    vector<RecordSeq> search(int begin, int end){
        vector<RecordSeq> result;
        long pos = linealSearch(begin);
        char file;
        RecordSeq dump;
        fopen(fileName);
        isfopen{
            faopen(auxName);
            isfaopen{
                do{
                    f.seekg(pos, ios::beg);
                    read(f, dump);
                    if(dump.getKey() > end) break;
                    if(dump.getKey() >= begin && dump.getKey() <= end)
                        result.push_back(dump);
                    if(dump.getFile() == 'a'){
                        //Buscar en aux
                        pos = dump.getNext();
                        do{
                            fa.seekg(pos, ios::beg);
                            read(fa, dump);
                            if(dump.getKey() >= begin && dump.getKey() <= end)
                                result.push_back(dump);
                            pos = dump.getNext();
                            file = dump.getFile();
                        }while(file == 'a');
                    }else{
                        pos = dump.getNext();
                        file = dump.getFile();
                    }
                }while(pos != 1);
                fa.close();
            }else cout << "Error al abrir aux en searchRange\n";
            f.close();
        }else cout << "Error al abrir data en searchRange\n";
        sort(result.begin(), result.end(), comp);
        return result;
    }
};
#endif  //SEQ.H