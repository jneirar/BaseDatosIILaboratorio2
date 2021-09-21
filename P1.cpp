#include "sequential.h"

int main(){
    cout << "---------------Borrando archivos---------------\n";
    init();
    Sequential seq("data", "aux");
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


    return 0;
}