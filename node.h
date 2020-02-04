#ifndef NODE_H
#define NODE_H

#include "nodetype.h"
#include <QString>
#include <QTextStream>
#include <vector>

namespace Neb {

struct Node {
    NodeType type;
    QString subtext;
    std::vector<Node*> children;
};

class NodePrinter{
public:
    static QString toDOT(const Node& n);
    static QString toDOT(const std::vector<Node*>& nodes);
};

}

#endif // NODE_H
