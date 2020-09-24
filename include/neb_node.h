#ifndef NEB_NODE_H
#define NEB_NODE_H

#include "neb_nodetype.h"
#include <QString>
#include <vector>
class QTextStream;

namespace Neb {

class Node {
public:
    typedef void(DotCallback)(QTextStream& out, const Node& n);
    static void DO_NOTHING(QTextStream&, const Node&){}

    NodeType type;
    CoarseType coarse_type;
    QString data; //Copying all the data is not ideal, but okay in early stage
    #ifdef NDEBUG
    void* hook;
    void* type_info;
    #else
    void* hook = nullptr;
    void* type_info = nullptr;
    #endif
    std::vector<Node*> children;

    Node(NodeType type);
    Node(NodeType type, QString data);
    QString toDOT(bool LR = false, bool typed = true, DotCallback callback = DO_NOTHING) const;
    static QString toDOT(const std::vector<Node*>& nodes, bool LR = false, bool typed = true, DotCallback callback = DO_NOTHING);
    static void deletePostorder(Node* n);
};

}

#endif // NODE_H
