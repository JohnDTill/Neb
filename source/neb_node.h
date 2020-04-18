#ifndef NEB_NODE_H
#define NEB_NODE_H

#include "neb_nodetype.h"
#include <QString>
#include <QTextStream>
#include <vector>

namespace Neb {

struct Node {
    NodeType type;
    QString data; //Copying all the data is not ideal, but okay in early stage
    std::vector<Node*> children;

    Node(){}
    Node(NodeType type) : type(type) {}
    Node(NodeType type, QString data)
        : type(type), data(data) {}
};

class NodeFunction{
public:
    static QString toDOT(const Node* n);
    static QString toDOT(const std::vector<Node*>& nodes);
    static void deletePostorder(Node* n);
};

}

#endif // NODE_H
