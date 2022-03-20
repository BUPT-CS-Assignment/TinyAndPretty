#include "BalanceTree.h"
#include "Node.cpp"
#include "Tree.cpp"
#include "Insert.cpp"
#include "Search.cpp"
#include "Delete.cpp"

int main(){
    BalanceTree<int,int> B(0);
    for(int i = 1;i<=20;i++){
        B.insert_data(new int(i), new int(i));
    }
    B.insert_data(new int(12),new int(12));
    for(int i = 1;i<=7;i++){
        B.delete_data(new int(i));
        cout<<*B.getHeader()<<endl;
    }
    B.check_all();
    cout<<*B.search_position(new int(18))<<endl;

    return 0;
}
