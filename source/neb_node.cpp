#include "neb_node.h"

#include <QTextStream>

namespace Neb {

static uint64_t writeDOT(QTextStream& out, const Node& n, uint64_t& curr){
    uint64_t id = curr++;

    out << "\tn" << QString::number(id) << "[label=\"" << labels[n.type];
    if(n.type == NUMBER || n.type == IDENTIFIER) out << n.data;
    out << '"';
    if(n.type == IDENTIFIER) out << ", style=filled, fillcolor=lightblue";
    else if(n.type == ERROR) out << ", style=filled, fillcolor=red";
    else if(n.children.empty()) out << ", style=filled, fillcolor=orange";

    out << "]\n";

    for(Node* n : n.children){
        uint64_t child_id = writeDOT(out, *n, curr);
        out << "\tn" << QString::number(id) << "->n" << QString::number(child_id) << '\n';
    }

    return id;
}

QString NodeFunction::toDOT(const Node* n, bool LR){
    Q_ASSERT(n);

    QString str;
    QTextStream out(&str);

    out << "digraph{\n";
    if(LR) out << "\trankdir=\"LR\"\n";
    uint64_t curr = 0;
    writeDOT(out, *n, curr);
    out << "}";

    return str;
}

QString NodeFunction::toDOT(const std::vector<Node*>& nodes, bool LR){
    QString str;
    QTextStream out(&str);

    out << "digraph{\n"
           "\trank1 [style=invis]\n"
           "\trank2 [style=invis]\n"
           "\trank1 -> rank2 [style=invis]\n";
    if(LR) out << "\trankdir=\"LR\"\n";
    uint64_t curr = 0;
    if(LR) for(auto it = nodes.rbegin(); it != nodes.rend(); it++) writeDOT(out, **it, curr);
    else for(Node* n : nodes) writeDOT(out, *n, curr);
    out << "}";

    return str;
}

void NodeFunction::deletePostorder(Node* n){
    for(Node* child : n->children) deletePostorder(child);
    delete n;
}

}
