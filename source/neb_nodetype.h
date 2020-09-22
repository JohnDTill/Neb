//CODE-GEN FILE
//This file is generated from subfolder "meta".
//Changes to this file must be made in the meta project.

#ifndef NEB_NODETYPE_H
#define NEB_NODETYPE_H

namespace Neb{

enum NodeType{
    FALSE,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    LOGICAL_AND,
    LOGICAL_NOT,
    LOGICAL_OR,
    TEST_EQUALITY,
    TEST_GREATER,
    TEST_GREATER_EQUAL,
    TEST_IN,
    TEST_LESS,
    TEST_LESS_EQUAL,
    TEST_NOT_EQUAL,
    TEST_NOT_IN,
    TRUE,
    ABS,
    ADDITION,
    BINOMIAL,
    CEIL,
    CLOSED_SURFACE_INTEGRAL,
    CLOSED_VOLUME_INTEGRAL,
    CONTOUR_INTEGRAL,
    CROSS,
    CURL,
    CURRENCY_DOLLARS,
    CURRENCY_EUROS,
    CURRENCY_POUNDS,
    DAGGER,
    DECREMENT,
    DERIVATIVE,
    DIVERGENCE,
    DIVIDE,
    DOT,
    DOUBLE_INTEGRAL,
    FACTORIAL,
    FLOOR,
    FORWARDSLASH,
    FRACTION,
    GRADIENT,
    IMPLICIT_MULTIPLY,
    INCREMENT,
    INFIMUM,
    INFTY,
    INNER_PRODUCT,
    INTEGRAL,
    LIMIT,
    MATRIX,
    MAX,
    MIN,
    MODULUS,
    MULTIPLICATION,
    NORM,
    NUMBER,
    ODOT,
    OUTER_PRODUCT,
    PARTIAL,
    POWER,
    PRODUCT,
    ROOT,
    SQRT,
    SUBSCRIPT_PARTIAL,
    SUMMATION,
    TRANSPOSE,
    TRIPLE_INTEGRAL,
    UNARY_MINUS,
    VEE,
    WEDGE,
    SEQUENCE_ENUMERATED,
    BOOLEANS,
    COMPLEX_NUMS,
    INTEGERS,
    INTERSECTION,
    INTERVAL_CLOSE_CLOSE,
    INTERVAL_CLOSE_OPEN,
    INTERVAL_INTEGER,
    INTERVAL_OPEN_CLOSE,
    INTERVAL_OPEN_OPEN,
    NARY_INTERSECTION,
    NARY_UNION,
    NARY_UNIONPLUS,
    NATURALS,
    NEGATIVE_RATIONALS,
    NEGATIVE_REALS,
    POSITIVE_RATIONALS,
    POSITIVE_REALS,
    QUATERNIONS,
    RATIONALS,
    REALS,
    SET_BUILDER,
    SET_ENUMERATED,
    UNION,
    STRING,
    ACCENT_ARROW,
    ACCENT_BAR,
    ACCENT_BREVE,
    ACCENT_DDDOT,
    ACCENT_DDOT,
    ACCENT_DOT,
    ACCENT_HAT,
    ACCENT_TILDE,
    ALEPH,
    BACKSLASH,
    BETH,
    CALL,
    CARDINALITY,
    CASES,
    CONDITIONAL_PROBABLITY,
    EMPTY_SET,
    ERROR,
    EVAL,
    FUN_SIGNATURE,
    GROUP_BRACKET,
    GROUP_PAREN,
    IDENTIFIER,
    IN,
    MINUS_PLUS_BINARY,
    MINUS_PLUS_UNARY,
    PLUS_MINUS_BINARY,
    PLUS_MINUS_UNARY,
    PROPORTIONAL,
    SET_LITERAL_DIMENSIONS,
    SUBSCRIPT_ACCESS,
    SUBTRACTION,
    SUPREMUM,
    TICK_DERIVATIVE,
    ALGORITHM,
    ASSIGN,
    BLOCK,
    DEFINE_EQUALS,
    EQUAL,
    EXPR_STMT,
    FUN_DEF,
    IF,
    NOT_EQUAL,
    NOT_IN,
    PRINT,
    RETURN,
    WHILE,
};

#define NEB_DECLARE_NODE_LABELS \
static const QString labels[140] { \
    "false", \
    ">", \
    "≥", \
    "<", \
    "≤", \
    "∧", \
    "¬", \
    "∨", \
    "=?", \
    ">?", \
    "≥?", \
    "∈?", \
    "<?", \
    "≤?", \
    "≠?", \
    "∉?", \
    "true", \
    "| |", \
    "+", \
    "⁜b", \
    "⌈ ⌉", \
    "∯", \
    "∰", \
    "∮", \
    "×", \
    "∇×", \
    "$", \
    "€", \
    "£", \
    "†", \
    "x⁻", \
    "ⅆy/ⅆx", \
    "∇⋅", \
    "÷", \
    "⋅", \
    "∬", \
    "!", \
    "⌊ ⌋", \
    "/", \
    "⁜f", \
    "∇", \
    "*imp", \
    "x⁺", \
    "inf", \
    "∞", \
    "⟨ | ⟩", \
    "∫", \
    "lim", \
    "⁜⊞", \
    "max", \
    "min", \
    "%", \
    "*", \
    "‖ ‖", \
    "", \
    "⨀", \
    "⊗", \
    "∂y/∂x", \
    "^", \
    "∏", \
    "⁜∛", \
    "⁜√", \
    "∂", \
    "Σ", \
    "⊤", \
    "∭", \
    "-", \
    "∨", \
    "∧", \
    "(,,,)", \
    "𝔹", \
    "ℂ ", \
    "ℤ", \
    "∩", \
    "[,]", \
    "[,)", \
    "⟦,⟧", \
    "(,]", \
    "(,)", \
    "⁜⋂", \
    "⁜⋃", \
    "⁜⨄", \
    "ℕ", \
    "ℚ⁻", \
    "ℝ⁻", \
    "ℚ⁺", \
    "ℝ⁺", \
    "ℍ", \
    "ℚ", \
    "ℝ", \
    "{ | }", \
    "{,,,}", \
    "∪", \
    "", \
    "⁜→", \
    "⁜ā", \
    "⁜ă", \
    "⁜⋯", \
    "⁜ä", \
    "⁜ȧ", \
    "⁜â", \
    "⁜ã", \
    "ℵ", \
    "\\\\", \
    "ℶ", \
    "f( )", \
    "#", \
    "⁜c", \
    "P( | )", \
    "∅", \
    "err", \
    "⁜_|", \
    "f:×→", \
    "[ ]", \
    "( )", \
    "id: ", \
    "∈", \
    "∓", \
    "∓", \
    "±", \
    "±", \
    "∝", \
    "dim", \
    "xᵢ", \
    "-", \
    "sup", \
    "'", \
    "alg", \
    "←", \
    "block", \
    "≔", \
    "=", \
    "expr\\nstmt", \
    "f:()↦", \
    "if", \
    "≠", \
    "∉", \
    "print", \
    "return", \
    "while", \
};

enum CoarseType{
    CT_Untyped,
    CT_Boolean,
    CT_Function,
    CT_Numeric,
    CT_Sequence,
    CT_Set_Boolean,
    CT_Set_Numeric,
    CT_String,
    CT_Void,
};

#define NEB_DECLARE_COARSETYPE_LABELS \
static const QString coarsetype_labels[8] { \
    "U", \
    "B", \
    "F", \
    "N", \
    "SEQ", \
    "SET", \
    "STR", \
    "V", \
};

#define NEB_DECLARE_COARSETYPE_INIT \
static CoarseType initCoarseType(const NodeType& type){ \
    if(type >= ACCENT_ARROW) return CT_Untyped; \
    else if(type >= STRING) return CT_String; \
    else if(type >= COMPLEX_NUMS) return CT_Set_Numeric; \
    else if(type >= BOOLEANS) return CT_Set_Boolean; \
    else if(type >= SEQUENCE_ENUMERATED) return CT_Sequence; \
    else if(type >= ABS) return CT_Numeric; \
    else return CT_Boolean; \
}

#define NEB_DECLARE_IS_EXPR \
static bool isExpr(const NodeType& type){ \
    return type < ALGORITHM; \
}

#define NEB_NUM_NODETYPES 140

#define NEB_HOMOGENOUS_BOOLEAN_ARGS \
    case Neb::LOGICAL_AND: \
    case Neb::LOGICAL_NOT: \
    case Neb::LOGICAL_OR: \

#define NEB_HOMOGENOUS_NUMERIC_ARGS \
    case Neb::GREATER: \
    case Neb::GREATER_EQUAL: \
    case Neb::LESS: \
    case Neb::LESS_EQUAL: \
    case Neb::TEST_GREATER: \
    case Neb::TEST_GREATER_EQUAL: \
    case Neb::TEST_LESS: \
    case Neb::TEST_LESS_EQUAL: \
    case Neb::ABS: \
    case Neb::ADDITION: \
    case Neb::BINOMIAL: \
    case Neb::CEIL: \
    case Neb::CROSS: \
    case Neb::CURL: \
    case Neb::CURRENCY_DOLLARS: \
    case Neb::CURRENCY_EUROS: \
    case Neb::CURRENCY_POUNDS: \
    case Neb::DAGGER: \
    case Neb::DECREMENT: \
    case Neb::DIVERGENCE: \
    case Neb::DIVIDE: \
    case Neb::DOT: \
    case Neb::FACTORIAL: \
    case Neb::FLOOR: \
    case Neb::FORWARDSLASH: \
    case Neb::FRACTION: \
    case Neb::INCREMENT: \
    case Neb::INNER_PRODUCT: \
    case Neb::LIMIT: \
    case Neb::MATRIX: \
    case Neb::MODULUS: \
    case Neb::MULTIPLICATION: \
    case Neb::NORM: \
    case Neb::ODOT: \
    case Neb::OUTER_PRODUCT: \
    case Neb::ROOT: \
    case Neb::SQRT: \
    case Neb::SUMMATION: \
    case Neb::TRANSPOSE: \
    case Neb::UNARY_MINUS: \
    case Neb::VEE: \
    case Neb::WEDGE: \
    case Neb::INTERVAL_CLOSE_CLOSE: \
    case Neb::INTERVAL_CLOSE_OPEN: \
    case Neb::INTERVAL_INTEGER: \
    case Neb::INTERVAL_OPEN_CLOSE: \
    case Neb::INTERVAL_OPEN_OPEN: \
    case Neb::SET_ENUMERATED: \
    case Neb::PROPORTIONAL: \

#define NEB_HOMOGENOUS_SET_NUMERIC_ARGS \
    case Neb::INTERSECTION: \
    case Neb::NARY_INTERSECTION: \
    case Neb::NARY_UNION: \
    case Neb::NARY_UNIONPLUS: \
    case Neb::UNION: \
    case Neb::CARDINALITY: \

}

#endif
