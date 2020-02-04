#ifndef PARSER_H
#define PARSER_H

#include "nodetype.h"
#include <QMap>
#include <QString>
#include <QTextStream>

#include "scanner.h"

namespace Neb {

struct Node {
    NodeType type;
    QString subtext;
    std::vector<Node*> children;
};

class Parser{

private:
    const QString& source;
    Scanner scanner;
    const std::vector<Token>& tokens;
    std::vector<Token>::size_type token_index = 0;
    std::vector<Node*> statements;
    QString err_msg;

public:
    Parser(const QString& source);
    std::vector<Node*> parse();
    Token lastExaminedToken();
    QString getErrorMessage();
    static QString toDOT(const Node& n);
    static QString toDOT(const std::vector<Node*>& nodes);

private:
    [[noreturn]] void fatalError(const QString& msg);
    static uint64_t writeDOT(QTextStream& out, const Node& n, uint64_t& curr);
    static Node* createNode(const NodeType& type);
    static Node* createNode(const NodeType& type, Node* child);
    static Node* createNode(const NodeType& type, Node* lhs, Node* rhs);
    static Node* createNode(const NodeType& type, std::vector<Node*> children);

    void consume(const TokenType& t);
    void consume(const std::vector<TokenType>& types);
    bool match(const TokenType& t);
    bool match(const std::vector<TokenType>& types);
    bool peek(const TokenType& t) const;
    bool peek(const std::vector<TokenType>& types) const;
    void skipPastSpecialClose();

    Node* statement();
    Node* expression();
    Node* conjunction();
    Node* disjunction();
    Node* addition();
    Node* multiplication();
    Node* leftUnary();
    Node* rightUnary();
    Node* exponent();
    Node* grouping();
    Node* callArgs();
    Node* escape();
    Node* escapeBinary();
    Node* escapeCases();
    Node* escapeMatrix();
    Node* escapeRoot();
    Node* escapeSuperscript();
    Node* escapeSubscript();
    Node* terminal();
};

}

#endif // PARSER_H
