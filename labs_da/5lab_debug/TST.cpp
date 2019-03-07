#include "TST.h"
/*
    ??? - try to answer later
    Crash after push_back 2nd text

*/


TSTNode::TSTNode(){
    begin = 0;
    length = 0;
    slink = nullptr;
    is_s1 = false;
    is_s2 = false;
}

TSTNode::TSTNode(size_t beg = 0, size_t len = 0, bool s1= false, bool s2 = false) {
    begin = beg;
    length = len;
    slink = nullptr;

    is_s1 = s1;
    is_s2 = s2;
    
}


TSTree::TSTree(std::string &s1, std::string &s2) {
    root = new TSTNode(0, 0, false, false);
    root->slink = root;
    text1 = s1 + symb1;
    text2 = s2 + symb2;
    textsize1 = text1.size();

    active_node = root;
    active_length = 0;
    active_char_pos = 0;
    how_much_left = 0;

    suffix_link_change = nullptr;
    
    for (size_t i = 0; i < textsize1; i++) {
        //std::cout << std::endl <<  "putting: !" << text1[i] << "!\n";
        PushBack1(i);
        //std::cout << "---------------------------" << std::endl;
        //TreePrint();
    }

    textsize2 = text2.size();

    active_node = root;
    active_length = 0;
    active_char_pos = 0;
    how_much_left = 0;
    suffix_link_change = nullptr;
    

    for(size_t i = 0; i < textsize2; i++) {
        //std::cout << std::endl <<  "adding: !" << text2[i] << "!\n";
        PushBack2(i);
        //std::cout << "---------------------------" << std::endl;
        //TreePrint();
    }

    //Append();
}


void TSTree::PushBack1(int pos_ch) {
    how_much_left++;
    char new_ch = text1[pos_ch]; // char to be pushed
   
    suffix_link_change = root; // обнуляем, т.к. рассмотрение ведется только при активной длине != 0 

    while(how_much_left) {
        if (active_length == 0) {
            active_char_pos = pos_ch;
        }
        std::map<char, TSTNode* >::iterator pos = active_node->next_nodes.find(text1[active_char_pos]);
         
        TSTNode *next = nullptr;

        // we didnt find link to other in node
        if (pos == active_node->next_nodes.end()) {
            TSTNode *leaf = new TSTNode(pos_ch, textsize1 - pos_ch, true,false); 
            active_node->next_nodes[text1[active_char_pos]] = leaf;
            add_link(active_node);

        // we found link, 2 ways    
        } else {
            next = pos->second;

            // if we can go dipper 
            if (active_length >= next->length) {
                active_char_pos += next->length;
                active_length -= next->length;
                active_node = next;
                continue;
            }
            // 3 rule, char (prolonged) nalready in node, have to know nex, false, truet
            // если вставляемая new_ch уже сожержится в ноде ,то мы увеличиваем актив_лен
            if( text1[next->begin + active_length] == new_ch) { 
                ++active_length;
                add_link(active_node);
                break;
            }
            

            TSTNode *split = new TSTNode(next->begin, active_length, true, false);
            TSTNode *leaf = new TSTNode(pos_ch, textsize1-pos_ch, true, false);
            
            active_node->next_nodes[text1[active_char_pos]] = split;
            
            split->next_nodes[new_ch] = leaf;
            next->begin += active_length;
            next->length -= active_length;

            split->next_nodes[text1[next->begin]] = next;
           

            add_link(split);
        }

        // выполняется, если мы не спустились ниже по дереву и не нашли совпадения
        how_much_left--;
        if(active_node == root && active_length != 0) {
            // going to previous suffix
            active_length--;
            active_char_pos = pos_ch - how_much_left + 1; // продвижение каждый раз вперед на 1
            
        } else {
            active_node = (active_node->slink) ? active_node->slink : root;
        }
    }

}

