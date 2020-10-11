#include "neb_node.h"

#include <QTextStream>

namespace Neb {

NEB_DECLARE_COARSETYPE_INIT

Node::Node(NodeType type, int start, int end)
    : type(type), start(start), end(end), coarse_type(initCoarseType(type)){}

NEB_DECLARE_NODE_LABELS

NEB_DECLARE_COARSETYPE_LABELS

NEB_DECLARE_IS_EXPR 

static uint64_t writeDOT(QTextStream& out, const Node& n, uint64_t& curr){
    uint64_t id = curr++;

    out << "\tn" << QString::number(id) << "[label=\"" << labels[n.type];
    if(n.type == IDENTIFIER) out << *n.data.text;
    else if(n.type == NUMBER) out << *n.data.text;
    else if(n.type == STRING) out << "\\\"" << *n.data.text << "\\\"";
    else if((n.type == TICK_DERIVATIVE || n.type == FACTORIAL) && n.data.order)
        out << n.data.order+1;
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

QString Node::toDOT(bool LR) const{
    QString str;
    QTextStream out(&str);

    out << "digraph{\n";
    if(LR) out << "\trankdir=\"LR\"\n";
    uint64_t curr = 0;
    writeDOT(out, *this, curr);
    out << "}";

    return str;
}

QString Node::toDOT(const std::vector<Node*>& nodes, bool LR){
    QString str;
    QTextStream out(&str);

    out << "digraph{\n";
    if(LR) out << "\trankdir=\"LR\"\n";
    uint64_t curr = 0;
    if(LR) for(auto it = nodes.rbegin(); it != nodes.rend(); it++) writeDOT(out, **it, curr);
    else for(Node* n : nodes) writeDOT(out, *n, curr);
    out << "}";

    return str;
}

void Node::deletePostorder(Node* n){
    for(Node* child : n->children) deletePostorder(child);
    delete n;
}

}
