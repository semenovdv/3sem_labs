#include "TRBTree.h"

using std::cout;
using std::cin;
using std::string;
using std::endl;




// поворот текущего влево, т.е. со свои правы сыном
// становимся левым сыном своего правого сына
void TRBTree::RotateLeft(Node *node) {

    Node *right_child = node->right; 
    node->right = right_child->left;

    if (right_child->left != nil)
        right_child->left->parent = node;
    
    if (right_child != nil) right_child->parent = node->parent;

    if (node->parent != nil) {
        if (node == node->parent->left)
            node->parent->left = right_child;
        else 
            node->parent->right = right_child;
    } else {
        root = right_child;
    }

    right_child->left = node;
    if (node != nil) node->parent = right_child;
    
}


// поворот текущего вправо, т.е. со своим левым сыном
// становимся правым сыном нашего левого сына
void TRBTree::RotateRight(Node *node) {

    Node *left_child = node->left;
    node->left = left_child->right;

    if (left_child->right != nil)
        left_child->right->parent = node;

    if (left_child != nil) left_child->parent = node->parent;

    if (node->parent != nil) {
        if (node == node->parent->right) 
            node->parent->right = left_child;
        else 
            node->parent->left = left_child;
    } else {
        root = left_child;
    }

    left_child->right = node;
    if (node != nil) node->parent = left_child;
    
}

void TRBTree::Insert_kv(char* key, unsigned long long value) {
    int i = 0;
    while( key[i] != '\0') {
        key[i] = tolower(key[i]);
        i++;
    }

    Node *node;
    try {
        node = new Node(key, value);
    } catch(std::bad_alloc& exc) {
        throw std::bad_exception{};
    }

    Node *p = root; // место вставки
    Node *q = nil; // родитель места

    // вставка как и в бинарном дереве
    while(p != nil){
        if (strcmp(p->m_key, key) == 0) {
            cout << "Exist" << endl;
            return;
        }
        q = p;
        if (strcmp(p->m_key, key) > 0) {
            p = p->left;
        } else {
            p = p->right;
        }
    } 

    node->left = nil;
    node->right = nil;
    node->parent = q;
    node->color = RED;

    if (q != nil) {
        if (strcmp(key, q->m_key) < 0) {
            q->left = node;
        } else {
            q->right = node;
        }
    } else {
        root = node;
    }

    FixInsertRBTree(node);
    cout << "OK" << endl;

    return;
}

void TRBTree::FixInsertRBTree(Node *node) {
    
    while (node != root 
        && node->parent->color == RED) 
    {
    
        
        // наш отец - левый сын
        if (node->parent == node->parent->parent->left) {
            // если дядя тоже красный, просто мняем цвета
            if (node->parent->parent->right->color == RED) { 
                node->parent->parent->color =  RED;
                node->parent->parent->right->color =  BLACK;
                node->parent->parent->left->color =  BLACK; 
                node = node->parent->parent; // переходим к дяде, типо рекурсия
            } else {
                // если мы правый сын - делаем левый поворот отца
                // мы становимся отцом, наш отец правым сыном

                // мы правый сын 
                if (node == node->parent->right){ 
                    node = node->parent;
                    RotateLeft(node);
                } 
                // мы левый сын
                node->parent->color =  BLACK;
                node->parent->parent->color =  RED;
                RotateRight(node->parent->parent); 
            }
        } else {
            // наш отец - правый сын
            
            // если дядя тоже красный
            if (node->parent->parent->left->color == RED) {
                node->parent->parent->color = RED;
                node->parent->parent->right->color = BLACK;
                node->parent->parent->left->color = BLACK; 
                node = node->parent->parent;
            } else {
                if (node == node->parent->left){ 
                    node = node->parent;
                    RotateRight(node); 
                } 
                // мы левый сын
                node->parent->color =  BLACK;
                node->parent->parent->color =  RED;
                RotateLeft(node->parent->parent);
            }
        }
    }

    this->root->color =  BLACK;
}

void TRBTree::Delete_k(char* key) {
    int i = 0;
    while( key[i] != '\0') {
        key[i] = tolower(key[i]);
        i++;
    }
    bool found = false;
    Node *node = Find(key, &found);
    if(found){
        DeleteHidden(node);
        cout << "OK" << endl;
    }
    //FixDeleteRBTree(node);    
}

