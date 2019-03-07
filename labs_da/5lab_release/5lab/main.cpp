#include "TST.h"
#include <iostream>



int main() {
    std::string s1, s2;
    std::cin >> s1 >> s2;

    TSTree tree = TSTree(s1, s2);
    //std::cout << "done" << std::endl;
    tree.FindLCS();

}