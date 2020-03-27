#ifndef PARSER_H
#define PARSER_H

#include "node.h"
#include "scanner.h"
#include <stack>

#include <QDebug>

namespace Neb {

class Parser{
private:
    const QString& source;
    Scanner scanner;
    const std::vector<Token>& tokens;
    std::vector<Token>::size_type token_index = 0;
    std::vector<Node*> statements;
    std::stack<Node*> dangling_nodes;
    QString err_msg;

public:
    Parser(const QString& source);
    std::vector<Node*> parse();
    QString getErrorMessage();
    QString::size_type errorStart();
    QString::size_type errorEnd();

private:
    [[noreturn]] void fatalError(const QString& msg);
    Node* createNode(const NodeType& type);
    Node* createNode(const NodeType& type, Node* child);
    Node* createNode(const NodeType& type, Node* lhs, Node* rhs);

    void consume(const TokenType& t);
    void consume(const std::vector<TokenType>& types);
    bool match(const TokenType& t);
    bool match(const std::vector<TokenType>& types);
    bool peek(const TokenType& t) const;
    bool peek(const std::vector<TokenType>& types) const;

    Node* statement();
    Node* equality(Node* n);
    Node* less(Node* n);
    Node* greater(Node* n);
    Node* expression();
    Node* conjunction();
    Node* disjunction();
    Node* addition();
    Node* multiplication();
    Node* implicitMultiplication();
    Node* leftUnary();
    Node* nablaStart();
    Node* leftUnaryNext();
    Node* rightUnary();
    Node* exponent();
    Node* grouping();
    Node* parenGrouping();
    Node* bracketGrouping();
    Node* setGrouping();
    Node* innerProduct();
    Node* generalGrouping(const NodeType& node_type, const TokenType& close_token);
    Node* callArgs();
    Node* escape();
    Node* escapeAccent(const NodeType& type);
    Node* escapeBigOperator(const NodeType& type);
    Node* escapeBinomial();
    Node* escapeCases();
    Node* escapeFraction();
    Node* fractionDerivative(const NodeType& type, const TokenType& deriv_token);
    Node* escapeIntegral(const NodeType& type);
    Node* integralUnicode(const NodeType& type);
    Node* escapeMatrix();
    uint escapeDimension();
    Node* escapeRoot();
    Node* escapeSubscript();
    Node* escapeSuperscript();
    Node* escapeUnderscriptedWord();
    Node* terminal();
    Node* idStart(const QString& text);
    Node* idOnly();
};

}

#endif // PARSER_H
