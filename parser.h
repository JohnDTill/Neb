#ifndef PARSER_H
#define PARSER_H

#include "nodetype.h"
#include <QMap>
#include <QString>
#include <QTextStream>
#include <vector>

class Parser{
public:
    struct Node {
        NodeType type;
        QString subtext;
        std::vector<Node*> children;
    };

private:
    static constexpr bool identifiers_use_multiple_chars = false;

    enum TokenType {
        ArrowAccent,
        At,
        Backslash,
        Bar,
        BarAccent,
        BigProduct,
        BigSum,
        Binomial,
        Cap,
        Cos,
        Cup,
        Caret,
        Cases,
        Comma,
        Colon,
        Conjunction,
        Dagger,
        DefEquals,
        Disjunction,
        Divide,
        Dollar,
        DotAccent,
        DotAccentDouble,
        DotAccentTriple,
        DotProduct,
        DoubleBar,
        DoubleDagger,
        DoubleExclam,
        Dualscript,
        EmptySet,
        Equals,
        Exclam,
        Exists,
        ForAll,
        Forwardslash,
        Fraction,
        Greater,
        GreaterEqual,
        Hash,
        Hat,
        Identifier,
        In,
        Infinity,
        Integer,
        Integral,
        LeftAngle,
        LeftArrow,
        LeftBrace,
        LeftBracket,
        LeftCeil,
        LeftDoubleAngle,
        LeftFloor,
        LeftParen,
        Less,
        LessEqual,
        MapsTo,
        Matrix,
        Minus,
        Multiply,
        Natural,
        Newline,
        Not,
        NotEqual,
        Number,
        Partial,
        Percent,
        Period,
        PlanckConst,
        Plus,
        Prime,
        Quaternion,
        Rational,
        Real,
        RightAngle,
        RightArrow,
        RightBrace,
        RightBracket,
        RightCeil,
        RightDoubleAngle,
        RightFloor,
        RightParen,
        Root,
        Semicolon,
        Sin,
        SpecialClose,
        SpecialEscape,
        SpecialOpen,
        Subscript,
        Superscript,
        Tick,
        Tilde,
        TildeAccent,
        Times,
        Transpose,
        UnderscriptedWord
    };

    static const QMap<QString, TokenType> keywords;

    struct Token {
        TokenType type;
        QString::size_type start;
        QString::size_type end;
    };

    const QString& source;
    QString::size_type source_index = 0;
    std::vector<Token> tokens;
    std::vector<Token>::size_type token_index = 0;
    std::vector<Node*> statements;

    uint64_t line = 0;

public:
    static std::vector<Node*> parse(const QString& source);
    static QString toDOT(const Node& n);
    static QString toDOT(const std::vector<Node*>& nodes);

private:
    Parser(const QString& source);
    static uint64_t writeDOT(QTextStream& out, const Node& n, uint64_t& curr);
    [[noreturn]] void fatalError(const QString& msg);
    static Node* createNode(const NodeType& type);
    static Node* createNode(const NodeType& type, Node* child);
    static Node* createNode(const NodeType& type, Node* lhs, Node* rhs);
    static Node* createNode(const NodeType& type, std::vector<Node*> children);

    void parse();
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

    void scan();
    void scanEscapeCode();
    void scanConstruct();
    void scanNumber();
    void scanText();
    bool isIdentifierQChar(const QChar& c) const;
    bool advanceScanner();
    void emitToken(const TokenType& t);
    void emitToken(const TokenType& t, QString::size_type start);
    void emitToken(const TokenType& t, QString::size_type start, QString::size_type end);
};

#endif // PARSER_H
