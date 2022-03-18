#include "BalanceTree.h"
#include "Index.cpp"
#include "Node.cpp"
#include "Tree.cpp"

int main(){
    BalanceTree<int> B(0);
    for(int i = 1;i<=10;i++){
        B.insert_data(new int(i), new Index(i));
    }
    //B.print_tree();
    return 0;
}