#ifndef NEB_NODE_H
#define NEB_NODE_H

#include "neb_nodetype.h"
#include <QString>
#include <QTextStream>
#include <vector>

namespace Neb {

struct Node {
    NodeType type;
    QString::size_type start;
    QString::size_type length;
    std::vector<Node*> children;

    Node(){}
    Node(NodeType type, QString::size_type start, QString::size_type length)
        : type(type), start(start), length(length) {}
};

class NodeFunction{
public:
    static QString toDOT(const Node* n, const QString& source);
    static QString toDOT(const std::vector<Node*>& nodes, const QString& source);
    static void deletePostorder(Node* n);
};

}

#endif // NODE_H
