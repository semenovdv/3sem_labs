#ifndef TST_H
#define TST_H

#include <iostream>
#include <map>
#include <set>
#include <memory>
#include <string>
#include <algorithm>
#include <deque>


class TSTNode {
public:    
    // next nodes
    std::map<char, TSTNode*> next_nodes;

    size_t begin;
    size_t length;

    bool is_s1;
    bool is_s2;
    

    TSTNode *slink;

    TSTNode();
    TSTNode(const size_t beg, const size_t len, bool s1, bool s2);

    ~TSTNode();
};

class TSTree {
public:
    TSTree(std::string& s);
    TSTree(std::string & s1, std::string &s2);
    void PushBack1(int ch_pos); // adding char into sufftree on-line 
    void PushBack2(int pos_ch);
    bool CanGoDown(int pos_ch, TSTNode *next);
    void add_link(TSTNode *node); 
    int LEN(TSTNode *node, size_t pos); // idk what ???
    // FIND
    void Append();
    int FindLCS_Node(TSTNode *node, size_t depth);
    int FindLCS();
    std::string GetStr();

    void NodePrint(TSTNode *node, int dpth);
    void TreePrint();
    virtual ~TSTree() {};
    std::string LCP();

private:

    std::string text1;
    std::string text2;

    long long textsize1;
    long long textsize2;
    const char symb1 = '$';
    const char symb2 = '#';
    int how_much_left;

    //size_t global_end; // [1:e] to make O(n)

    TSTNode *root;
    //long long remaining;
    
    TSTNode *active_node; // конкретная нода, в который мы находимся в данный момент
    
    TSTNode *suffix_link_change;

    long long active_length; // отвечает за то, сколько суффиксов нужно дабавить до текущего
    size_t active_char_pos; // активная позиция добавляемого в даный момент, а не рассматриваемого
    

    std::pair<size_t, std::set<std::string>> out;
    std::deque<TSTNode *> deq;    
};


#endif // TST_H