Node *TRBTree::Find( char* key, bool *found) {
    Node *node  = this->root;
    
    // ПОИСК ЭЛЕМЕНТА
    if (node == nil) {
        *found = false;
        cout << "NoSuchWord" << endl;  
        return nil;
    }
    
    while (node != nil) {
        // ищем нужный элемент
        if (strcmp(key, node->m_key) < 0) {
            node = node->left;
        } 
        if (strcmp(key, node->m_key) > 0) {
            node = node->right;
        } 
        if (node != nil && (strcmp(key, node->m_key) == 0)) {
            *found = true;
            return node;
        }
    }
    
    cout << "NoSuchWord" << endl;
    *found = false;
    return nil;

    //return node;
}

unsigned long long TRBTree::Find_k(char* key, bool *done) {
    int i = 0;
    while( key[i] != '\0') {
        key[i] = tolower(key[i]);
        i++;
    }
    Node *node  = this->root;
    
    // ПОИСК ЭЛЕМЕНТА
    if (node == nil) {
        cout << "NoSuchWord" << endl;  
        *done = false;
        return 0;
    }
    
    while ( strcmp(node->m_key, key) != 0) {
        // ищем нужный элемент
        if (node == nil) {
            break;
        }

        if (strcmp(key, node->m_key) < 0) {
            node = node->left;
        }else 
        if (strcmp(key, node->m_key) > 0) {
            node = node->right;
        } 
        
    }

    if (strcmp(node->m_key, key) == 0) {
        *done = true;
        return node->m_value;
    }
    
    cout << "NoSuchWord" << endl;
    *done = false;
    return 0;

    //return node;
}

void TRBTree::DeleteHidden(Node *node) { // and fixup
    if (node == nil) return;
    Node *repl = node; 
    Node *fret = nil;  //TODO 
    Node *ret = nil; 
    bool is_left = false;

    // слева нет ребенка, а справа может быть, а может и нет
    if (node->left == nil || node->right == nil){
        repl = node;
    } else {
        repl = node->right;
        while (repl->left != nil) repl = repl->left;
    }

    if (repl->left != nil) {
        ret = repl->left;
    } else {
        ret = repl->right;
    }

    fret = repl->parent;
    
    if (fret != nil) {
        if (repl == repl->parent->left){
            repl->parent->left = ret;
            is_left = true;
            if(ret != nil) ret->parent = repl->parent;
        }else { 
            repl->parent->right = ret;
            is_left = false;
            if(ret != nil) ret->parent = repl->parent;
        }
    } else {
        root = ret;
        ret->parent = nil;
    }

    if (repl != node){
        strcpy(node->m_key , repl->m_key);
        node->m_value = repl->m_value;
    }
    if (repl->color == BLACK) FixDeleteRBTree(ret, fret, is_left);
    
    delete repl;
}

void TRBTree::FixDeleteRBTree(Node *node, Node *fnode, bool is_left) {
    
    while (node != root && node->color == BLACK ) {
        
        if (is_left) {
            Node *bro = fnode->right;

            if (bro->color == RED) { // Red  - точно не nil
                bro->color =  BLACK;
                fnode->color = RED;
                RotateLeft(fnode);
                bro = fnode->right; 
            }
            if (bro->left->color == BLACK && bro->right->color == BLACK) {
                bro->color =  RED;
                node = fnode;
                fnode = fnode->parent;
                if (node != root && node->color == BLACK) {
                    if ( node == fnode->left){
                        is_left = true;
                    } else {
                        is_left = false;
                    }
                }
                
            } else{
                if (bro->right->color == BLACK) {
                    bro->left->color =  BLACK;
                    bro->color = RED;
                    RotateRight(bro);
                    bro = fnode->right;
                } 
                
                bro->color = fnode->color;
                fnode->color = BLACK;
                bro->right->color = BLACK;
                RotateLeft(fnode);
                node = root;
                fnode = nil;
            } 
        } else {
            Node *bro = fnode->left;

            if (bro->color == RED) {
                bro->color = BLACK;
                fnode->color = RED;
                RotateRight(fnode);
                bro = fnode->left; 
            }

            if (bro->right->color == BLACK && bro->left->color == BLACK) {
                bro->color = RED;
                node = fnode;
                fnode = fnode->parent;
                if (node != root && node->color == BLACK) {
                    if ( node == fnode->left){
                        is_left = true;
                    } else {
                        is_left = false;
                    }
                }
            } else{
                if (bro->left->color == BLACK) {
                    bro->right->color = BLACK;
                    bro->color = RED;
                    RotateLeft(bro);
                    bro = fnode->left;
                }

                bro->color = fnode->color;
                fnode->color = BLACK;
                bro->left->color = BLACK;
                RotateRight(fnode);
                node = root;
                fnode = nil;
            }
        }
    }

  node->color = BLACK;
}

