#include "isam.h"

int main(){
    init();
    ISAM isam("data", "index");
    Record r;
    r.setData(2);
    isam.add(r);
    r.setData(4);
    isam.add(r);
    r.setData(1);
    isam.add(r);

    r.setData(6);
    isam.add(r);
    r.setData(3);
    isam.add(r);
    r.setData(10);
    isam.add(r);
    r.setData(7);
    isam.add(r);


    isam.printIndex1();
    isam.showDataPage(0);
    isam.showDataPage(1);
    isam.showDataPage(2);


    r.setData(12);
    isam.add(r);
    r.setData(9);
    isam.add(r);
    r.setData(10);
    isam.add(r);


    isam.printIndex1();
    isam.showDataPage(0);
    isam.showDataPage(1);
    isam.showDataPage(2);
    isam.showDataPage(3);

/*
    auto rec = isam.search(4);
    if(rec) (*rec).showData();
    rec = isam.search(10);
    if(rec) (*rec).showData();
  */

/*
    cout << "\nCreación de nuevo datapage\n\n";
    r.setData(6, "Luis", "Informatica", 8);
    isam.add(r);
    isam.printIndex1();

    r.setData(7, "Luis2", "Informatica2", 2);
    isam.add(r);
    r.setData(8, "Luis3", "Informatica3", 4);
    isam.add(r);

    r.setData(9, "Name1", "Carrera1", 4);
    isam.add(r);
    isam.printIndex1();
  */  
    return 0;
}

