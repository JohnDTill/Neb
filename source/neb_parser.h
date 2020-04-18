#ifndef NEB_PARSER_H
#define NEB_PARSER_H

#include "neb_node.h"
#include "neb_scanner.h"
#include <array>
#include <stack>

#include <QDebug>

namespace Neb {

class Parser{
public:
    QString err_msg;

private:
    const QString& source;
    Scanner scanner;
    Token current;
    Token previous;
    bool parsing_dimensions = false;
    std::stack<Node*> dangling_nodes;

public:
    Parser(const QString& source);
    Node* parseStatement();

private:
    //Parser helpers
    Node* createNode(const NodeType& type);
    Node* createNode(const NodeType& type, Node* child);
    Node* createNode(const NodeType& type, Node* lhs, Node* rhs);
    template<int a> Node* createNode(const NodeType& type, const std::array<Node*,a>& args);
    void scanToRecoveryPoint();
    [[noreturn]] void errorAt(Token* token, const QString& message);
    [[noreturn]] void error(const QString& message);
    [[noreturn]] void errorAtCurrent(const QString& message);
    void advance();
    void checkGap();
    void consume(TokenType type, const QString& message);
    template<int n> void consume(const std::array<TokenType,n>& types, const QString& message);
    bool match(TokenType type);
    template<int n> bool match(const std::array<TokenType,n>& types);
    bool peek(TokenType type) const;
    template<int n> bool peek(const std::array<TokenType,n>& types) const;

    //Statements
    Node* statementBody();
    Node* equality(Node* n);
    Node* less(Node* n);
    Node* greater(Node* n);

    //Expression descent
    Node* expression();
    Node* conjunction();
    Node* disjunction();
    Node* addition();
    Node* multiplication();
    Node* implicitMultiplication();
    Node* leftUnary();
    Node* rightUnary();
    Node* exponent();
    Node* script();
    Node* primary();

    //Specialized rules
    Node* grouping(const NodeType& t, const TokenType& close);
    Node* idOnly();
    Node* idStart(bool id_only = false);
    Node* call(Node* id);
    template <NodeType t>
        Node* setLiteral();
    Node* setLiteralMathBranScript(const NodeType& t);
    template<bool check_gap = true>
        Node* unicodeSuperscriptExpression();
    Node* unicodeSuperscriptImplicitMult();
    Node* unicodeSuperscriptGrouping();
    Node* unicodeSuperscriptTerminal();
    template<bool check_gap = true>
        Node* unicodeSubscriptExpression();
    Node* unicodeSubscriptImplicitMult();
    Node* unicodeSubscriptGrouping();
    Node* unicodeSubscriptTerminal();
    Node* nablaStart();
    Node* parenStart();
    Node* braceStart();
    Node* setStart();
    Node* innerProduct();
    Node* integral(const NodeType& type);

    //MathBran
    Node* mathBranUnary(const NodeType& t);
    Node* mathBranBinary(const NodeType& t);
    Node* mathBranIntegral(const NodeType& type);
    Node* mathBranCases();
    Node* mathBranBigOperator(const NodeType& t);
    Node* mathBranMatrix();
    Node* mathBranFraction();
    Node* mathBranDerivative(const NodeType& type, const TokenType& deriv_token);
    Node* mathBranUnderscriptedFunction(const NodeType& t);
    Node* mathBranLimit();
    Node* mathBranSuperscript(Node* body, bool consume_on_start = true);
    Node* mathBranSubscript(Node* body, bool consume_on_start = true);
    Node* mathBranDualscript(Node* body);
    Node* mathBranAccent(const NodeType& t);
    Node* mathBranAccentArrow();
};

}

#endif // PARSER_H