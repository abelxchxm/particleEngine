#ifndef NODE_H
#define NODE_H

#include <list>
#include <cstddef>

using std::list;

class Node
{
public:
    Node();
    virtual ~Node();

    void setParent(Node *parent);
    Node *getParent();

    list<Node *> &getChildren();

    static list<Node *> &getNodes();

private:
    static list<Node *> nodes;

protected:
    list<Node *> children;
    Node *parent;
};

#endif