void TRBTree::Print_Tree(Node *ptr, long long n) {
    if (ptr == nil || ptr == nullptr) return;
    Print_Tree(ptr->right, n+1);
    cout << endl;
    for(long long i = 0; i < n; i++){
        cout << "  ";
    }
    cout << ptr->m_key << " " << ptr->m_value << " " << ptr->color << endl;
    Print_Tree(ptr->left, n+1);
}

typedef struct {
	bool left;
    bool right;
    nodeColor color; 
    unsigned long long m_value;
    unsigned int len;
} PrMin;


void TRBTree::Save_Tree(std::ofstream *outfile, Node *node) {
    if (node != nil){
        PrMin *data = (PrMin*)malloc(sizeof(PrMin));
        data->color = node->color;
        data->m_value = node->m_value;
        data->len = strlen(node->m_key);
        data->left = false;
        data->right = false;
        if(node->left != nil) data->left = true;
        if(node->right != nil) data->right = true;
        char *zapis = (char*)malloc(sizeof(char) * (data->len + 1));
        int i = 0;
        for( i = 0; i < data->len; i++){
            zapis[i] = node->m_key[i];
        }
        zapis[i] = '\0'; // valgrind this string and len + 1 upper

        data->len++;
        (*outfile).write((char*)data, sizeof(PrMin));
        (*outfile).write((char*)zapis, sizeof(char) * (data->len));
        free(data);
        free(zapis);

        Save_Tree(outfile, node->left);
        Save_Tree(outfile, node->right);
    }  

}

void TRBTree::Load_Tree(std::ifstream *infile){
    Node *pnode = nil; // previous node
    while(true){

        PrMin *data = (PrMin*)malloc(sizeof(PrMin));
        (*infile).read((char*)data, sizeof(PrMin));
        char *key = nullptr;
        key = (char*)malloc((data->len) * sizeof(char));
        (*infile).read((char*)key, sizeof(char)*(data->len));
        key[data->len] = '\0';

        if (*infile){
            Node *node;
            try {
                node = new Node(data->color, nullptr, nullptr, nullptr, key, data->m_value);
            } catch(std::bad_alloc&) {
                throw std::bad_exception{};
            }

            if(!data->left){
                node->left = nil;
            }
            if(!data->right) {
                node->right = nil;
            }
            node->len_key = data->len;
           

            if (pnode == nil){
                root = node; 
                node->parent = nil;
                pnode = node;
            } else {
                if(pnode->left == nil && pnode->right == nil){
                    pnode = pnode->parent;
                    while(pnode->right != nullptr){
                        pnode = pnode->parent;
                    }
                }
                
                if (pnode->left == nullptr ){
                    pnode->left = node;
                } else {
                    pnode->right = node;
                } 
                node->parent = pnode;

                pnode = node;
            }
        } else {
            break;
        }

         delete data;
    }
    cout << "OK" << endl;
}


void TRBTree::Clear_Tree(Node *node){
    if (node != nil) {
        
        Clear_Tree(node->left);
        node->left = nullptr;
        Clear_Tree(node->right);
        node->right = nullptr;
        if (node != root) {
            if( node == node->parent->left) {
                node->parent->left = nullptr;
            } else {
                node->parent->right = nullptr;
            }
        }
        node->parent = nullptr;
        if (node == root) {
            delete node;
            root = nil;
        } else {
            delete node;
        }
        
    }
}

