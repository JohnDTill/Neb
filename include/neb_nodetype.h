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
    INTEGER_LITERAL,
    DECIMAL_LITERAL,
    ODOT,
    OUTER_PRODUCT,
    PARTIAL,
    POWER,
    PRODUCT,
    ROOT,
    SQRT,
    SLICE,
    SUBSCRIPT_PARTIAL,
    SUMMATION,
    TRANSPOSE,
    TRIPLE_INTEGRAL,
    UINT_PARSED,
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
static const QString labels[143] { \
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
    "", \
    "⨀", \
    "⊗", \
    "∂y/∂x", \
    "^", \
    "∏", \
    "⁜∛", \
    "⁜√", \
    ":", \
    "∂", \
    "Σ", \
    "⊤", \
    "∭", \
    "", \
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

}

#endif
