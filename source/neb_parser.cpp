#include "neb_parser.h"

#include "neb_scanner.h"
#include <QDebug>

namespace Neb {

Parser::Parser(const QString& source)
    : source(source),
      scanner(*new Scanner(source, err_msg, err_start, err_end)) {
    advance();
}

Parser::~Parser(){
    delete &scanner;
}

Node* Parser::parseStatement(TokenType surrounding_terminator, bool nested){
    if(!ok() && !nested){
        scanToRecoveryPoint();
        err_msg.clear();
    }

    while(match(Newline));
    if(match(EndOfFile))
        return nested ? error("Reached end of file without closing scope") : nullptr;

    Node* body;
    bool must_terminate = true;

    switch (current.type) {
        case Algorithm: must_terminate = false; body = algorithm(surrounding_terminator); break;
        case If: must_terminate = false; body = ifStatement(surrounding_terminator); break;
        case Print: body = printStatement(); break;
        case Return: body = returnStatement(surrounding_terminator); break;
        case While: must_terminate = false; body = whileStatement(surrounding_terminator); break;
        default: body = mathStatement();
    }

    if(must_terminate) consume<3>({Newline, Comma, surrounding_terminator}, "Expect statement terminator");

    if(current.type == Error){
        Node::deletePostorder(body);
        return new Node(ERROR, current.start, current.start + current.length);
    }else{
        return body;
    }
}

std::vector<Node*> Parser::parseAll(){
    std::vector<Node*> stmts;
    while(Node* stmt = parseStatement()) stmts.push_back(stmt);

    return stmts;
}

Node* Parser::createNodeFromPrevToken(const NodeType& type){
    Node* n = new Node(type, previous.start, previous.start + previous.length);
    return n;
}

Node* Parser::createNodeFromPrevToken(const NodeType& type, Node* child){
    Node* n = createNodeFromPrevToken(type);
    n->children.push_back(child);

    return n;
}

Node* Parser::createNodeFromPrevToken(const NodeType& type, Node* lhs, Node* rhs){
    Node* n = createNodeFromPrevToken(type);
    n->children.push_back(lhs);
    n->children.push_back(rhs);

    return n;
}

Node* Parser::createNodeFromPrevToken(uint number){
    Node* n = createNodeFromPrevToken(NUMBER);
    n->data.text = new QString();
    n->data.text->setNum(number);

    return n;
}

template<int a>
Node* Parser::createNode(const NodeType& type, const std::array<Node*,a>& args){
    Node* n = createNodeFromPrevToken(type);
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

Node* Parser::error(const QString& message){
    return error(message, current);
}

Node* Parser::error(const QString& message, const Token& t){
    if(err_msg.isEmpty()){
        err_msg = message;
        err_start = t.start;
        err_end = t.start + t.length;
    }
    return new Node(ERROR, t.start, t.start + t.length);
}

void Parser::advance(){
    if(current.type == Error){
        previous = current;
        return;
    }

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

Node* Parser::printStatement(){
    advance();
    consume(LeftParen, "Expect '(' in Print stmt.");
    Node* print_stmt = createNodeFromPrevToken(PRINT, expression());
    consume(RightParen, "Expect ')' to close Print stmt.");

    return print_stmt;
}

Node* Parser::whileStatement(TokenType surrounding_terminator){
    advance();
    consume(LeftParen, "Expect '(' in Print stmt.");
    Node* condition = boolExpression();
    consume(RightParen, "Expect ')' to close Print stmt.");

    return createNodeFromPrevToken(WHILE,
        condition,
        peek(LeftBracket) ? blockStatement(true) : parseStatement(surrounding_terminator, true));
}

Node* Parser::ifStatement(TokenType surrounding_terminator){
    advance();
    consume(LeftParen, "Expect '(' in IF stmt.");
    Node* condition = boolExpression();
    consume(RightParen, "Expect ')' to close IF stmt.");

    Node* if_stmt = createNodeFromPrevToken(IF,
        condition,
        peek(LeftBracket) ? blockStatement(true) : parseStatement(surrounding_terminator, true));

    if(match(Else)) if_stmt->children.push_back(
        peek(LeftBracket) ? blockStatement(true) : parseStatement(surrounding_terminator, true));

    return if_stmt;
}

Node* Parser::blockStatement(bool nested){
    consume(LeftBracket, "Expect '{' in block statement.");
    Node* body = createNodeFromPrevToken(BLOCK);
    while(match(Newline));
    while(!match(RightBracket) && err_msg.isEmpty()){
        body->children.push_back(parseStatement(RightBracket, nested));
        while(match(Newline));
    }

    return body;
}

Node* Parser::returnStatement(TokenType surrounding_terminator){
    advance();

    if(peek<3>({Newline, Comma, surrounding_terminator}))
        return createNodeFromPrevToken(RETURN);
    else return createNodeFromPrevToken(RETURN, boolExpression());
}

Node* Parser::algorithm(TokenType surrounding_terminator){
    advance();

    Node* alg = createNodeFromPrevToken(ALGORITHM, idOnly());

    consume(LeftParen, "Expect '(' after algorithm name");
    if(!peek(RightParen)){
        do {
            match(Newline);
            alg->children.push_back(mathStatement());
        } while(match(Comma));
    }
    consume(RightParen, "Expect ')' to close algorithm parameter list");

    if(peek(LeftBracket)){
        alg->children.push_back( blockStatement(true) );
    }else{
        Node* s = parseStatement(surrounding_terminator, true);
        if(s) alg->children.push_back(s);
        else{
            Node::deletePostorder(alg);
            return error("Algorithm requires a body");
        }
    }

    return alg;
}

Node* Parser::mathStatement(){
    Node* n = expression();

    switch (current.type) {
        case LeftArrow: advance(); return createNodeFromPrevToken(ASSIGN, n, expression());
        case Equals: advance(); return equality(n);
        case Less: advance(); return less(n);
        case LessEqual: advance(); return less(n);
        case Greater: advance(); return greater(n);
        case GreaterEqual: advance(); return greater(n);
        case In: advance(); return createNodeFromPrevToken(IN, n, expression());
        case DefEquals: advance(); return createNodeFromPrevToken(DEFINE_EQUALS, n, expression());
        case NotEqual: advance(); return createNodeFromPrevToken(NOT_EQUAL, n, expression());
        case Proportional: advance(); return createNodeFromPrevToken(PROPORTIONAL, n, expression());
        case Colon: advance(); return functionDefinition(n);
        default: return createNodeFromPrevToken(EXPR_STMT, n);
    }
}

Node* Parser::equality(Node* n){
    match(Newline);
    Node* equality_stmt = createNodeFromPrevToken(EQUAL, n, expression());
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
        n = createNodeFromPrevToken(inclusive ? LESS_EQUAL : LESS, n, expression());
    } while(match<2>({Less, LessEqual}));

    return n;
}

Node* Parser::greater(Node* n){
    do{
        bool inclusive = previous.type == GreaterEqual;
        match(Newline);
        n = createNodeFromPrevToken(inclusive ? GREATER_EQUAL : GREATER, n, expression());
    } while(match<2>({Greater, GreaterEqual}));

    return n;
}

Node* Parser::functionDefinition(Node* n){
    if(n->type != IDENTIFIER){
        error("Expected identifier in function definition");
        Node::deletePostorder(n);
        return createNodeFromPrevToken(Error);
    }

    if(match(LeftParen)){
        //example - p : (x,y) ↦ x^y
        n = createNodeFromPrevToken(FUN_DEF, n);

        consume(Identifier, "Pure function must have an input");
        n->children.push_back(createNodeFromPrevToken(IDENTIFIER));

        do {
            match(Newline);
            consume(Identifier, "Expect identifier");
            n->children.push_back(createNodeFromPrevToken(IDENTIFIER));
        } while(match(Comma));

        consume(RightParen, "Expect ')' to close function parameter list");

        consume(MapsTo, "Expect '↦' after parameter list");
        n->children.push_back(expression());
        return n;
    }else{
        //example - f : ℝ × ℤ → ℝ
        n = createNodeFromPrevToken(FUN_SIGNATURE, n);

        parsing_dimensions = true;
        do{
            Node* input_type = expression();
            //DO THIS - validate that input_type has type "set"
            n->children.push_back(input_type);
        } while(match(Times));
        parsing_dimensions = false;

        consume(RightArrow, "Expect '→' after function signature inputs");

        Node* output_type = expression();
        //DO THIS - validate that output_type has type "set"
        n->children.push_back(output_type);

        return n;
    }
}

typedef Node* (Parser::*Rule)();

Node* Parser::expression(){
    return disjunction<&Parser::conjunction<&Parser::addition> >();
}

template<Rule next>
Node* Parser::disjunction(){
    Node* expr = (this->*next)();
    if(match(Disjunction)){
        match(Newline);
        expr = createNodeFromPrevToken(LOGICAL_OR, expr, (this->*next)());
        while(match(Disjunction)){
            match(Newline);
            expr->children.push_back((this->*next)());
        }
    }

    return expr;
}

template<Rule next>
Node* Parser::conjunction(){
    Node* expr = (this->*next)();
    if(match(Conjunction)){
        match(Newline);
        expr = createNodeFromPrevToken(LOGICAL_AND, expr, (this->*next)());
        while(match(Conjunction)){
            match(Newline);
            expr->children.push_back((this->*next)());
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
            case Plus: expr = createNodeFromPrevToken(ADDITION, expr, rhs); break;
            case PlusMinus: expr = createNodeFromPrevToken(PLUS_MINUS_BINARY, expr, rhs); break;
            case Minus: expr = createNodeFromPrevToken(SUBTRACTION, expr, rhs); break;
            case MinusPlus: expr = createNodeFromPrevToken(MINUS_PLUS_BINARY, expr, rhs); break;
            case Cap: expr = createNodeFromPrevToken(INTERSECTION, expr, rhs); break;
            case Cup: expr = createNodeFromPrevToken(UNION, expr, rhs); break;
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
            case Backslash: expr = createNodeFromPrevToken(BACKSLASH, expr, rhs); break;
            case Divide: expr = createNodeFromPrevToken(DIVIDE, expr, rhs); break;
            case DotProduct: expr = createNodeFromPrevToken(DOT, expr, rhs); break;
            case Forwardslash: expr = createNodeFromPrevToken(FORWARDSLASH, expr, rhs); break;
            case Multiply: expr = createNodeFromPrevToken(MULTIPLICATION, expr, rhs); break;
            case ODot: expr = createNodeFromPrevToken(ODOT, expr, rhs); break;
            case OuterProduct: expr = createNodeFromPrevToken(OUTER_PRODUCT, expr, rhs); break;
            case Percent: expr = createNodeFromPrevToken(MODULUS, expr, rhs); break;
            case Times: expr = createNodeFromPrevToken(CROSS, expr, rhs); break;
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
        expr = createNodeFromPrevToken(IMPLICIT_MULTIPLY, expr, rightUnary());

        while(peek<NEB_NUM_IMPLICIT_MULT>(NEB_IMPLICIT_MULT_MACRO_EXPANSION))
            expr->children.push_back(rightUnary());
    }

    return expr;
}

Node* Parser::leftUnary(){
    switch (current.type) {
        case Minus: advance(); checkGap(); return createNodeFromPrevToken(UNARY_MINUS, rightUnary());
        case Not: advance(); checkGap(); return createNodeFromPrevToken(LOGICAL_NOT, rightUnary());
        case PlusMinus: advance(); checkGap(); return createNodeFromPrevToken(PLUS_MINUS_UNARY, rightUnary());
        case MinusPlus: advance(); checkGap(); return createNodeFromPrevToken(MINUS_PLUS_UNARY, rightUnary());
        case Pound: advance(); return createNodeFromPrevToken(CARDINALITY, script());
        default: return rightUnary();
    }
}

Node* Parser::rightUnary(){
    Node* expr = exponent();
    while(peek<2>( {Exclam, Tick} )){
        if(match(Exclam)){
            Node* n = createNodeFromPrevToken(FACTORIAL, expr);
            n->data.number = 0;
            while(peek(Exclam)){
                if(n->data.number == 255){
                    Node::deletePostorder(n);
                    return error("Cannot have more than 255 factorials");
                }
                n->data.number++;

                checkGap();
                advance();
            }

            return n;
        }else{
            consume(Tick, "Expect '");
            Node* n = createNodeFromPrevToken(TICK_DERIVATIVE, expr);
            n->data.number = 0;
            while(peek(Tick)){
                if(n->data.number == 255){
                    Node::deletePostorder(n);
                    return error("Cannot have more than 255 tick derivatives");
                }
                n->data.number++;

                checkGap();
                advance();
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
        n = createNodeFromPrevToken(POWER, n, implicitMultiplication());
    }
    return n;
}

Node* Parser::script(){
    Node* n = primary();

    switch (current.type) {
        case MB_Superscript: advance(); return mathBranSuperscript(n);
        case MB_Subscript: advance(); return mathBranSubscript(n);
        case MB_Dualscript: advance(); return mathBranDualscript(n);
        case SuperscriptLeftParen: return createNodeFromPrevToken(POWER, n, unicodeSuperscriptGrouping());
        case SuperscriptNumber: return createNodeFromPrevToken(POWER, n, unicodeSuperscriptExpression());
        case SuperscriptIdentifier: return createNodeFromPrevToken(POWER, n, unicodeSuperscriptExpression());
        case SuperscriptPlus: return createNodeFromPrevToken(INCREMENT, n);
        case SuperscriptMinus: return createNodeFromPrevToken(DECREMENT, n);
        case SuperscriptWedge: return createNodeFromPrevToken(WEDGE, n);
        case SuperscriptVee: return createNodeFromPrevToken(VEE, n);
        case SubscriptLeftParen: return createNodeFromPrevToken(SUBSCRIPT_ACCESS, n, unicodeSubscriptGrouping());
        case SubscriptNumber: return createNodeFromPrevToken(SUBSCRIPT_ACCESS, n, unicodeSubscriptExpression());
        case SubscriptIdentifier: return createNodeFromPrevToken(SUBSCRIPT_ACCESS, n, unicodeSubscriptExpression());
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
        case Aleph: return cardinalNumber(ALEPH);
        case Beth: return cardinalNumber(BETH);

        //Value literal
        case EmptySet: return createNodeFromPrevToken(EMPTY_SET);
        case Infinity: return createNodeFromPrevToken(INFTY);
        case Number: return number();
        case True: return createNodeFromPrevToken(TRUE);
        case False: return createNodeFromPrevToken(FALSE);
        case String: return string();

        //Grouping
        case LeftCeil: return grouping(CEIL, RightCeil);
        case LeftFloor: return grouping(FLOOR, RightFloor);
        case Bar: return grouping(ABS, Bar);
        case DoubleBar: return grouping(NORM, DoubleBar);

        //Set literal
        case Doublestruck_B: return setLiteral<BOOLEANS>();
        case Doublestruck_C: return setLiteral<COMPLEX_NUMS>();
        case Doublestruck_H: return createNodeFromPrevToken(QUATERNIONS);
        case Doublestruck_N: return setLiteral<NATURALS>();
        case Doublestruck_Q: return setLiteral<RATIONALS>();
        case Doublestruck_R: return setLiteral<REALS>();
        case Doublestruck_Z: return setLiteral<INTEGERS>();

        //Currency
        case Dollar: return createNodeFromPrevToken(CURRENCY_DOLLARS, primary());
        case Euro: return createNodeFromPrevToken(CURRENCY_EUROS, primary());
        case PoundSterling: return createNodeFromPrevToken(CURRENCY_POUNDS, primary());

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
            return error("Expected a primary expression, got \"" +
                         source.mid(previous.start, previous.length) +
                         '"',
                         previous);
    }
}

Node* Parser::boolExpression(){
    return disjunction<&Parser::conjunction<&Parser::boolEquality> >();
}

Node* Parser::boolEquality(){
    Node* n = addition();

    switch (current.type) {
        case Equals: advance(); return createNodeFromPrevToken(TEST_EQUALITY, n, addition());
        case NotEqual: advance(); return createNodeFromPrevToken(TEST_NOT_EQUAL, n, addition());
        case Less: advance(); return createNodeFromPrevToken(TEST_LESS, n, addition());
        case Greater: advance(); return createNodeFromPrevToken(TEST_GREATER, n, addition());
        case LessEqual: advance(); return createNodeFromPrevToken(TEST_LESS_EQUAL, n, addition());
        case GreaterEqual: advance(); return createNodeFromPrevToken(TEST_GREATER_EQUAL, n, addition());
        case In: advance(); return createNodeFromPrevToken(TEST_IN, n, addition());
        case NotIn: advance(); return createNodeFromPrevToken(TEST_NOT_IN, n, addition());
        default: return n;
    }
}

Node* Parser::number(){
    Node* n = createNodeFromPrevToken(NUMBER);
    n->data.text = new QString(source.mid(previous.start, previous.length));
    return n;
}

Node* Parser::string(){
    Node* n = createNodeFromPrevToken(STRING);
    n->data.text = new QString(source.mid(previous.start, previous.length));
    return n;
}

Node* Parser::grouping(const NodeType& t, const TokenType& close){
    Node* n = boolExpression();
    consume(close, "Expect grouping close symbol.");

    return createNodeFromPrevToken(t, n);
}

Node* Parser::idOnly(){
    consume(Identifier, "Expect identifier");
    return idStart(true);
}

Node* Parser::idStart(bool id_only){
    Node* n = createNodeFromPrevToken(IDENTIFIER);
    n->data.text = new QString(source.mid(previous.start, previous.length));
    if(!id_only && match(LeftParen)) return call(n);
    else if(match(MB_Superscript)){
        consume(MB_Open, "Expect open symbol");
        if(match(Multiply)){
            n->data.text->append("⁜^⏴*⏵");
            consume(MB_Close, "Expect close symbol");
        }else if(id_only){
            error("Superscript not allowed in this context");
        }else{
            return mathBranSuperscript(n, false);
        }
    }else if(match(MB_Subscript)){
        consume(MB_Open, "Expect open symbol");
        if(match(Multiply)){
            n->data.text->append("⁜_⏴*⏵");
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
    if(match(RightParen)) return createNodeFromPrevToken(CALL, id);

    match(Newline);
    Node* first_arg = boolExpression();

    if(match(Bar)){
        if(id->type != IDENTIFIER || *id->data.text != "P")
            error("Probablity function can only be represented by P");

        id->type = CONDITIONAL_PROBABLITY;
        id->children.push_back(first_arg);
        id->children.push_back(expression());
        consume(RightParen, "Expect ) after conditional probability");

        return id;
    }else{
        Node* n = createNodeFromPrevToken(CALL, id, first_arg);

        while(match(Comma)){
            match(Newline);
            n->children.push_back(boolExpression());
        }

        consume(RightParen, "Expect ')' to close function call");

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
                return createNodeFromPrevToken(POSITIVE_REALS);
            }else if(t == RATIONALS){
                return createNodeFromPrevToken(POSITIVE_RATIONALS);
            }else{
                error("Set does not support positive script");
                return createNodeFromPrevToken(ERROR);
            }
        case SuperscriptMinus:
            checkGap();
            advance();
            if(t == REALS){
                return createNodeFromPrevToken(NEGATIVE_REALS);
            }else if(t == RATIONALS){
                return createNodeFromPrevToken(NEGATIVE_RATIONALS);
            }else{
                error("Set does not support negative script");
                return createNodeFromPrevToken(ERROR);
            }
        case SuperscriptIdentifier:
        case SuperscriptLeftParen:
        case SuperscriptNumber:
            return createNodeFromPrevToken(SET_LITERAL_DIMENSIONS, createNodeFromPrevToken(t), unicodeSuperscriptExpression());
        default:
            return createNodeFromPrevToken(t);
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
                return createNodeFromPrevToken(POSITIVE_REALS);
            }else{
                error("Set does not support positive script");
                return createNodeFromPrevToken(ERROR);
            }
        case Minus:
            advance();
            consume(MB_Close, "Expect MB close");
            if(t == REALS){
                return createNodeFromPrevToken(NEGATIVE_REALS);
            }else{
                error("Set does not support negative script");
                return createNodeFromPrevToken(ERROR);
            }
        default:
            parsing_dimensions = true;
            Node* n = createNodeFromPrevToken(SET_LITERAL_DIMENSIONS, createNodeFromPrevToken(t), addition());
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
        if(previous.type == SuperscriptMinus) n = createNodeFromPrevToken(SUBTRACTION, n, unicodeSuperscriptImplicitMult());
        else if(n->type == ADDITION) n->children.push_back(unicodeSuperscriptGrouping());
        else n = createNodeFromPrevToken(ADDITION, n, unicodeSuperscriptGrouping());
    }

    return n;
}

Node* Parser::unicodeSuperscriptImplicitMult(){
    Node* n = unicodeSuperscriptGrouping();
    if(current.type == SuperscriptLeftParen || current.type == SuperscriptIdentifier){
        checkGap();
        n = createNodeFromPrevToken(IMPLICIT_MULTIPLY, n, unicodeSuperscriptGrouping());
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
        return createNodeFromPrevToken(GROUP_PAREN, n);
    }

    return unicodeSuperscriptTerminal();
}

Node* Parser::unicodeSuperscriptTerminal(){
    if(match(SuperscriptNumber)){
        return createNodeFromPrevToken(NUMBER);
    }else{
        consume(SuperscriptIdentifier, "Expected identifier");
        Node* n = createNodeFromPrevToken(IDENTIFIER);

        return n; //Ignore function calls for now
    }
}

template<bool check_gap>
Node* Parser::unicodeSubscriptExpression(){
    if(check_gap) checkGap();

    Node* n = unicodeSubscriptImplicitMult();
    while(match(SubscriptPlus) || match(SubscriptMinus)){
        if(previous.type == SubscriptMinus) n = createNodeFromPrevToken(SUBTRACTION, n, unicodeSubscriptImplicitMult());
        else if(n->type == ADDITION) n->children.push_back(unicodeSubscriptGrouping());
        else n = createNodeFromPrevToken(ADDITION, n, unicodeSubscriptGrouping());
    }

    return n;
}

Node* Parser::unicodeSubscriptImplicitMult(){
    Node* n = unicodeSubscriptGrouping();
    if(current.type == SubscriptLeftParen || current.type == SubscriptIdentifier){
        checkGap();
        n = createNodeFromPrevToken(IMPLICIT_MULTIPLY, n, unicodeSubscriptGrouping());
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
        return createNodeFromPrevToken(GROUP_PAREN, n);
    }

    return unicodeSubscriptTerminal();
}

Node* Parser::unicodeSubscriptTerminal(){
    if(match(SubscriptNumber)){
        return createNodeFromPrevToken(NUMBER);
    }else{
        consume(SubscriptIdentifier, "Expected identifier");
        Node* n = createNodeFromPrevToken(IDENTIFIER);

        return n; //Ignore function calls for now
    }
}

Node* Parser::nablaStart(){
    if(match(DotProduct)) return createNodeFromPrevToken(DIVERGENCE, rightUnary());
    else if(match(Times)) return createNodeFromPrevToken(CURL, rightUnary());
    else return createNodeFromPrevToken(GRADIENT, rightUnary());
}

Node* Parser::parenStart(){
    Node* expr = boolExpression();

    if(match(Comma)){ //Exclusive start of range, e.g. x ∈ (0, 1]
        Node* range_end = boolExpression();

        if(match(RightParen)){
            return createNodeFromPrevToken(INTERVAL_OPEN_OPEN, expr, range_end);
        }else if(match(RightBrace)){
            return createNodeFromPrevToken(INTERVAL_OPEN_CLOSE, expr, range_end);
        }else{
            consume(Comma, "Invalid interval or sequence");
            Node* n = createNodeFromPrevToken(SEQUENCE_ENUMERATED, expr, range_end);
            do{
                n->children.push_back(boolExpression());
            } while(match(Comma));
            consume(RightParen, "Expect ')' at end of sequence");

            return n;
        }
    }else{
        consume(RightParen, "Expect )");
        return createNodeFromPrevToken(GROUP_PAREN, expr);
    }
}

Node* Parser::braceStart(){
    Node* expr = boolExpression();

    if(match(Comma)){ //Inclusive start of range, e.g. x ∈ [0, 1)
        Node* range_end = boolExpression();

        if(match(RightParen)){
            return createNodeFromPrevToken(INTERVAL_CLOSE_OPEN, expr, range_end);
        }else if(match(RightBrace)){
            return createNodeFromPrevToken(INTERVAL_CLOSE_CLOSE, expr, range_end);
        }else if(match(Semicolon) || match(Newline)){
            return braceMatrix(createNode<3>(MATRIX, {createNodeFromPrevToken(2), expr, range_end}), 2, previous.type);
        }else{
            consume(Comma, "Expect end of interval or matrix delimiter");
            Node* n = createNodeFromPrevToken(MATRIX, expr, range_end);
            uint cols = 2;
            do{
                n->children.push_back(boolExpression());
                cols++;
            } while(match(Comma));

            n->children.insert(n->children.begin(), createNodeFromPrevToken(cols));
            if(match(Semicolon) || match(Newline)){
                return braceMatrix(n, cols, previous.type);
            }else{
                consume(RightBrace, "Expect ']' at end of matrix");
                return n;
            }
        }
    }else if(match(Semicolon) || match(Newline)){
        return braceMatrix(createNodeFromPrevToken(MATRIX, createNodeFromPrevToken(1), expr), 1, previous.type);
    }else{
        consume(RightBrace, "Expect ]");
        return createNodeFromPrevToken(GROUP_BRACKET, expr);
    }
}

Node* Parser::braceMatrix(Node* n, uint col_count, TokenType row_delimiter){
    do{
        if(row_delimiter == Semicolon) match(Newline);
        n->children.push_back(boolExpression());
        for(uint i = col_count-1; i > 0; i--){
            consume(Comma, "Expect ',' between matrix elements");
            n->children.push_back(boolExpression());
        }
    } while(match(row_delimiter));
    consume(RightBrace, "Expect ']' at end of matrix");

    return n;
}

Node* Parser::doubleBraceStart(){
    Node* start = boolExpression();
    consume(Comma, "Expect interval delimiter");
    Node* end = boolExpression();
    consume(RightDoubleBrace, "Expect '⟧' to close integer interval");

    return createNodeFromPrevToken(INTERVAL_INTEGER, start, end);
}

Node* Parser::setStart(){
    if(match(RightBracket)) return createNodeFromPrevToken(EMPTY_SET);

    Node* expr = boolExpression();
    if(match(RightBracket)) return createNodeFromPrevToken(SET_ENUMERATED, expr); //1 member set
    else if(current.type == Comma){ //n-member enumerated set
        expr = createNodeFromPrevToken(SET_ENUMERATED, expr);
        while(match(Comma)){
            match(Newline);
            expr->children.push_back( boolExpression() );
        }
        consume(RightBracket, "Expect } to close enumerated set");

        return expr;
    }else{ //Set selector
        expr = match(In) ?
               createNodeFromPrevToken(SET_BUILDER, createNodeFromPrevToken(IN, expr, expression())) : // {x ∈ ℝ : ...}
               createNodeFromPrevToken(SET_BUILDER, expr); // {x : ...}

        consume<2>({Colon, Bar}, "Expect set builder");

        expr->children.push_back(mathStatement());
        while(match(Comma)) expr->children.push_back(mathStatement());
        consume(RightBracket, "Expect } to close set builder");

        return expr;
    }
}

Node* Parser::innerProduct(){
    Node* expr = expression();
    if(match(Comma)){
        expr = createNodeFromPrevToken(INNER_PRODUCT, expr, expression());
    }else{
        consume(Bar, "Expect '|' or ',' delimiter between inner product args");
        expr = createNodeFromPrevToken(INNER_PRODUCT, expression(), expr);
    }
    consume(RightAngle, "Expect '⟩' to close inner product");

    return expr;
}

Node* Parser::integral(const NodeType& type){
    #ifdef INTEGRAL_LOW_PRECEDENCE
    Node* n = createNode(type, implicitMultiplication());
    #else
    Node* n = createNodeFromPrevToken(type, expression());
    #endif
    consume(Doublestruck_d, "Expect differential 'ⅆ' after integral kernal");
    n->children.push_back(idOnly());

    return n;
}

Node* Parser::cardinalNumber(NodeType type){
    if(peek<3>({SubscriptNumber, SubscriptLeftParen, SubscriptIdentifier}))
        return createNodeFromPrevToken(type, unicodeSubscriptExpression());

    checkGap();
    consume(MB_Subscript, "Expect cardinal number to have a subscript");
    consume(MB_Open, "Expect open");
    Node* degree = expression();
    consume(MB_Close, "Expect close");

    return createNodeFromPrevToken(type, degree);
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

    return createNodeFromPrevToken(t, lhs, rhs);
}

Node* Parser::mathBranRoot(){
    consume(MB_Open, "Expect MB open symbol");
    Node* body = expression();
    consume(MB_Close, "Expect MB close symbol");
    if(match(MB_Open)){
        Node* script = expression();
        consume(MB_Close, "Expect MB close symbol");
        return createNodeFromPrevToken(ROOT, body, script);
    }else{
        return createNodeFromPrevToken(SQRT, body);
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
    Node* n = createNodeFromPrevToken(CASES);

    consume(MB_Open, "Expect open symbol");
    do{
        n->children.push_back(expression());
        consume(MB_Close, "Expect close symbol");
        consume(MB_Open, "Expect open symbol");
        n->children.push_back(boolExpression());
        consume(MB_Close, "Expect close symbol");
    } while(match(MB_Open));

    return n;
}

Node* Parser::mathBranBigOperator(const NodeType& t){
    Node* initializer = nullptr;
    Node* final_value = nullptr;

    //Optional underscript
    if(match(MB_Open)){
        initializer = mathStatement();
        consume(MB_Close, "Expect close symbol");

        //Optional overscript
        if(match(MB_Open)){
            final_value = expression();
            consume(MB_Close, "Expect close symbol");
        }
    }

    //Body
    Node* n = createNodeFromPrevToken(t, expression());
    if(initializer) n->children.push_back(initializer);
    if(final_value) n->children.push_back(final_value);

    return n;
}

Node* Parser::mathBranMatrix(){
    Node* n = createNodeFromPrevToken(MATRIX);

    consume(MB_Open, "Expect open symbol for matrix row count");
    consume(Number, "Expect matrix row count");
    bool success;
    ushort rows = source.midRef(previous.start, previous.length).toUShort(&success);
    if(!success || rows==0 || rows> 255) error("Matrix column count must be positive");
    consume(MB_Close, "Expect close symbol for matrix row count");

    consume(MB_Open, "Expect open symbol for matrix col count");
    consume(Number, "Expect matrix col count");
    ushort cols = source.midRef(previous.start, previous.length).toUShort(&success);
    if(!success || cols==0 || cols > 255) error("Matrix column count must be positive");
    Node* dims = createNodeFromPrevToken(UINT_PARSED);
    dims->data.number = rows | (cols << 8);
    n->children.push_back(dims);
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

    return createNodeFromPrevToken(FRACTION, num, den);
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

    return createNodeFromPrevToken(type, expr, wrt);
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
        case Minus: if(peek(MB_Close)) return mathBranExponentOp(body, DECREMENT);
        default:
            Node* n = createNodeFromPrevToken(POWER, body, expression());
            consume(MB_Close, "Expect close symbol");
            return n;
    }
}

Node* Parser::mathBranExponentOp(Node* body, NodeType op){
    advance();
    consume(MB_Close, "Expect close symbol");
    return createNodeFromPrevToken(op, body);
}

Node* Parser::mathBranSubscript(Node* body, bool consume_on_start){
    if(consume_on_start) consume(MB_Open, "Expect open symbol");

    switch (current.type) {
        case Comma:{
            advance();
            Node* n = createNodeFromPrevToken(SUBSCRIPT_PARTIAL, body, expression());
            consume(MB_Close, "Expect close symbol");
            return n;
        }
        case Bar:{
            advance();
            Node* n = createNodeFromPrevToken(EVAL, body, mathStatement());
            consume(MB_Close, "Expect close symbol");
            return n;
        }
        default:
            Node* n = createNodeFromPrevToken(SUBSCRIPT_ACCESS, body, expression());
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
    Node* n = createNodeFromPrevToken(t, idOnly());
    consume(MB_Close, "Expect MB close symbol");

    return n;
}

Node* Parser::mathBranAccentArrow(){
    consume(MB_Open, "Expect MB open symbol");
    Node* n = createNodeFromPrevToken(ACCENT_ARROW, idOnly());
    if(peek(Identifier)) n->children.push_back(idOnly());
    consume(MB_Close, "Expect MB close symbol");

    return n;
}

}
