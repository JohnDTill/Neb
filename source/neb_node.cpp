#include "neb_node.h"

#include <QTextStream>

namespace Neb {

NEB_DECLARE_COARSETYPE_INIT

Node::Node(NodeType type) : type(type), coarse_type(initCoarseType(type)){
    NEB_DECLARE_SET_INIT
}

Node::Node(NodeType type, QString data) : type(type), coarse_type(initCoarseType(type)), data(data) {
    NEB_DECLARE_SET_INIT
}

NEB_DECLARE_NODE_LABELS

NEB_DECLARE_COARSETYPE_LABELS

NEB_DECLARE_IS_EXPR

static uint64_t writeDOT(QTextStream& out, const Node& n, uint64_t& curr, bool typed){
    uint64_t id = curr++;

    out << "\tn" << QString::number(id) << "[label=\"" << labels[n.type];
    if(n.type == NUMBER || n.type == IDENTIFIER) out << n.data;
    else if(n.type == STRING) out << "\\\"" << n.data << "\\\"";
    if(typed && isExpr(n.type)) out << "\\n" << coarsetype_labels[n.coarse_type];
    out << '"';
    if(n.type == IDENTIFIER) out << ", style=filled, fillcolor=lightblue";
    else if(n.type == ERROR) out << ", style=filled, fillcolor=red";
    else if(n.children.empty()) out << ", style=filled, fillcolor=orange";

    out << "]\n";

    for(Node* n : n.children){
        uint64_t child_id = writeDOT(out, *n, curr, typed);
        out << "\tn" << QString::number(id) << "->n" << QString::number(child_id) << '\n';
    }

    return id;
}

QString Node::toDOT(bool LR, bool typed) const{
    QString str;
    QTextStream out(&str);

    out << "digraph{\n";
    if(LR) out << "\trankdir=\"LR\"\n";
    uint64_t curr = 0;
    writeDOT(out, *this, curr, typed);
    out << "}";

    return str;
}

QString Node::toDOT(const std::vector<Node*>& nodes, bool LR, bool typed){
    QString str;
    QTextStream out(&str);

    out << "digraph{\n";
    if(LR) out << "\trankdir=\"LR\"\n";
    uint64_t curr = 0;
    if(LR) for(auto it = nodes.rbegin(); it != nodes.rend(); it++) writeDOT(out, **it, curr, typed);
    else for(Node* n : nodes) writeDOT(out, *n, curr, typed);
    out << "}";

    return str;
}

void Node::deletePostorder(Node* n){
    for(Node* child : n->children) deletePostorder(child);
    delete n;
}

}
