#ifndef PARSER_H
#define PARSER_H

#include <QMap>
#include <QString>
#include <QTextStream>
#include <vector>

class Parser{
public:
    struct Node {
        QChar label;
        QString subtext;
        std::vector<Node*> children;
    };

private:
    enum TokenType {
        At,
        Backslash,
        Bar,
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
        Dot,
        DoubleBar,
        DoubleExclam,
        EmptySet,
        Equals,
        Exclam,
        Forwardslash,
        Fraction,
        Greater,
        GreaterEqual,
        Hash,
        Identifier,
        In,
        Infinity,
        Integer,
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
        Matrix,
        Minus,
        Multiply,
        Natural,
        Newline,
        Not,
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
        Times,
        Transpose
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

public:
    static std::vector<Node*> parse(const QString& source);
    static QString toDOT(const Node& n);
    static QString toDOT(const std::vector<Node*>& nodes);

private:
    Parser(const QString& source);
    static uint64_t writeDOT(QTextStream& out, const Node& n, uint64_t& curr);
    [[noreturn]] void fatalError(const QString& msg);
    static Node* createNode(const QChar& label);
    static Node* createNode(const QChar& label, Node* child);
    static Node* createNode(const QChar& label, Node* lhs, Node* rhs);
    static Node* createNode(const QChar& label, std::vector<Node*> children);

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
    Node* addition();
    Node* multiplication();
    Node* leftUnary();
    Node* rightUnary();
    Node* exponent();
    Node* grouping();
    Node* escape();
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
};

#endif // PARSER_H
