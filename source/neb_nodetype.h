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
    TEST_NOT_IN,
    TEST_NOTEQUAL,
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
    DERIVATIVE,
    DIVERGENCE,
    DIVIDE,
    DOT,
    DOUBLE_INTEGRAL,
    EVAL,
    FACTORIAL,
    FLOOR,
    FRACTION,
    GRADIENT,
    INFIMUM,
    INFTY,
    INNER_PRODUCT,
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
    VEE,
    WEDGE,
    SEQUENCE_ENUMERATED,
    ALEPH,
    BOOLEANS,
    COMPLEX_NUMS,
    EMPTY_SET,
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
    ACCENT_ARROW,
    ACCENT_BAR,
    ACCENT_BREVE,
    ACCENT_DDDOT,
    ACCENT_DDOT,
    ACCENT_DOT,
    ACCENT_HAT,
    ACCENT_TILDE,
    BACKSLASH,
    BETH,
    CALL,
    CARDINALITY,
    CASES,
    CONDITIONAL_PROBABLITY,
    DAGGER,
    DECREMENT,
    ERROR,
    FORWARDSLASH,
    FUN_SIGNATURE,
    GROUP_BRACKET,
    GROUP_PAREN,
    IDENTIFIER,
    IMPLICIT_MULTIPLY,
    IN,
    INCREMENT,
    INTEGRAL,
    LIMIT,
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
    UNARY_MINUS,
    ALGORITHM,
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
static const QString labels[138] { \
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
    "∉?", \
    "≠?", \
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
    "ⅆy/ⅆx", \
    "∇⋅", \
    "÷", \
    "⋅", \
    "∬", \
    "⁜_|", \
    "!", \
    "⌊ ⌋", \
    "⁜f", \
    "∇", \
    "inf", \
    "∞", \
    "⟨ | ⟩", \
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
    "∨", \
    "∧", \
    "(,,,)", \
    "ℵ", \
    "𝔹", \
    "ℂ ", \
    "∅", \
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
    "⁜→", \
    "⁜ā", \
    "⁜ă", \
    "⁜⋯", \
    "⁜ä", \
    "⁜ȧ", \
    "⁜â", \
    "⁜ã", \
    "\\\\", \
    "ℶ", \
    "f( )", \
    "#", \
    "⁜c", \
    "P( | )", \
    "†", \
    "x⁻", \
    "err", \
    "/", \
    "f:×→", \
    "[ ]", \
    "( )", \
    "id: ", \
    "*imp", \
    "∈", \
    "x⁺", \
    "∫", \
    "lim", \
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
    "-", \
    "alg", \
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
    CT_Set,
};

#define NEB_DECLARE_COARSETYPE_LABELS \
static const QString coarsetype_labels[6] { \
    "U", \
    "B", \
    "F", \
    "N", \
    "SEQ", \
    "S", \
};

#define NEB_DECLARE_COARSETYPE_INIT \
static CoarseType initCoarseType(const NodeType& type){ \
    if(type >= ACCENT_ARROW) return CT_Untyped; \
    else if(type >= ALEPH) return CT_Set; \
    else if(type >= SEQUENCE_ENUMERATED) return CT_Sequence; \
    else if(type >= ABS) return CT_Numeric; \
    else return CT_Boolean; \
}

#define NEB_DECLARE_IS_EXPR \
static bool isExpr(const NodeType& type){ \
    return type < ALGORITHM; \
}

#define NEB_NUM_NODETYPES 138

}

#endif
