#ifndef NEB_NODE_H
#define NEB_NODE_H

#include "neb_nodetype.h"
#include <QString>
#include <vector>

namespace Neb {

class Node {
public:
    NodeType type;
    CourseType course_type;
    QString data; //Copying all the data is not ideal, but okay in early stage
    std::vector<Node*> children;

    Node(NodeType type);
    Node(NodeType type, QString data);
    QString toDOT(bool LR = false) const;
    static QString toDOT(const std::vector<Node*>& nodes, bool LR = false);
    static void deletePostorder(Node* n);
};

}

#endif // NODE_H