void TSTree::PushBack2(int pos_ch) {
    how_much_left++;
    char new_ch = text2[pos_ch];
    
    suffix_link_change = root;

    while(how_much_left) {
        if (active_length == 0) {
            active_char_pos = pos_ch;
        }
        std::map<char, TSTNode* >::iterator pos = active_node->next_nodes.find(text2[active_char_pos]);
        
        TSTNode *next = nullptr;

        // we didnt find link to other in node
        if (pos == active_node->next_nodes.end()) {
            TSTNode *leaf = new TSTNode(pos_ch, textsize2 - pos_ch, false, true); 
            active_node->next_nodes[text2[active_char_pos]] = leaf;
            add_link(active_node);

        // we found link, 2 ways    
        } else {
            next = pos->second;

            // if we can go dipper
            if (active_length >= next->length) {
                active_char_pos += next->length;
                active_length -= next->length;
                active_node = next;
                continue;
            }
            // 3 rule, char (prolonged) already in node, have to know next
            if (next->is_s1){
                if( text1[next->begin + active_length] == new_ch) { 
                    ++active_length;
                    add_link(active_node);
                    break;
                }
            } else if (next->is_s2){
                if( text2[next->begin + active_length] == new_ch) { 
                    ++active_length;
                    add_link(active_node);
                    break;
                }
            }
            // add if s1, if 21 !!!!
            // we are inside string node
            
            TSTNode *split = new TSTNode(next->begin, active_length,  next->is_s1, !(next->is_s1));
            TSTNode *leaf = new TSTNode(pos_ch, textsize2-pos_ch, false, true);

            
            active_node->next_nodes[text2[active_char_pos]] = split; //??? check this

            split->next_nodes[new_ch] = leaf;
            next->begin += active_length;
            next->length -= active_length;

            if (next->is_s1) {
                split->next_nodes[text1[next->begin]] = next;
            } else {
                split->next_nodes[text2[next->begin]] = next;
            }
            add_link(split);
        }

        how_much_left--;
        if(active_node == root && active_length != 0) {
            // going to previous suffix
            active_length--;
            active_char_pos = pos_ch - how_much_left + 1; 
            
        } else {
            active_node = (active_node->slink) ? active_node->slink : root;
        }
    }

}

void TSTree::add_link(TSTNode *node) {
    if (suffix_link_change != root) {
        suffix_link_change->slink = node;
    }
    suffix_link_change = node;
}

/*
std::string TSTree::LCP(){
    size_t common_length = 0;

    while(text1[common_length] == text2[common_length]) common_length++;
    std::string str = "";
    for(size_t i = 0; i < common_length; i++) {
        str += text1[i];
    }
    return str;
}*/
/*
void TSTree::Append() {
    // first part - finding longest common prefix
    
    size_t text1_pos = 0;
    size_t active_pos = 0;
    bool found = false;

    std::string lcp_string = LCP();
    size_t lcp_pos = 0;

    // going to lcp path
    while (!found) {
        TSTNode *next = nullptr;
        auto pos = active_node->next_nodes.find(lcp_string[lcp_pos]);

        if (pos != active_node->next_nodes.end()) {
            next = pos->second;
            text1_pos = next->begin;

            while (text1_pos < next->begin + next->length && lcp_pos < lcp_string.size()) {

                if (text1[text1_pos] != lcp_string[lcp_pos]) {
                    TSTNode *split = new TSTNode(next->begin, active_pos, active_node, true, false); // lcp_pos -> active pos
                    active_node->next_nodes[text1[next->begin]] = split;

                    //split->next_nodes[new_ch] = leaf;
                    next->begin += active_pos;
                    next->length -= active_pos;
                    split->next_nodes[text1[next->begin]] = next;
                    //add_link(split); // ????
                    active_node = split;
                    found = true; // ???
                    break;
                }
                text1_pos++;
                lcp_pos++;
                active_pos++;
            }

            if (lcp_pos == lcp_string.size()) {
                TSTNode *split = new TSTNode(next->begin, lcp_pos, active_node, true, false); // lcp_pos -> active pos
                active_node->next_nodes[text1[split->begin]] = split;

                //split->next_nodes[new_ch] = leaf;
                next->begin += lcp_pos;
                next->length -= lcp_pos;
                split->next_nodes[text1[next->begin]] = next;
                //add_link(split); // ????
                active_node = split;
                found = true;
            }

            if (!found) {
                active_node = next;
            }

        // we didnt find char in node
        } else {
            found = true; // begin not in tree
        }
        
    }

    // cycle to find and prepate sufftree to pushingback 2nd text
    // we find longest common prefix and push in its new node
    */
   /*
    suffix_link_change = active_node;
    active_node->slink = active_node;

    std::cout << "-------------------" << std::endl;
    TreePrint();

    size_t text2_pos = lcp_pos;

    for(;text2_pos < text2.size(); text2_pos++) {
        std::cout << std::endl <<  "adding: !" << text2[text2_pos] << "!\n";
        PushBack(active_node, text2_pos, false, true);
        std::cout << "---------------------------" << std::endl;
        TreePrint();
    }

}
*/

