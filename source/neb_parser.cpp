#include "neb_parser.h"

namespace Neb {

Parser::Parser(const QString& source)
    : source(source),
      scanner(source, err_msg) {
    advance();
}

Node* Parser::parseStatement(){
    if(current.type == Error){
        scanToRecoveryPoint();
        err_msg.clear();
    }

    while(match(Newline));
    if(match(EndOfFile)) return nullptr;

    Node* body = statementBody();
    consume<3>({Newline, Comma, EndOfFile}, "Expect statement terminator");

    if(current.type == Error){
        NodeFunction::deletePostorder(body);
        return new Node(ERROR, QString(current.start));
    }else{
        return body;
    }
}

Node* Parser::createNode(const NodeType& type){
    return new Node(type, source.mid(previous.start, previous.length));
}

Node* Parser::createNode(const NodeType& type, Node* child){
    Node* n = createNode(type);
    n->children.push_back(child);

    return n;
}

Node* Parser::createNode(const NodeType& type, Node* lhs, Node* rhs){
    Node* n = createNode(type);
    n->children.push_back(lhs);
    n->children.push_back(rhs);

    return n;
}

Node* Parser::createNode(uint number){
    Node* n = createNode(NUMBER);
    n->data = QString::number(number);

    return n;
}

template<int a>
Node* Parser::createNode(const NodeType& type, const std::array<Node*,a>& args){
    Node* n = createNode(type);
    n->children.insert(std::begin(n->children), std::begin(args), std::end(args));

    return n;
}

void Parser::scanToRecoveryPoint(){
    bool line_continuation = false;

    while(!(match(EndOfFile) || (!line_continuation && match(Newline)))){
        if(match<8>({Plus, Minus, PlusMinus, MinusPlus, Multiply, Divide, Forwardslash, Backslash})){
            line_continuation = true;
        }else{
            line_continuation = false;
            previous = current;
            current = scanner.scanToken();
        }
    }
}

void Parser::error(const QString& message){
    error(message, current);
}

void Parser::error(const QString& message, const Token& t){
    if(!err_msg.isEmpty()) return;

    err_msg = message;

    current.type = Error;
    current.start = t.start;
    current.length = t.length;
}

void Parser::advance(){
    if(current.type == Error) return;

    previous = current;
    current = scanner.scanToken();
}

void Parser::checkGap(){
    if(previous.start + previous.length != current.start) error("Invalid whitespace");
}

void Parser::consume(TokenType type, const QString& message){
    if (current.type == type) {
        advance();
        return;
    }

    error(message);
}

template<int n>
void Parser::consume(const std::array<TokenType,n>& types, const QString& message){
    for(TokenType t : types){
        if(current.type == t){
            advance();
            return;
        }
    }

    error(message);
}

bool Parser::match(TokenType type){
    if(current.type == type){
        advance();
        return true;
    }else{
        return false;
    }
}

template<int n>
bool Parser::match(const std::array<TokenType,n>& types){
    for(TokenType t : types){
        if(current.type == t){
            advance();
            return true;
        }
    }

    return false;
}

bool Parser::peek(TokenType type) const{
    return current.type == type;
}

template<int n>
bool Parser::peek(const std::array<TokenType,n>& types) const{
    for(TokenType t : types)
        if(current.type == t) return true;
    return false;
}

Node* Parser::statementBody(){
    Node* n = expression();

    switch (current.type) {
        case Equals: advance(); return equality(n);
        case Less: advance(); return less(n);
        case LessEqual: advance(); return less(n);
        case Greater: advance(); return greater(n);
        case GreaterEqual: advance(); return greater(n);
        case In: advance(); return createNode(IN, n, expression());
        case DefEquals: advance(); return createNode(DEFINE_EQUALS, n, expression());
        case NotEqual: advance(); return createNode(NOT_EQUAL, n, expression());
        case Proportional: advance(); return createNode(PROPORTIONAL, n, expression());
        default: return createNode(EXPR_STMT, n);
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
        bool inclusive = previous.type == LessEqual;
        match(Newline);
        n = createNode(inclusive ? LESS_EQUAL : LESS, n, expression());
    } while(match<2>({Less, LessEqual}));

    return n;
}

Node* Parser::greater(Node* n){
    do{
        bool inclusive = previous.type == GreaterEqual;
        match(Newline);
        n = createNode(inclusive ? GREATER_EQUAL : GREATER, n, expression());
    } while(match<2>({Greater, GreaterEqual}));

    return n;
}

Node* Parser::expression(){
    return disjunction();
}

Node* Parser::disjunction(){
    Node* expr = conjunction();
    if(match(Disjunction)){
        match(Newline);
        expr = createNode(LOGICAL_OR, expr, conjunction());
        while(match(Disjunction)){
            match(Newline);
            expr->children.push_back(conjunction());
        }
    }

    return expr;
}

Node* Parser::conjunction(){
    Node* expr = addition();
    if(match(Conjunction)){
        match(Newline);
        expr = createNode(LOGICAL_AND, expr, addition());
        while(match(Conjunction)){
            match(Newline);
            expr->children.push_back(addition());
        }
    }

    return expr;
}

Node* Parser::addition(){ //Left associative (subtraction is anti-commutative)
    Node* expr = multiplication();
    while(match<6>({Plus, PlusMinus, Minus, MinusPlus, Cap, Cup})){
        TokenType t = previous.type;
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
    while(match<9>({Backslash, Divide, DotProduct, Forwardslash, Multiply, ODot, OuterProduct, Percent})
          || (!parsing_dimensions && match(Times)) ){
        TokenType t = previous.type;
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

    if(peek<NEB_NUM_IMPLICIT_MULT>(NEB_IMPLICIT_MULT_MACRO_EXPANSION)){
        expr = createNode(IMPLICIT_MULTIPLY, expr, rightUnary());

        while(peek<NEB_NUM_IMPLICIT_MULT>(NEB_IMPLICIT_MULT_MACRO_EXPANSION))
            expr->children.push_back(rightUnary());
    }

    return expr;
}

Node* Parser::leftUnary(){
    switch (current.type) {
        case Minus: advance(); checkGap(); return createNode(UNARY_MINUS, rightUnary());
        case Not: advance(); checkGap(); return createNode(LOGICAL_NOT, rightUnary());
        case PlusMinus: advance(); checkGap(); return createNode(PLUS_MINUS_UNARY, rightUnary());
        case MinusPlus: advance(); checkGap(); return createNode(MINUS_PLUS_UNARY, rightUnary());
        default: return rightUnary();
    }
}

Node* Parser::rightUnary(){
    Node* expr = exponent();
    while(peek<2>( {Exclam, Tick} )){
        if(match(Exclam)){
            Node* n = createNode(FACTORIAL);
            while(peek(Exclam)){
                checkGap();
                advance();
                n->data += '!';
            }

            return n;
        }else{
            consume(Tick, "Expect '");
            Node* n = createNode(TICK_DERIVATIVE);
            while(match(Tick));
            while(peek(Tick)){
                checkGap();
                advance();
                n->data += '\'';
            }

            return n;
        }
    }

    return expr;
}

Node* Parser::exponent(){
    Node* n = script();
    if(match(Caret)){
        match(Newline);
        n = createNode(POWER, n, implicitMultiplication());
    }
    return n;
}

Node* Parser::script(){
    Node* n = primary();

    switch (current.type) {
        case MB_Superscript: advance(); return mathBranSuperscript(n);
        case MB_Subscript: advance(); return mathBranSubscript(n);
        case MB_Dualscript: advance(); return mathBranDualscript(n);
        case SuperscriptLeftParen: return createNode(POWER, n, unicodeSuperscriptGrouping());
        case SuperscriptNumber: return createNode(POWER, n, unicodeSuperscriptExpression());
        case SuperscriptIdentifier: return createNode(POWER, n, unicodeSuperscriptExpression());
        case SuperscriptPlus: return createNode(INCREMENT, n);
        case SuperscriptMinus: return createNode(DECREMENT, n);
        case SuperscriptWedge: return createNode(WEDGE, n);
        case SuperscriptVee: return createNode(VEE, n);
        case SubscriptLeftParen: return createNode(SUBSCRIPT_ACCESS, n, unicodeSubscriptGrouping());
        case SubscriptNumber: return createNode(SUBSCRIPT_ACCESS, n, unicodeSubscriptExpression());
        case SubscriptIdentifier: return createNode(SUBSCRIPT_ACCESS, n, unicodeSubscriptExpression());
        default:
            return n;
    }
}

Node* Parser::primary(){
    advance();

    switch (previous.type) {
        //Cascading rules
        case Identifier: return idStart();
        case LeftParen: return parenStart();
        case LeftBrace: return braceStart();
        case LeftDoubleBrace: return doubleBraceStart();
        case LeftBracket: return setStart();

        //Value literal
        case EmptySet: return createNode(EMPTY_SET);
        case Infinity: return createNode(INFTY);
        case Number: return createNode(NUMBER);

        //Grouping
        case LeftCeil: return grouping(CEIL, RightCeil);
        case LeftFloor: return grouping(FLOOR, RightFloor);
        case Bar: return grouping(ABS, Bar);
        case DoubleBar: return grouping(NORM, DoubleBar);

        //Set literal
        case Doublestruck_B: return setLiteral<BOOLEANS>();
        case Doublestruck_C: return setLiteral<COMPLEX_NUMS>();
        case Doublestruck_H: return createNode(QUATERNIONS);
        case Doublestruck_N: return setLiteral<NATURALS>();
        case Doublestruck_Q: return setLiteral<RATIONALS>();
        case Doublestruck_R: return setLiteral<REALS>();
        case Doublestruck_Z: return setLiteral<INTEGERS>();

        //Currency
        case Dollar: return createNode(CURRENCY_DOLLARS, primary());
        case Euro: return createNode(CURRENCY_EUROS, primary());
        case PoundSterling: return createNode(CURRENCY_POUNDS, primary());

        //Unicode Math
        case Nabla: return nablaStart();
        case LeftAngle: return innerProduct();
        case Integral: return integral(INTEGRAL);
        case ContourIntegral: return integral(CONTOUR_INTEGRAL);

        //MathBran
        case MB_Binomial: return mathBranBinary(BINOMIAL);
        case MB_Root: return mathBranRoot();
        case MB_Integral: return mathBranIntegral(INTEGRAL);
        case MB_ContourIntegral: return mathBranIntegral(CONTOUR_INTEGRAL);
        case MB_DoubleIntegral: return mathBranIntegral(DOUBLE_INTEGRAL);
        case MB_ClosedSurfaceIntegral: return mathBranIntegral(CLOSED_SURFACE_INTEGRAL);
        case MB_TripleIntegral: return mathBranIntegral(TRIPLE_INTEGRAL);
        case MB_ClosedVolumeIntegral: return mathBranIntegral(CLOSED_VOLUME_INTEGRAL);
        case MB_Cases: return mathBranCases();
        case MB_Sum: return mathBranBigOperator(SUMMATION);
        case MB_Product: return mathBranBigOperator(PRODUCT);
        case MB_Intersection: return mathBranBigOperator(NARY_INTERSECTION);
        case MB_Union: return mathBranBigOperator(NARY_UNION);
        case MB_UnionPlus: return mathBranBigOperator(NARY_UNIONPLUS);
        case MB_Matrix: return mathBranMatrix();
        case MB_Fraction: return mathBranFraction();
        case MB_Max: return mathBranUnderscriptedFunction(MAX);
        case MB_Min: return mathBranUnderscriptedFunction(MIN);
        case MB_Infimum: return mathBranUnderscriptedFunction(INFIMUM);
        case MB_Supremum: return mathBranUnderscriptedFunction(SUPREMUM);
        case MB_Lim: return mathBranLimit();
        case MB_AccentHat: return mathBranUnary(ACCENT_HAT);
        case MB_AccentDot: return mathBranAccent(ACCENT_DOT);
        case MB_AccentDdot: return mathBranAccent(ACCENT_DDOT);
        case MB_AccentDddot: return mathBranAccent(ACCENT_DDDOT);
        case MB_AccentBreve: return mathBranAccent(ACCENT_BREVE);
        case MB_AccentTilde: return mathBranAccent(ACCENT_TILDE);
        case MB_AccentArrow: return mathBranAccentArrow();
        case MB_AccentBar: return mathBranUnary(ACCENT_BAR);

        default:
            error("Expected a primary expression, got \"" +
                  source.mid(previous.start, previous.length) + '"', previous);
            return createNode(ERROR);
    }
}

Node* Parser::grouping(const NodeType& t, const TokenType& close){
    Node* n = expression();
    consume(close, "Expect grouping close symbol.");

    return createNode(t, n);
}

Node* Parser::idOnly(){
    consume(Identifier, "Expect identifier");
    return idStart(true);
}

Node* Parser::idStart(bool id_only){
    Node* n = createNode(IDENTIFIER);
    if(!id_only && match(LeftParen)) return call(n);
    else if(match(MB_Superscript)){
        consume(MB_Open, "Expect open symbol");
        if(match(Multiply)){
            n->data += "^*";
            consume(MB_Close, "Expect close symbol");
        }else if(id_only){
            error("Superscript not allowed in this context");
        }else{
            return mathBranSuperscript(n, false);
        }
    }else if(match(MB_Subscript)){
        consume(MB_Open, "Expect open symbol");
        if(match(Multiply)){
            n->data += "_*";
            consume(MB_Close, "Expect close symbol");
        }else if(id_only){
            error("Superscript not allowed in this context");
        }else{
            return mathBranSubscript(n, false);
        }
    }

    return n;
}

Node* Parser::call(Node* id){
    if(match(RightParen)) return createNode(CALL, id);

    match(Newline);
    Node* first_arg = expression();

    if(match(Bar)){
        if(id->type != IDENTIFIER || id->data != "P")
            error("Probablity function can only be represented by P");

        id->type = CONDITIONAL_PROBABLITY;
        id->children.push_back(first_arg);
        id->children.push_back(expression());
        consume(RightParen, "Expect ) after conditional probability");

        return id;
    }else{
        Node* n = createNode(CALL, id, first_arg);

        while(!match(RightParen)){
            consume(Comma, "Expect ',' between call args");
            match(Newline);
            n->children.push_back(parseStatement());
        }

        return n;
    }
}

template<NodeType t>
Node* Parser::setLiteral(){
    switch (current.type) {
        case MB_Superscript: return setLiteralMathBranScript(t);
        case SuperscriptPlus:
            checkGap();
            advance();
            if(t == REALS){
                return createNode(POSITIVE_REALS);
            }else if(t == RATIONALS){
                return createNode(POSITIVE_RATIONALS);
            }else{
                error("Set does not support positive script");
                return createNode(ERROR);
            }
        case SuperscriptMinus:
            checkGap();
            advance();
            if(t == REALS){
                return createNode(NEGATIVE_REALS);
            }else if(t == RATIONALS){
                return createNode(NEGATIVE_RATIONALS);
            }else{
                error("Set does not support negative script");
                return createNode(ERROR);
            }
        case SuperscriptIdentifier:
        case SuperscriptLeftParen:
        case SuperscriptNumber:
            return createNode(SET_LITERAL_DIMENSIONS, createNode(t), unicodeSuperscriptExpression());
        default:
            return createNode(t);
    }
}

Node* Parser::setLiteralMathBranScript(const NodeType& t){
    advance();
    consume(MB_Open, "Expect MB open");
    checkGap();

    switch(current.type){
        case Plus:
            advance();
            consume(MB_Close, "Expect MB close");
            if(t == REALS){
                return createNode(POSITIVE_REALS);
            }else{
                error("Set does not support positive script");
                return createNode(ERROR);
            }
        case Minus:
            advance();
            consume(MB_Close, "Expect MB close");
            if(t == REALS){
                return createNode(NEGATIVE_REALS);
            }else{
                error("Set does not support negative script");
                return createNode(ERROR);
            }
        default:
            parsing_dimensions = true;
            Node* n = createNode(SET_LITERAL_DIMENSIONS, createNode(t), addition());
            while(match(Times)) n->children.push_back(expression());
            parsing_dimensions = false;
            consume(MB_Close, "Expect MB close");

            return n;
    }
}

template<bool check_gap>
Node* Parser::unicodeSuperscriptExpression(){
    if(check_gap) checkGap();

    Node* n = unicodeSuperscriptImplicitMult();
    while(match(SuperscriptPlus) || match(SuperscriptMinus)){
        if(previous.type == SuperscriptMinus) n = createNode(SUBTRACTION, n, unicodeSuperscriptImplicitMult());
        else if(n->type == ADDITION) n->children.push_back(unicodeSuperscriptGrouping());
        else n = createNode(ADDITION, n, unicodeSuperscriptGrouping());
    }

    return n;
}

Node* Parser::unicodeSuperscriptImplicitMult(){
    Node* n = unicodeSuperscriptGrouping();
    if(current.type == SuperscriptLeftParen || current.type == SuperscriptIdentifier){
        checkGap();
        n = createNode(IMPLICIT_MULTIPLY, n, unicodeSuperscriptGrouping());
    }
    while(current.type == SuperscriptLeftParen || current.type == SuperscriptIdentifier){
        checkGap();
        n->children.push_back(unicodeSuperscriptGrouping());
    }

    return n;
}

Node* Parser::unicodeSuperscriptGrouping(){
    if(match(SuperscriptLeftParen)){
        Node* n = unicodeSuperscriptExpression<false>();
        consume(SuperscriptRightParen, "Expect right paren");
        return createNode(GROUP_PAREN, n);
    }

    return unicodeSuperscriptTerminal();
}

Node* Parser::unicodeSuperscriptTerminal(){
    if(match(SuperscriptNumber)){
        return createNode(NUMBER);
    }else{
        consume(SuperscriptIdentifier, "Expected identifier");
        Node* n = createNode(IDENTIFIER);

        return n; //Ignore function calls for now
    }
}

template<bool check_gap>
Node* Parser::unicodeSubscriptExpression(){
    if(check_gap) checkGap();

    Node* n = unicodeSubscriptImplicitMult();
    while(match(SubscriptPlus) || match(SubscriptMinus)){
        if(previous.type == SubscriptMinus) n = createNode(SUBTRACTION, n, unicodeSubscriptImplicitMult());
        else if(n->type == ADDITION) n->children.push_back(unicodeSubscriptGrouping());
        else n = createNode(ADDITION, n, unicodeSubscriptGrouping());
    }

    return n;
}

Node* Parser::unicodeSubscriptImplicitMult(){
    Node* n = unicodeSubscriptGrouping();
    if(current.type == SubscriptLeftParen || current.type == SubscriptIdentifier){
        checkGap();
        n = createNode(IMPLICIT_MULTIPLY, n, unicodeSubscriptGrouping());
    }
    while(current.type == SubscriptLeftParen || current.type == SubscriptIdentifier){
        checkGap();
        n->children.push_back(unicodeSubscriptGrouping());
    }

    return n;
}

Node* Parser::unicodeSubscriptGrouping(){
    if(match(SubscriptLeftParen)){
        Node* n = unicodeSubscriptExpression<false>();
        consume(SubscriptRightParen, "Expect right paren");
        return createNode(GROUP_PAREN, n);
    }

    return unicodeSubscriptTerminal();
}

Node* Parser::unicodeSubscriptTerminal(){
    if(match(SubscriptNumber)){
        return createNode(NUMBER);
    }else{
        consume(SubscriptIdentifier, "Expected identifier");
        Node* n = createNode(IDENTIFIER);

        return n; //Ignore function calls for now
    }
}

Node* Parser::nablaStart(){
    if(match(DotProduct)) return createNode(DIVERGENCE, rightUnary());
    else if(match(Times)) return createNode(CURL, rightUnary());
    else return createNode(GRADIENT, rightUnary());
}

Node* Parser::parenStart(){
    Node* expr = expression();

    if(match(Comma)){ //Exclusive start of range, e.g. x ∈ (0, 1]
        Node* range_end = expression();

        if(match(RightParen)){
            return createNode(INTERVAL_OPEN_OPEN, expr, range_end);
        }else if(match(RightBrace)){
            return createNode(INTERVAL_OPEN_CLOSE, expr, range_end);
        }else{
            consume(Comma, "Invalid interval or sequence");
            Node* n = createNode(SEQUENCE_ENUMERATED, expr, range_end);
            do{
                n->children.push_back(expression());
            } while(match(Comma));
            consume(RightParen, "Expect ')' at end of sequence");

            return n;
        }
    }else{
        consume(RightParen, "Expect )");
        return createNode(GROUP_PAREN, expr);
    }
}

Node* Parser::braceStart(){
    Node* expr = expression();

    if(match(Comma)){ //Inclusive start of range, e.g. x ∈ [0, 1)
        Node* range_end = expression();

        if(match(RightParen)){
            return createNode(INTERVAL_CLOSE_OPEN, expr, range_end);
        }else if(match(RightBrace)){
            return createNode(INTERVAL_CLOSE_CLOSE, expr, range_end);
        }else if(match(Semicolon) || match(Newline)){
            return braceMatrix(createNode<3>(MATRIX, {createNode(2), expr, range_end}), 2, previous.type);
        }else{
            consume(Comma, "Expect end of interval or matrix delimiter");
            Node* n = createNode(MATRIX, expr, range_end);
            uint cols = 2;
            do{
                n->children.push_back(expression());
                cols++;
            } while(match(Comma));

            n->children.insert(n->children.begin(), createNode(cols));
            if(match(Semicolon) || match(Newline)){
                return braceMatrix(n, cols, previous.type);
            }else{
                consume(RightBrace, "Expect ']' at end of matrix");
                return n;
            }
        }
    }else if(match(Semicolon) || match(Newline)){
        return braceMatrix(createNode(MATRIX, createNode(1), expr), 1, previous.type);
    }else{
        consume(RightBrace, "Expect ]");
        return createNode(GROUP_BRACKET, expr);
    }
}

Node* Parser::braceMatrix(Node* n, uint col_count, TokenType row_delimiter){
    do{
        if(row_delimiter == Semicolon) match(Newline);
        n->children.push_back(expression());
        for(uint i = col_count-1; i > 0; i--){
            consume(Comma, "Expect ',' between matrix elements");
            n->children.push_back(expression());
        }
    } while(match(row_delimiter));
    consume(RightBrace, "Expect ']' at end of matrix");

    return n;
}

Node* Parser::doubleBraceStart(){
    Node* start = expression();
    consume(Comma, "Expect interval delimiter");
    Node* end = expression();
    consume(RightDoubleBrace, "Expect '⟧' to close integer interval");

    return createNode(INTERVAL_INTEGER, start, end);
}

Node* Parser::setStart(){
    if(match(RightBracket)) return createNode(EMPTY_SET);

    Node* expr = expression();
    if(match(RightBracket)) return createNode(SET_ENUMERATED, expr); //1 member set
    else if(current.type == Comma){ //n-member enumerated set
        expr = createNode(SET_ENUMERATED, expr);
        while(match(Comma)){
            match(Newline);
            expr->children.push_back( expression() );
        }
        consume(RightBracket, "Expect } to close enumerated set");

        return expr;
    }else{ //Set selector
        expr = match(In) ?
               createNode(SET_BUILDER, createNode(IN, expr, expression())) : // {x ∈ ℝ : ...}
               createNode(SET_BUILDER, expr); // {x : ...}

        consume<2>({Colon, Bar}, "Expect set builder");

        expr->children.push_back(statementBody());
        while(match(Comma)) expr->children.push_back(statementBody());
        consume(RightBracket, "Expect } to close set builder");

        return expr;
    }
}

Node* Parser::innerProduct(){
    Node* expr = expression();
    if(match(Comma)){
        expr = createNode(INNER_PRODUCT, expr, expression());
    }else{
        consume(Bar, "Expect '|' or ',' delimiter between inner product args");
        expr = createNode(INNER_PRODUCT, expression(), expr);
    }
    consume(RightAngle, "Expect '⟩' to close inner product");

    return expr;
}

Node* Parser::integral(const NodeType& type){
    #ifdef INTEGRAL_LOW_PRECEDENCE
    Node* n = createNode(type, implicitMultiplication());
    #else
    Node* n = createNode(type, expression());
    #endif
    consume(Doublestruck_d, "Expect differential 'ⅆ' after integral kernal");
    n->children.push_back(idOnly());

    return n;
}

Node* Parser::mathBranUnary(const NodeType& t){
    consume(MB_Open, "Expect MB open symbol");
    return grouping(t, MB_Close);
}

Node* Parser::mathBranBinary(const NodeType& t){
    consume(MB_Open, "Expect MB open symbol");
    Node* lhs = expression();
    consume(MB_Close, "Expect MB close symbol");
    consume(MB_Open, "Expect MB open symbol");
    Node* rhs = expression();
    consume(MB_Close, "Expect MB close symbol");

    return createNode(t, lhs, rhs);
}

Node* Parser::mathBranRoot(){
    consume(MB_Open, "Expect MB open symbol");
    Node* body = expression();
    consume(MB_Close, "Expect MB close symbol");
    if(match(MB_Open)){
        Node* script = expression();
        consume(MB_Close, "Expect MB close symbol");
        return createNode(ROOT, body, script);
    }else{
        return createNode(SQRT, body);
    }
}

Node* Parser::mathBranIntegral(const NodeType& type){
    //Optional subscript
    Node* t0 = nullptr;
    Node* tf = nullptr;
    if(match(MB_Open)){
        t0 = expression();
        consume(MB_Close, "Expect close symbol");

        //Optional superscript
        if((type == INTEGRAL || type == CONTOUR_INTEGRAL) && match(MB_Open)){
            tf = expression();
            consume(MB_Close, "Expect close symbol");
        }
    }

    Node* n = integral(type);
    if(t0) n->children.push_back(t0);
    if(tf) n->children.push_back(tf);

    return n;
}

Node* Parser::mathBranCases(){
    Node* n = createNode(CASES);

    consume(MB_Open, "Expect open symbol");
    do{
        n->children.push_back(expression());
        consume(MB_Close, "Expect close symbol");
        consume(MB_Open, "Expect open symbol");
        n->children.push_back(statementBody());
        consume(MB_Close, "Expect close symbol");
    } while(match(MB_Open));

    return n;
}

Node* Parser::mathBranBigOperator(const NodeType& t){
    Node* initializer = nullptr;
    Node* final_value = nullptr;

    //Optional underscript
    if(match(MB_Open)){
        initializer = statementBody();
        consume(MB_Close, "Expect close symbol");

        //Optional overscript
        if(match(MB_Open)){
            final_value = expression();
            consume(MB_Close, "Expect close symbol");
        }
    }

    //Body
    Node* n = createNode(t, expression());
    if(initializer) n->children.push_back(initializer);
    if(final_value) n->children.push_back(final_value);

    return n;
}

Node* Parser::mathBranMatrix(){
    Node* n = createNode(MATRIX);

    consume(MB_Open, "Expect open symbol for matrix row count");
    consume(Number, "Expect matrix row count");
    bool success;
    ushort rows = source.midRef(previous.start, previous.length).toUShort(&success);
    if(!success || rows==0 || rows> 255) error("Matrix column count must be positive");
    n->children.push_back(createNode(NUMBER));
    consume(MB_Close, "Expect close symbol for matrix row count");

    consume(MB_Open, "Expect open symbol for matrix col count");
    consume(Number, "Expect matrix col count");
    ushort cols = source.midRef(previous.start, previous.length).toUShort(&success);
    if(!success || cols==0 || cols > 255) error("Matrix column count must be positive");
    n->children.push_back(createNode(NUMBER));
    consume(MB_Close, "Expect close symbol for matrix col count");

    for(ushort i = 0; i < rows*cols; i++){
        consume(MB_Open, "Expect open symbol for matrix arg " + QString::number(i+1));
        n->children.push_back(expression());
        consume(MB_Close, "Expect close symbol for matrix arg " + QString::number(i+1));
    }

    return n;
}

Node* Parser::mathBranFraction(){
    consume(MB_Open, "Expect open symbol");

    if(match(Doublestruck_d)) return mathBranDerivative(DERIVATIVE, Doublestruck_d);
    else if(match(Partial)) return mathBranDerivative(PARTIAL, Partial);

    Node* num = expression();
    consume(MB_Close, "Expect close symbol");
    consume(MB_Open, "Expect open symbol");
    Node* den = expression();
    consume(MB_Close, "Expect close symbol");

    return createNode(FRACTION, num, den);
}

Node* Parser::mathBranDerivative(const NodeType& type, const TokenType& deriv_token){
    Node* expr;
    Node* wrt;

    if(match(MB_Close)){ //Derivative comes after fraction: (d/dx) y
        consume(MB_Open, "Expect open symbol");
        consume(deriv_token, "Expect derivative symbol in denominator");
        wrt = idOnly();
        consume(MB_Close, "Expect close symbol");
        expr = primary();
    }else{ //Derivative is part of fraction: (dy/dx)
        expr = primary();
        consume(MB_Close, "Expect close symbol");
        consume(MB_Open, "Expect open symbol");
        consume(deriv_token, "Expect derivative symbol in denominator");
        wrt = idOnly();
        consume(MB_Close, "Expect close symbol");
    }

    return createNode(type, expr, wrt);
}

Node* Parser::mathBranUnderscriptedFunction(const NodeType& t){
    Node* n = mathBranUnary(t);
    n->children.push_back(expression());

    return n;
}

Node* Parser::mathBranLimit(){
    Node* n = mathBranBinary(LIMIT);
    n->children.push_back(expression());

    return n;
}

Node* Parser::mathBranSuperscript(Node* body, bool consume_on_start){
    if(consume_on_start) consume(MB_Open, "Expect open symbol");

    switch (current.type) {
        case Transpose: return mathBranExponentOp(body, TRANSPOSE);
        case Dagger: return mathBranExponentOp(body, DAGGER);
        case Conjunction: return mathBranExponentOp(body, WEDGE);
        case Disjunction: return mathBranExponentOp(body, VEE);
        case Plus: return mathBranExponentOp(body, INCREMENT);
        case Minus: return mathBranExponentOp(body, DECREMENT);
        default:
            Node* n = createNode(POWER, body, expression());
            consume(MB_Close, "Expect close symbol");
            return n;
    }
}

Node* Parser::mathBranExponentOp(Node* body, NodeType op){
    advance();
    consume(MB_Close, "Expect close symbol");
    return createNode(op, body);
}

Node* Parser::mathBranSubscript(Node* body, bool consume_on_start){
    if(consume_on_start) consume(MB_Open, "Expect open symbol");

    switch (current.type) {
        case Comma:{
            advance();
            Node* n = createNode(SUBSCRIPT_PARTIAL, body, expression());
            consume(MB_Close, "Expect close symbol");
            return n;
        }
        case Bar:{
            advance();
            Node* n = createNode(EVAL, body, statementBody());
            consume(MB_Close, "Expect close symbol");
            return n;
        }
        default:
            Node* n = createNode(SUBSCRIPT_ACCESS, body, expression());
            while(match(Comma)) n->children.push_back(expression());
            consume(MB_Close, "Expect close symbol");
            return n;
    }
}

Node* Parser::mathBranDualscript(Node* body){
    return mathBranSuperscript(mathBranSubscript(body));
    //Only bad case is sup-* ad ID: x_i^* should parse as x^*_i
}

Node* Parser::mathBranAccent(const NodeType& t){
    consume(MB_Open, "Expect MB open symbol");
    Node* n = createNode(t, idOnly());
    consume(MB_Close, "Expect MB close symbol");

    return n;
}

Node* Parser::mathBranAccentArrow(){
    consume(MB_Open, "Expect MB open symbol");
    Node* n = createNode(ACCENT_ARROW, idOnly());
    if(peek(Identifier)) n->children.push_back(idOnly());
    consume(MB_Close, "Expect MB close symbol");

    return n;
}

}
