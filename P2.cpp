#include "isam.h"

int main(){
    init();
    ISAM isam("data", "index");
    Record r;
    cout << "\n--------------------Agrega 2 y 3--------------------\n";
    r.setData(2);
    isam.add(r);
    r.setData(3);
    isam.add(r);
    isam.printIndex1();
    isam.showDataPage(0);

    cout << "\n--------------------Agrega hasta tener 2 linked dataPages--------------------\n";
    r.setData(20);
    isam.add(r);
    r.setData(5);
    isam.add(r);
    r.setData(6);
    isam.add(r);
    r.setData(4);
    isam.add(r);
    r.setData(7);
    isam.add(r);
    r.setData(8);
    isam.add(r);
    r.setData(9);
    isam.add(r);
    r.setData(15);
    isam.add(r);
    isam.printIndex1();
    isam.showDataPage(0);
    isam.showDataPage(1);

    cout << "\n--------------------Agrega 18 y provoco refactoring--------------------\n";
    r.setData(18);
    isam.add(r);
    isam.printIndex1();
    isam.showDataPage(0);
    isam.showDataPage(1);
    isam.showDataPage(2);
    isam.showDataPage(3);
    
    cout << "\n--------------------Agrega valores para buscar luego--------------------\n";
    r.setData(10);
    isam.add(r);
    r.setData(12);
    isam.add(r);
    r.setData(14);
    isam.add(r);
    
    cout << "\n--------------------Busco 4, 10, 17--------------------\n";
    auto rec = isam.search(4);
    if(rec) (*rec).showData(1); cout << "\n";
    rec = isam.search(10);
    if(rec) (*rec).showData(1);
    rec = isam.search(17);
    if(rec) (*rec).showData(1);
    else cout << "No se encontró 17\n";
    
    cout << "\n--------------------Busco por rango de 2 a 6--------------------\n";
    auto recs = isam.search(2, 6);
    for(auto r : recs)
      r.showData(1);

    cout << "\n--------------------Busco por rango de 10 a 15--------------------\n";
    recs = isam.search(10, 15);
    for(auto r : recs)
      r.showData(1);

    return 0;
}