void TSTree::NodePrint(TSTNode *node, int dpth) {
    std::cout << std::endl << dpth;
    for (int i = 0; i < dpth; ++i) {
        std::cout <<" ";
        std::cout <<" ";
        std::cout <<" ";
        std::cout <<" ";
    }

    std::cout << "node represents: ";
    if (node->is_s1){
        for (size_t str = node->begin; str != node->begin + node->length; ++str) {
            std::cout << text1[str];
        }
       
    }

    if (node->is_s2 && !node->is_s1){
        for (size_t str = node->begin; str != node->begin + node->length; ++str) {
            std::cout << text2[str];
        }   
        
    }

    if (node->is_s1) std::cout << "(1)"; 
    if (node->is_s2) std::cout << "(2)";
    //std::cout << std::endl; 

    //std:: cout << " node is " << node << " sufflink is " << node->slink << std::endl;
    //std:: cout << " node is " << node << " sufflink is " << node->slink << std::endl;


    for (std::map<char, TSTNode *>::iterator it = node->next_nodes.begin(); it != node->next_nodes.end(); ++it) {
        NodePrint(it->second, dpth + 1);
    }
    std::cout.flush();
}

void TSTree::TreePrint()
{
    NodePrint(root, 0);
    std::cout << std::endl;
    std::cout.flush();
}

int TSTree::FindLCS(){
    std::string str = "";
    FindLCS_Node(root, 0);
    //print results
    std::cout << out.first << std::endl;
    /*std::set<std::string>::iterator i;
    for( auto i : out.second) {
        std::cout << i << std::endl;
    }*/
    return 0;
}

std::string TSTree::GetStr(){
    std::string str = "";
    for (auto i = deq.begin(); i != deq.end(); i++){
        if ((*i)->is_s1)
            for (auto j = (*i)->begin; j < (*i)->begin + (*i)->length; j++)
                str+=text1[j];
        if ((*i)->is_s2)
            for (auto j = (*i)->begin; j < (*i)->begin + (*i)->length; j++)
                str+=text2[j];
    }

    return str;
}

// 0 - for both, 1 - for first , 2- second, 5 - undefined
int TSTree::FindLCS_Node(TSTNode *node, size_t depth) { // ??? &str

    deq.push_back(node);
    
    int got = 5;
    int to_ret = 5;
    depth = depth + node->length;

    // to_ret never 5 after this moment
    // if (to_ret == 0) {} // do nothing bcs we already have deeper 
/*
    if (node->is_s1) 
            for(size_t i = node->begin; i < node->begin + node->length; i++)
                str += text1[i];
    if (node->is_s2) 
            for(size_t i = node->begin; i < node->begin + node->length; i++)
                str += text2[i];
  */  
    //std::cout << "im in node " << str << std::endl;

    for (std::map<char, TSTNode *>::iterator it = node->next_nodes.begin(); it != node->next_nodes.end(); ++it) {
        got = FindLCS_Node(it->second, depth); // never got 5

        if (got == 0) to_ret = 0;

        if (got == 1) {
            if (to_ret == 5) to_ret = 1;
            if (to_ret == 2) to_ret = 0;
            // if toret == 1 - nothing
            // if toret == 0 - nothing
        } 
        if (got == 2){
            if (to_ret == 1) to_ret = 0;
            if (to_ret == 5) to_ret = 2;
        }

    }

    //std::cout << "ret is " << to_ret << std::endl;
    if (to_ret == 5){
        if (node->is_s1) to_ret = 1;
        if (node->is_s2) to_ret = 2;
    }


    if (to_ret == 0) {
       // std::cout << "GOT" << std::endl;
    
        if (depth > out.first) {
            out.first = depth;
            out.second.clear();
            out.second.insert(GetStr());
        } 
        if (depth == out.first) {
            out.second.insert(GetStr());
        }
    } 
    
    deq.pop_back();

    return to_ret;
    
}