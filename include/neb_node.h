#ifndef NEB_NODE_H
#define NEB_NODE_H

#include "neb_nodetype.h"
#include <QString>
#include <vector>
class QTextStream;

namespace Neb {

class Node {
public:
    union Data{
        QString* text;
        uint8_t order;
    };

    NodeType type;
    #ifdef NDEBUG
    Data data;
    void* hook;
    #else
    Data data = {nullptr}; //Internal annotation
    void* hook = nullptr; //For any external annotation
    #endif
    CoarseType coarse_type;
    std::vector<Node*> children;

    Node(NodeType type);
    QString toDOT(bool LR = false) const;
    static QString toDOT(const std::vector<Node*>& nodes, bool LR = false);
    static void deletePostorder(Node* n);
};

}

#endif // NODE_H