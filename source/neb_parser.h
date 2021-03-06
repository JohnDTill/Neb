#ifndef NEB_PARSER_H
#define NEB_PARSER_H

#include "neb_node.h"
#include "neb_token.h"
#include <array>

namespace Neb {

class Scanner;

class Parser{
public:
    QString err_msg = "";
    int err_start;
    int err_end;

private:
    const QString& source;
    Scanner& scanner;
    Token current;
    Token previous;
    bool parsing_dimensions = false;

public:
    Parser(const QString& source);
    ~Parser();
    Node* parseStatement(TokenType surrounding_terminator = EndOfFile, bool nested = false);
    std::vector<Node*> parseAll();
    bool ok() const {return err_msg.isEmpty();}

private:
    //Parser helpers
    Node* createNodeFromPrevToken(const NodeType& type);
    Node* createNodeFromPrevToken(const NodeType& type, Node* child);
    Node* createNodeFromPrevToken(const NodeType& type, Node* lhs, Node* rhs);
    template<int a> Node* createNode(const NodeType& type, const std::array<Node*,a>& args);
    Node* createNodeFromPrevToken(uint integer);
    void scanToRecoveryPoint();
    Node* error(const QString& message);
    Node* error(const QString& message, const Token& t);
    void advance();
    void checkGap();
    void consume(TokenType type, const QString& message);
    template<int n> void consume(const std::array<TokenType,n>& types, const QString& message);
    bool match(TokenType type);
    template<int n> bool match(const std::array<TokenType,n>& types);
    bool peek(TokenType type) const;
    template<int n> bool peek(const std::array<TokenType,n>& types) const;

    //Statements
    Node* printStatement();
    Node* whileStatement(TokenType surrounding_terminator);
    Node* ifStatement(TokenType surrounding_terminator);
    Node* blockStatement(bool nested);
    Node* returnStatement(TokenType surrounding_terminator);
    Node* algorithm(TokenType surrounding_terminator);
    Node* mathStatement();
    Node* equality(Node* n);
    Node* less(Node* n);
    Node* greater(Node* n);
    Node* functionDefinition(Node* n);

    //Expression descent
    typedef Node* (Parser::*Rule)();
    Node* expression();
    template<Rule next> Node* disjunction();
    template<Rule next> Node* conjunction();
    Node* addition();
    Node* multiplication();
    Node* implicitMultiplication();
    Node* leftUnary();
    Node* rightUnary();
    Node* exponent();
    Node* script();
    Node* primary();

    //Boolean descent
    Node* boolExpression();
    Node* boolEquality();

    //Specialized rules
    Node* integer();
    Node* decimal();
    Node* string();
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
    Node* braceMatrix(Node* n, uint col_count, TokenType row_delimiter);
    Node* doubleBraceStart();
    Node* setStart();
    Node* innerProduct();
    Node* integral(const NodeType& type);
    Node* cardinalNumber(NodeType type);

    //MathBran
    Node* mathBranUnary(const NodeType& t);
    Node* mathBranBinary(const NodeType& t);
    Node* mathBranRoot();
    Node* mathBranIntegral(const NodeType& type);
    Node* mathBranCases();
    Node* mathBranBigOperator(const NodeType& t);
    Node* mathBranMatrix();
    Node* mathBranFraction();
    Node* mathBranDerivative(const NodeType& type, const TokenType& deriv_token);
    Node* mathBranUnderscriptedFunction(const NodeType& t);
    Node* mathBranLimit();
    Node* mathBranSuperscript(Node* body, bool consume_on_start = true);
    Node* mathBranExponentOp(Node* body, NodeType op);
    Node* mathBranSubscript(Node* body, bool consume_on_start = true);
    Node* slice();
    Node* mathBranDualscript(Node* body);
    Node* mathBranAccent(const NodeType& t);
    Node* mathBranAccentArrow();
};

}

#endif // PARSER_H
