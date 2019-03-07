#include "TST.h"
#include <iostream>
#include <ctime>


int main() {
    std::string s1, s2;
    //std::cin >> s1 >> s2;
    s1 = "fjdkndbsvdvhjdvcvsjbchsdvjhcbjshdvjhvjsxbcjshjdcjxbsjjhdvfsdjbf";
    s2 = "dbmsvdjvshdvhvsdbcjsbhjdvjhsbdjcksgdbcrvfvbsjdgjvsjhdbfjsvdjfvjs";
    time_t b1 = clock();
    TSTree tree = TSTree(s1, s2);
    //std::cout << "done" << std::endl;
    tree.FindLCS();
    time_t end = clock();

    std::cout << "Time is: " << end - b1 << std::endl;
}