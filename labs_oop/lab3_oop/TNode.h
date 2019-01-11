#ifndef TNODE_H
#define TNODE_H

#include "Figure.h"
#include "Octagon.h"
#include "Triangle.h"
#include "Squarefig.h"
#include <memory>

class TNode {
public:
    TNode(const std::shared_ptr<Figure>& fig);
    friend std::ostream& operator<<(std::ostream& os, const TNode& obj);
    std::shared_ptr<TNode> Push(std::shared_ptr<Figure>& fig , std::shared_ptr<TNode> *tree);
    void Print(std::shared_ptr<TNode> tree, int n);
    std::shared_ptr<Figure> GetFigure();
    std::shared_ptr<TNode> GetLeft();
    void SetLeft(std::shared_ptr<TNode> left);
    std::shared_ptr<TNode> GetRight();
    void SetRight(std::shared_ptr<TNode> right);
    virtual ~TNode();

private:
    std::shared_ptr<Figure> figure; 
    std::shared_ptr<TNode> left;
    std::shared_ptr<TNode> right;
};

#endif // TNODE_H