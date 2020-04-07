#include "nebparser.h"

namespace Neb {

Parser::Parser(const QString& source)
    : source(source),
      scanner(source),
      tokens(scanner.tokens) {
    //DO NOTHING
}

std::vector<Node*> Parser::parse(){
    try{
        scanner.scan();
    }catch(int code){
        if(code == 646) err_msg = scanner.err_msg;
        throw code;
    }

    while(match(Newline));
    while(token_index < tokens.size()){
        statements.push_back( statement() );
        while(!dangling_nodes.empty()) dangling_nodes.pop();
        if(token_index < tokens.size()) consume({Newline, Comma});
        while(match(Newline));
    }

    return statements;
}

QString Parser::getErrorMessage(){
    Q_ASSERT(!err_msg.isEmpty());
    return err_msg;
}

QString::size_type Parser::errorStart(){
    Token t = (token_index < tokens.size()) ? tokens[token_index] : tokens.back();
    return t.start;
}

QString::size_type Parser::errorEnd(){
    Token t = (token_index < tokens.size()) ? tokens[token_index] : tokens.back();
    return t.end;
}

void Parser::fatalError(const QString& msg){
    err_msg = "Parser Error:\n" + msg;

    if(token_index >= tokens.size()) err_msg += "\nat end of file";
    else err_msg += "\nToken: " + token_names[tokens[token_index].type];

    //Clean up dynamically allocated memory
    for(Node* n : statements) NodeFunction::deletePostorder(n);
    while(!dangling_nodes.empty()){
        delete dangling_nodes.top();
        dangling_nodes.pop();
    }

    throw 646;
}

Node* Parser::createNode(const NodeType& type){
    Node* n = new Node;
    n->type = type;
    dangling_nodes.push(n);

    return n;
}

Node* Parser::createNode(const NodeType& type, Node* child){
    Node* n = new Node;
    n->type = type;
    n->children.push_back(child);
    dangling_nodes.push(n);

    return n;
}

Node* Parser::createNode(const NodeType& type, Node* lhs, Node* rhs){
    Node* n = new Node;
    n->type = type;
    n->children.push_back(lhs);
    n->children.push_back(rhs);
    dangling_nodes.push(n);

    return n;
}

Node* Parser::createNode(const NodeType& type, const std::vector<Node*>& args){
    Node* n = new Node;
    n->type = type;
    n->children = args;
    dangling_nodes.push(n);

    return n;
}

void Parser::consume(const TokenType& t){
    if(token_index >= tokens.size())
        fatalError("Reached end of token stream while scanning for token: " + token_names[t]);

    if(t != tokens[token_index++].type){
        token_index--;
        fatalError(QString("Expected token type '") + token_names[t] + "', got type '"
                   + token_names[tokens[token_index].type] + "'");
    }
}

void Parser::consume(const std::vector<TokenType>& types){
    if(token_index >= tokens.size()){
        QString tokens;
        for(TokenType t : types) tokens += '\n' + token_names[t];
        fatalError("Reached end of token stream while scanning for tokens:" + tokens);
    }

    TokenType t = tokens[token_index++].type;
    for(TokenType type : types) if(type == t) return;
    token_index--;
    QString tokens;
    for(TokenType t : types) tokens += "\n\t" + token_names[t] + ',';
    fatalError(QString("Invalid token type '") + token_names[t] + "' ∉ {" + tokens + "\n}");
}

bool Parser::match(const TokenType& t){
    if(token_index >= tokens.size()) return false;
    if(t == tokens[token_index].type){
        token_index++;
        return true;
    }else{
        return false;
    }
}

bool Parser::match(const std::vector<TokenType>& types){
    if(token_index >= tokens.size()) return false;
    TokenType t = tokens[token_index].type;
    for(TokenType type : types) if(type == t){
        token_index++;
        return true;
    }

    return false;
}

bool Parser::peek(const TokenType& t) const{
    return token_index < tokens.size() && t == tokens[token_index].type;
}

bool Parser::peek(const std::vector<TokenType>& types) const{
    if(token_index >= tokens.size()) return false;
    TokenType t = tokens[token_index].type;
    for(TokenType type : types) if(type == t) return true;
    return false;
}

Node* Parser::statement(){
    Node* n = expression();

    if(token_index >= tokens.size()) return createNode(EXPR_STMT, n);

    switch (tokens[token_index++].type) {
        case Equals: return equality(n);
        case Less: return less(n);
        case LessEqual: return less(n);
        case Greater: return greater(n);
        case GreaterEqual: return greater(n);
        case In: return createNode(IN, n, expression());
        case DefEquals: return createNode(DEFINE_EQUALS, n, expression());
        case NotEqual: return createNode(NOT_EQUAL, n, expression());
        case Proportional: return createNode(PROPORTIONAL, n, expression());
        default: token_index--; return createNode(EXPR_STMT, n);
    }
}

Node* Parser::equality(Node* n){
    match(Newline);
    Node* equality_stmt = createNode(EQUAL, n, expression());
    while(match(Equals)){
        match(Newline);
        equality_stmt->children.push_back(expression());
    }

    return equality_stmt;
}

Node* Parser::less(Node* n){
    do{
        bool inclusive = tokens[token_index-1].type == LessEqual;
        match(Newline);
        n = createNode(inclusive ? LESS_EQUAL : LESS, n, expression());
    } while(match({Less, LessEqual}));

    return n;
}

Node* Parser::greater(Node* n){
    do{
        bool inclusive = tokens[token_index-1].type == GreaterEqual;
        match(Newline);
        n = createNode(inclusive ? GREATER_EQUAL : GREATER, n, expression());
    } while(match({Greater, GreaterEqual}));

    return n;
}

Node* Parser::expression(){
    return conjunction();
}

Node* Parser::conjunction(){
    Node* expr = disjunction();
    if(match(Conjunction)){
        match(Newline);
        expr = createNode(LOGICAL_AND, expr, disjunction());
        while(match(Conjunction)){
            match(Newline);
            expr->children.push_back(disjunction());
        }
    }

    return expr;
}

Node* Parser::disjunction(){
    Node* expr = addition();
    if(match(Disjunction)){
        match(Newline);
        expr = createNode(LOGICAL_OR, expr, addition());
        while(match(Disjunction)){
            match(Newline);
            expr->children.push_back(addition());
        }
    }

    return expr;
}

Node* Parser::addition(){ //Left associative (subtraction is anti-commutative)
    Node* expr = multiplication();
    while(match({Plus, PlusMinus, Minus, MinusPlus, Cap, Cup})){
        TokenType t = tokens[token_index-1].type;
        match(Newline);
        Node* rhs = multiplication();
        switch(t){
            case Plus: expr = createNode(ADDITION, expr, rhs); break;
            case PlusMinus: expr = createNode(PLUS_MINUS_BINARY, expr, rhs); break;
            case Minus: expr = createNode(SUBTRACTION, expr, rhs); break;
            case MinusPlus: expr = createNode(MINUS_PLUS_BINARY, expr, rhs); break;
            case Cap: expr = createNode(INTERSECTION, expr, rhs); break;
            case Cup: expr = createNode(UNION, expr, rhs); break;
            default: break;
        }
    }

    return expr;
}

Node* Parser::multiplication(){
    Node* expr = implicitMultiplication();
    while(match({Backslash, Divide, DotProduct, Forwardslash, Multiply, ODot, OuterProduct, Percent, Times})){
        TokenType t = tokens[token_index-1].type;
        match(Newline);
        Node* rhs = implicitMultiplication();
        switch(t){
            case Backslash: expr = createNode(BACKSLASH, expr, rhs); break;
            case Divide: expr = createNode(DIVIDE, expr, rhs); break;
            case DotProduct: expr = createNode(DOT, expr, rhs); break;
            case Forwardslash: expr = createNode(FORWARDSLASH, expr, rhs); break;
            case Multiply: expr = createNode(MULTIPLICATION, expr, rhs); break;
            case ODot: expr = createNode(ODOT, expr, rhs); break;
            case OuterProduct: expr = createNode(OUTER_PRODUCT, expr, rhs); break;
            case Percent: expr = createNode(MODULUS, expr, rhs); break;
            case Times: expr = createNode(CROSS, expr, rhs); break;
            default: break;
        }
    }

    return expr;
}

Node* Parser::implicitMultiplication(){
    Node* expr = leftUnary();

    //Bar, DoubleBar have identical open and close symbols, so are disallowed for implicit mult
    if(expr->type == ABS || expr->type == NORM) return expr;

    if(peek({Identifier, IntegralUnicode, LeftAngle, LeftBracket, LeftCeil, LeftFloor,
            LeftParen, Nabla, SpecialEscape})){
        expr = createNode(IMPLICIT_MULTIPLY, expr, leftUnary());

        while(peek({Identifier, IntegralUnicode, LeftAngle, LeftBracket, LeftCeil, LeftFloor,
                   LeftParen, Nabla, SpecialEscape}))
            expr->children.push_back(leftUnary());
    }

    return expr;
}

Node* Parser::leftUnary(){
    if(match(Minus)){
        return createNode(UNARY_MINUS, leftUnary());
    }else if(match(Not)){
        return createNode(LOGICAL_NOT, rightUnary());
    }else if(match(PlusMinus)){
        return createNode(PLUS_MINUS_UNARY, leftUnaryNext());
    }else if(match(MinusPlus)){
        return createNode(MINUS_PLUS_UNARY, leftUnaryNext());
    }else if(match(Nabla)){
        return nablaStart();
    }else{
        return leftUnaryNext();
    }
}

Node* Parser::nablaStart(){
    if(match(DotProduct)){
        return createNode(DIVERGENCE, leftUnaryNext());
    }else if(match(Times)){
        return createNode(CURL, leftUnaryNext());
    }else{
        return createNode(GRADIENT, leftUnaryNext());
    }
}

Node* Parser::leftUnaryNext(){
    if(match(Dollar)) return createNode(CURRENCY_DOLLARS, grouping());
    else if(match(Euro)) return createNode(CURRENCY_EUROS, grouping());
    else if(match(PoundSterling)) return createNode(CURRENCY_POUNDS, grouping());
    else return rightUnary();
}

Node* Parser::rightUnary(){
    Node* expr = exponent();
    while(peek( {Exclam, Tick} )){
        if(match(Exclam)){
            if(peek(Exclam)){
                int degree = 1;
                while(match(Exclam)) degree++;
                Node* n = createNode(MULTIFACTORIAL, expr);
                n->subtext = QString::number(degree);

                return n;
            }else{
                return createNode(FACTORIAL, expr);
            }
        }else{
            consume(Tick);
            if(peek(Tick)){
                int degree = 1;
                while(match(Tick)) degree++;
                Node* n = createNode(TICK_DERIVATIVE, expr);
                n->subtext = QString::number(degree);

                return n;
            }else{
                return createNode(TICK_DERIVATIVE, expr);
            }
        }
    }

    return expr;
}

Node* Parser::exponent(){
    Node* expr = grouping();
    if(match(Caret)){
        match(Newline);
        expr = createNode(POWER, expr, leftUnary());
    }
    return expr;
}

Node* Parser::grouping(){
    if(match(LeftParen)) return parenGrouping();
    else if(match(LeftBracket)) return bracketGrouping();
    else if(match(LeftBrace)) return setGrouping();
    else if(match(LeftAngle)) return innerProduct();
    else if(match(Bar)) return generalGrouping(ABS, Bar);
    else if(match(DoubleBar)) return generalGrouping(NORM, DoubleBar);
    else if(match(LeftCeil)) return generalGrouping(CEILING, RightCeil);
    else if(match(LeftFloor)) return generalGrouping(FLOOR, RightFloor);
    else if(match(IntegralUnicode)) return integralUnicode(INTEGRAL);
    else if(match(SpecialEscape)) return escape();
    else return terminal();
}

Node* Parser::parenGrouping(){
    Node* expr = expression();

    if(match(Comma)){ //Exclusive start of range, e.g. x ∈ (0, 1]
        Node* range_end = expression();

        if(match(RightParen)){
            return createNode(RANGE_OPEN_OPEN, expr, range_end);
        }else{
            consume(RightBracket);
            return createNode(RANGE_OPEN_CLOSE, expr, range_end);
        }
    }else{
        consume(RightParen);
        return createNode(PAREN_GROUPING, expr);
    }
}

Node* Parser::bracketGrouping(){
    Node* expr = expression();

    if(match(Comma)){ //Inclusive start of range, e.g. x ∈ [0, 1)
        Node* range_end = expression();

        if(match(RightParen)){
            return createNode(RANGE_CLOSE_OPEN, expr, range_end);
        }else{
            consume(RightBracket);
            return createNode(RANGE_CLOSE_CLOSE, expr, range_end);
        }
    }else{
        consume(RightBracket);
        return createNode(BRACKET_GROUPING, expr);
    }
}

Node* Parser::setGrouping(){
    if(match(RightBrace)) return createNode(SET_ENUMERATED); //Empty set

    Node* expr = expression();
    if(match(RightBrace)) return createNode(SET_ENUMERATED, expr); //1 member set
    else if(peek(Comma)){ //n-member enumerated set
        expr = createNode(SET_ENUMERATED, expr);
        while(match(Comma)){
            match(Newline);
            expr->children.push_back( expression() );
        }
        consume(RightBrace);

        return expr;
    }else{ //Set selector
        expr = match(In) ?
               createNode(SET_BUILDER, createNode(IN, expr, expression())) : // {x ∈ ℝ : ...}
               createNode(SET_BUILDER, expr); // {x : ...}

        consume({Colon, Bar});

        expr->children.push_back(statement());
        while(match(Comma)) expr->children.push_back(statement());
        consume(RightBrace);

        return expr;
    }
}

Node* Parser::innerProduct(){
    Node* expr = expression();
    if(match(Comma)){
        expr = createNode(INNER_PRODUCT, expr, expression());
    }else{
        consume(Bar);
        expr = createNode(INNER_PRODUCT, expression(), expr);
    }
    consume(RightAngle);

    return expr;
}

Node* Parser::generalGrouping(const NodeType& node_type, const TokenType& close_token){
    Node* expr = expression();
    consume(close_token);

    return createNode(node_type, expr);
}

Node* Parser::call(Node* callee){
    if(match(RightParen)) return createNode(CALL, callee, createNode(ARGS));

    match(Newline);
    Node* first = expression();

    if(match(Bar)){
        Node* condition = expression();
        consume(RightParen);

        if(callee->type != IDENTIFIER || callee->subtext != "P")
            fatalError("Probablity function can only be represented by P");

        return createNode(CONDITIONAL_PROBABLITY, first, condition);
    }

    Node* args = createNode(ARGS, first);

    while(!match(RightParen)){
        consume(Comma);
        match(Newline);
        args->children.push_back( expression() );
    }

    return createNode(CALL, callee, args);
}

Node* Parser::escape(){
    if(token_index >= tokens.size()) fatalError("No token after Escape");

    switch (tokens[token_index++].type) {
        case AccentArrow: return escapeAccent(ACCENT_ARROW);
        case AccentBar: return escapeAccent(ACCENT_BAR);
        case AccentBreve: return escapeAccent(ACCENT_BREVE);
        case AccentDot: return escapeAccent(ACCENT_DOT);
        case AccentDdot: return escapeAccent(ACCENT_DDOT);
        case AccentDddot: return escapeAccent(ACCENT_DDDOT);
        case AccentHat: return escapeAccent(ACCENT_HAT);
        case AccentTilde: return escapeAccent(ACCENT_TILDE);
        case Bar: return escapeAccent(ABS);
        case DoubleBar: return escapeAccent(NORM);
        case LeftCeil: return escapeAccent(CEILING);
        case LeftFloor: return escapeAccent(FLOOR);
        case LeftParen: return escapeAccent(PAREN_GROUPING);
        case LeftBracket: return escapeAccent(BRACKET_GROUPING);
        case BigSum: return escapeBigOperator(SUMMATION);
        case BigProduct: return escapeBigOperator(PRODUCT);
        case Binomial: return escapeBinomial();
        case Cases: return escapeCases();
        case Fraction: return escapeFraction();
        case Integral: return escapeIntegral(INTEGRAL);
        case Matrix: return escapeMatrix();
        case Subscript: return escapeSubscript();
        case Superscript: return escapeSuperscript();
        case Root: return escapeRoot();
        case UnderscriptedWord: return escapeUnderscriptedWord();
        default:
            fatalError("Invalid escape code (Call from parser.cpp " + QString::number(__LINE__) + ")");
    }
}

Node* Parser::escapeAccent(const NodeType& type){
    consume(SpecialOpen);
    Node* expr = createNode(type, expression());
    consume(SpecialClose);

    return expr;
}

Node* Parser::escapeBigOperator(const NodeType& type){
    Node* n = createNode(type);

    //Optional underscript
    if(match(SpecialOpen)){
        n->children.push_back(statement());
        consume(SpecialClose);

        //Optional overscript
        if(match(SpecialOpen)){
            n->children.push_back(expression());
            consume(SpecialClose);
        }
    }

    //Body
    n->children.push_back(expression());

    return n;
}

Node* Parser::escapeBinomial(){
    consume(SpecialOpen);
    Node* num = expression();
    consume(SpecialClose);
    consume(SpecialOpen);
    Node* den = expression();
    consume(SpecialClose);

    return createNode(TYPED_BINOMIAL, num, den);
}

Node* Parser::escapeCases(){
    Node* n = createNode(TYPED_CASES);

    do{
        consume(SpecialOpen);
        n->children.push_back(expression());
        consume(SpecialClose);
        consume(SpecialOpen);
        n->children.push_back(statement());
        consume(SpecialClose);
    } while(peek(SpecialOpen));

    return n;
}

Node* Parser::escapeFraction(){
    consume(SpecialOpen);

    if(match(Differential)) return fractionDerivative(DERIVATIVE, Differential);
    else if(match(Partial)) return fractionDerivative(PARTIAL, Partial);

    Node* num = expression();
    consume(SpecialClose);
    consume(SpecialOpen);
    Node* den = expression();
    consume(SpecialClose);

    return createNode(TYPED_FRACTION, num, den);
}

Node* Parser::fractionDerivative(const NodeType& type, const TokenType& deriv_token){
    Node* expr;
    Node* wrt;

    if(match(SpecialClose)){ //Derivative comes after fraction: (d/dx) y
        consume(SpecialOpen);
        consume(deriv_token);
        wrt = idOnly();
        consume(SpecialClose);
        expr = grouping();
    }else{ //Derivative is part of fraction: (dy/dx)
        expr = grouping();
        consume(SpecialClose);
        consume(SpecialOpen);
        consume(deriv_token);
        wrt = idOnly();
        consume(SpecialClose);
    }

    return createNode(type, expr, wrt);
}

Node* Parser::escapeIntegral(const NodeType& type){
    Node* n = createNode(type);

    //Optional subscript
    if(match(SpecialOpen)){
        n->children.push_back(expression());
        consume(SpecialClose);

        //Optional superscript
        if(match(SpecialOpen)){
            n->children.push_back(expression());
            consume(SpecialClose);
        }
    }

    //Kernel
    n->children.push_back(expression());

    //Integration variable
    consume(Differential);
    n->children.push_back(idOnly());

    return n;
}

Node* Parser::integralUnicode(const NodeType& type){
    Node* n = createNode(type, expression()); //Unicode integral must be indefinite integral
    consume(Differential);
    n->children.push_back(idOnly());

    return n;
}

Node* Parser::escapeMatrix(){
    Node* expr = createNode(TYPED_MATRIX);

    uint rows = escapeDimension();
    uint cols = escapeDimension();

    for(uint i = 0; i < rows*cols; i++){
        consume(SpecialOpen);
        expr->children.push_back( expression() );
        consume(SpecialClose);
    }

    return expr;
}

uint Parser::escapeDimension(){
    consume(SpecialOpen);
    consume(Number);
    bool success;
    QString::size_type start = tokens[token_index-1].start;
    QString::size_type end = tokens[token_index-1].end;
    uint size = source.mid(start, end-start).toUInt(&success);
    if(!success) fatalError("Error parsing matrix size");
    if(size == 0) fatalError("Error parsing matrix size: size==0");
    consume(SpecialClose);

    return size;
}

Node* Parser::escapeSubscript(){
    consume(SpecialOpen);
    Node* expr = leftUnary();
    consume(SpecialClose);
    consume(SpecialOpen);

    if(token_index >= tokens.size())
        fatalError("Ran out of tokens (Call from parser.cpp " + QString::number(__LINE__) + ")");

    switch (tokens[token_index++].type) {
        case Multiply:
            if(expr->type != IDENTIFIER) fatalError("Asterisk superscript on non-identifier");
            expr->subtext += "_*";
            break;
        case Comma:
            expr = createNode(SUBSCRIPT_PARTIAL, expr, idOnly());
            break;
        default:
            token_index--;
            expr = createNode(SUBSCRIPT_ACCESS, expr, expression());
            while(match(Comma)) expr->children.push_back(expression());

            //Note: this could be part of identifier if lhs is id and subscript is single id,
            //      e.g. u = Kₚ * e
    }

    consume(SpecialClose);

    if(match(LeftParen)) return call(expr);

    return expr;
}

Node* Parser::escapeSuperscript(){
    consume(SpecialOpen);
    Node* expr = leftUnary();
    consume(SpecialClose);
    consume(SpecialOpen);

    if(token_index >= tokens.size())
        fatalError("Ran out of tokens (Call from parser.cpp " + QString::number(__LINE__) + ")");

    switch (tokens[token_index++].type) {
        case Multiply:
            if(expr->type != IDENTIFIER) fatalError("Asterisk superscript on non-identifier");
            expr->subtext += "^*";
            break;
        case Transpose:
            expr = createNode(TRANSPOSE, expr);
            break;
        case Dagger:
            expr = createNode(DAGGER, expr);
            break;
        case Conjunction:
            expr = createNode(WEDGE, expr);
            break;
        case Disjunction:
            expr = createNode(VEE, expr);
            break;
        case Plus:
            if(expr->type == IDENTIFIER) expr = createNode(INCREMENT, expr);
            else if(expr->type == REALS) expr->type = POSITIVE_REALS;
            else fatalError("Unexpected '+' superscript");
            break;
        case Minus:
            if(expr->type == IDENTIFIER) expr = createNode(DECREMENT, expr);
            else if(expr->type == REALS) expr->type = NEGATIVE_REALS;
            else fatalError("Unexpected '-' superscript");
            break;
        default:
            token_index--;
            if(expr->type == COMPLEX_NUMS) expr = createNode(CONTINUOUS, expression());
            else expr = createNode(TYPED_POWER, expr, expression());
    }

    consume(SpecialClose);

    return expr;
}

Node* Parser::escapeUnderscriptedWord(){
    consume(SpecialOpen);
    if(match(Lim)){
        Node* n = createNode(LIMIT);
        consume(SpecialClose);
        consume(SpecialOpen);
        n->children.push_back( expression() );
        consume(RightArrow);
        n->children.push_back( expression() );
        consume(SpecialClose);
        n->children.push_back( expression() );

        return n;
    }else{
        Node* n;
        if(match(Min)) n = createNode(MIN);
        else if(match(Max)) n = createNode(MAX);
        else if(match(Infimum)) n = createNode(INFIMUM);
        else{
            consume(Supremum);
            n = createNode(SUPREMUM);
        }

        consume(SpecialClose);
        consume(SpecialOpen);
        n->children.push_back( statement() );
        consume(SpecialClose);
        n->children.push_back( expression() );

        return n;
    }
}

Node* Parser::escapeRoot(){
    consume(SpecialOpen);
    Node* child = expression();
    consume(SpecialClose);
    if(match(SpecialOpen)){
        Node* power = expression();
        consume(SpecialClose);

        return createNode(TYPED_ROOT, child, power);
    }else{
        return createNode(TYPED_SQRT, child);
    }
}

Node* Parser::terminal(){
    if(token_index >= tokens.size())
        fatalError("Ran out of tokens (Call from parser.cpp " + QString::number(__LINE__) + ")");

    QString::size_type start = tokens[token_index].start;
    QString::size_type end = tokens[token_index].end;
    QString source_text = source.mid(start, end-start);

    switch (tokens[token_index++].type) {
        case Integer: return createNode(INTEGERS);
        case Natural: return createNode(NATURAL_NUMS);
        case Rational: return createNode(RATIONAL_NUMS);
        case Real: return createNode(REALS);
        case Quaternion: return createNode(QUATERNIONS);
        case Complex: return createNode(COMPLEX_NUMS);
        case Infinity: return createNode(INFTY);
        case Boolean: return createNode(BOOLEANS);
        case Identifier: return idStart(source_text);
        case Number:{
            Node* n = createNode(NUMBER);
            n->subtext = source_text;
            return n;
        }
        default:
            fatalError("Invalid token (Call from parser.cpp " + QString::number(__LINE__) + ")");
    }
}

Node* Parser::idStart(const QString& text){
    Node* n = createNode(IDENTIFIER);
    n->subtext = text;

    if(match(LeftParen)) return call(n);

    return n;
}

Node* Parser::idOnly(){
    if(match(Identifier)){
        QString::size_type start = tokens[token_index-1].start;
        QString::size_type end = tokens[token_index-1].end;
        QString source_text = source.mid(start, end-start);

        Node* id = createNode(IDENTIFIER);
        id->subtext = source_text;

        return id;
    }else{
        if(match(SpecialEscape)){
            if(match(Subscript)) return escapeSubscript();
        }

        fatalError("Invalid identifier");
    }
}

}
