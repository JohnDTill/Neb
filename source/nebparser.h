#ifndef PARSER_H
#define PARSER_H

#include "node.h"
#include "scanner.h"
#include <stack>

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
    Token lastExaminedToken();
    QString getErrorMessage();

private:
    [[noreturn]] void fatalError(const QString& msg);
    Node* createNode(const NodeType& type);
    Node* createNode(const NodeType& type, Node* child);
    Node* createNode(const NodeType& type, Node* lhs, Node* rhs);
    Node* createNode(const NodeType& type, std::vector<Node*> children);

    void consume(const TokenType& t);
    void consume(const std::vector<TokenType>& types);
    bool match(const TokenType& t);
    bool match(const std::vector<TokenType>& types);
    bool peek(const TokenType& t) const;
    bool peekBehind(const TokenType& t) const;
    bool peek(const std::vector<TokenType>& types) const;
    void skipPastSpecialClose();

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
    Node* rightUnary();
    Node* exponent();
    Node* grouping();
    Node* parenGrouping();
    Node* bracketGrouping();
    Node* setGrouping();
    Node* generalGrouping(const NodeType& node_type, const TokenType& close_token);
    Node* callArgs();
    Node* escape();
    Node* escapeBigOperator(const NodeType& type);
    Node* escapeBinomial();
    Node* escapeCases();
    Node* escapeFraction();
    Node* escapeIntegral(const NodeType& type);
    Node* escapeMatrix();
    uint escapeDimension();
    Node* escapeRoot();
    Node* escapeSuperscript();
    Node* escapeSubscript();
    Node* escapeUnderscriptedWord();
    Node* terminal();
    Node* idStart(const QString& text);
    Node* idOnly();
};

}

#endif // PARSER_H
