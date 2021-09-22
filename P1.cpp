#include "sequential.h"

int main(){
    cout << "---------------Borrando archivos---------------\n";
    init();
    Sequential seq("data", "daux");
    RecordSeq r;
    vector<RecordSeq> vr;
    r.setData(14);
    vr.push_back(r);
    r.setData(24);
    vr.push_back(r);
    r.setData(85);
    vr.push_back(r);
    r.setData(45);
    vr.push_back(r);
    cout << "\n---------------Insert all---------------\n";
    seq.insertAll(vr);
    seq.showAll();

    cout << "\n---------------Insert 4 records in aux---------------\n";
    cout << "-------Insert 16 22 Between 14 & 24-------\n";
    cout << "-------Insert 60 50 Between 45 & 85-------\n";
    r.setData(16);
    seq.add(r);
    r.setData(22);
    seq.add(r);
    r.setData(60);
    seq.add(r);
    r.setData(50);
    seq.add(r);
    seq.showAll();
    
    cout << "\n---------------Insert 1 record more in aux (max 5)---------------\n";
    cout << "---------------Insert 1 to refactor---------------\n";
    r.setData(1);
    seq.add(r);
    seq.showAll();

    cout << "\n---------------Insert 4 records to aux---------------\n";
    cout << "\n---------------Insert 3, 13, 46, 65---------------\n";
    r.setData(3);
    seq.add(r);
    r.setData(13);
    seq.add(r);
    r.setData(46);
    seq.add(r);
    r.setData(65);
    seq.add(r);
    seq.showAll();


    cout << "\n---------------Search 5---------------\n";
    r = seq.search(5);
    cout << "\n---------------Search 50---------------\n";
    r = seq.search(50);
    r.showData();
    cout << "\n---------------Search 3---------------\n";
    r = seq.search(3);
    r.showData();
    cout << "\n---------------Search 85---------------\n";
    r = seq.search(85);
    r.showData();

    
    cout << "\n---------------Search 3 to 20---------------\n";
    vr = seq.search(3, 20);
    cout << "Result size: " << vr.size() << "\n";
    for(auto rec : vr){
        rec.showData(1);
    }

    cout << "\n---------------Search 40 to 60---------------\n";
    vr = seq.search(40, 60);
    cout << "Result size: " << vr.size() << "\n";
    for(auto rec : vr){
        rec.showData(1);
    }

    cout << "InsertAll: O(n) (El vector tiene n records, como se ordena en memoria principal, la insercion toma O(1) por cada elemento, ya que agrega siempre al final\n";
    cout << "\nadd: O(lgn) (Busqueda binaria para buscar la posicion a insertar, luego insertar en data o aux)\n";
    cout << "\nRefactor: O(n+k) (Toma todos los registros (n en data y k en aux) y los agrega en forma ordenada en data, borra aux)\n";
    cout << "\nSearch: O(lgn + k) (Busqueda binaria y luego busqueda lineal en aux (si tiene)\n";
    cout << "\nSearchRange: O(n+k) (Busqueda en todos los registros en el peor caso (n en data y k en aux))\n";
    return 0;
}