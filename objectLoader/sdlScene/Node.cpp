#include "Node.h"

list<Node *> Node::nodes;

Node::Node()
{
    parent = NULL;

    nodes.push_back(this);
}

Node::~Node()
{
    nodes.remove(this);
}

void Node::setParent(Node *p)
{
    if(p == NULL && parent != NULL)
    {
        parent->children.remove(this);
        parent = NULL;
    }
    else if(p != NULL && p->getParent() != this)
    {
        parent = p;

        parent->children.push_back(this);
    }
}

Node *Node::getParent()
{
    return parent;
}

list<Node *> &Node::getChildren()
{
    return children;
}

list<Node *> &Node::getNodes()
{
    return nodes;
}
