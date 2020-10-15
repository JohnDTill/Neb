#include "neb_node.h"

#include <QTextStream>

namespace Neb {

Node::Node(NodeType type, int start, int end)
    : type(type), start(start), end(end) {}

Node::~Node(){
    switch (type) {
        case DECIMAL_LITERAL:
        case IDENTIFIER:
        case INTEGER_LITERAL:
        case STRING:
            delete data.text;
        default: return;
    }

    Q_ASSERT(hook == nullptr);
}

NEB_DECLARE_NODE_LABELS

static uint64_t writeDOT(QTextStream& out, const Node& n, uint64_t& curr){
    uint64_t id = curr++;

    out << "\tn" << QString::number(id) << "[label=\"" << labels[n.type];
    switch (n.type){
        case DECIMAL_LITERAL:
        case IDENTIFIER:
        case INTEGER_LITERAL:
            out << *n.data.text;
            break;
        case STRING:
            out << "\\\"" << *n.data.text << "\\\"";
            break;
        case FACTORIAL:
        case TICK_DERIVATIVE:
            out << n.data.number+1;
            break;
        case UINT_PARSED:
            out << (n.data.number & 255) << QString("×") << (n.data.number >> 8);
            break;
        default: break;
    }
    out << '"';
    if(n.type == IDENTIFIER) out << ", style=filled, fillcolor=lightblue";
    else if(n.type == ERROR) out << ", style=filled, fillcolor=red";
    else if(n.children.empty()) out << ", style=filled, fillcolor=orange";

    out << "]\n";

    for(Node* c : n.children){
        uint64_t child_id = writeDOT(out, *c, curr);
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
