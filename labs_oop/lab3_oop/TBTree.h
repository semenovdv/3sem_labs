#ifndef TBTREE_H
#define TBTREE_H 

#include "TNode.h"
#include "Figure.h"
#include <memory>

class TBTree {
public:
    TBTree();

    void Push(std::shared_ptr<Figure> &&fig);
    bool empty();
    friend std::ostream& operator<<(std::ostream& os, const TBTree& tree);

    virtual ~TBTree();

private:
    std::shared_ptr<TNode> root;
};



#endif //TTREE_H