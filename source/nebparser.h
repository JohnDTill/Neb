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
    Node* callArgs();
    Node* escape();
    Node* escapeBinary();
    Node* escapeCases();
    Node* escapeIntegral();
    Node* escapeMatrix();
    Node* escapeRoot();
    Node* escapeSuperscript();
    Node* escapeSubscript();
    Node* escapeUnderscriptedWord();
    Node* terminal();
    Node* idStart();
};

}

#endif // PARSER_H
