#ifndef NEB_NODE_H
#define NEB_NODE_H

#include "neb_nodetype.h"
#include <QString>
#include <vector>

namespace Neb {

struct Node {
    NodeType type;
    QString data; //Copying all the data is not ideal, but okay in early stage
    std::vector<Node*> children;

    Node(){}
    Node(NodeType type) : type(type) {}
    Node(NodeType type, QString data) : type(type), data(data) {}
};

namespace NodeFunction{
    QString toDOT(const Node* n, bool LR = false);
    QString toDOT(const std::vector<Node*>& nodes, bool LR = false);
    void deletePostorder(Node* n);
};

}

#endif // NODE_H
