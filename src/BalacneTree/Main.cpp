#include "BalanceTree.h"
#include "Node.cpp"
#include "Tree.cpp"
#include "Insert.cpp"
#include "Search.cpp"
#include "Delete.cpp"

int main(){
    BalanceTree<int,int> B(0);
    for(int i = 0;i<=50;i++){
        B.insert_data(new int(i), new int(i));
    }
    //B.print_tree();
    return 0;
}
