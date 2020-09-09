//CODE-GEN FILE
//This file is generated from subfolder "meta".
//Changes to this file must be made in the meta project.

#ifndef NEB_NODETYPE_H
#define NEB_NODETYPE_H

namespace Neb{

enum NodeType{
	ABS,
	ACCENT_ARROW,
	ACCENT_BAR,
	ACCENT_BREVE,
	ACCENT_DDDOT,
	ACCENT_DDOT,
	ACCENT_DOT,
	ACCENT_HAT,
	ACCENT_TILDE,
	ADDITION,
	ALEPH,
	ALGORITHM,
	BACKSLASH,
	BETH,
	BINOMIAL,
	BLOCK,
	BOOLEANS,
	CALL,
	CASES,
	CEIL,
	COMPLEX_NUMS,
	CONDITIONAL_PROBABLITY,
	CONTOUR_INTEGRAL,
	CROSS,
	CURL,
	CURRENCY_DOLLARS,
	CURRENCY_EUROS,
	CURRENCY_POUNDS,
	DAGGER,
	DECREMENT,
	DEFINE_EQUALS,
	DERIVATIVE,
	DIVERGENCE,
	DIVIDE,
	DOT,
	CARDINALITY,
	CLOSED_SURFACE_INTEGRAL,
	DOUBLE_INTEGRAL,
	EMPTY_SET,
	EQUAL,
	ERROR,
	EVAL,
	EXPR_STMT,
	FACTORIAL,
	FALSE,
	FLOOR,
	FORWARDSLASH,
	FRACTION,
	FUN_DEF,
	FUN_SIGNATURE,
	GRADIENT,
	GREATER,
	GREATER_EQUAL,
	GROUP_BRACKET,
	GROUP_PAREN,
	IDENTIFIER,
	IF,
	IMPLICIT_MULTIPLY,
	IN,
	INCREMENT,
	INFIMUM,
	INFTY,
	INNER_PRODUCT,
	INTEGERS,
	INTEGRAL,
	INTERSECTION,
	LESS,
	LESS_EQUAL,
	LIMIT,
	LOGICAL_AND,
	LOGICAL_NOT,
	LOGICAL_OR,
	MATRIX,
	MAX,
	MIN,
	MINUS_PLUS_BINARY,
	MINUS_PLUS_UNARY,
	MODULUS,
	MULTIPLICATION,
	NARY_INTERSECTION,
	NARY_UNION,
	NARY_UNIONPLUS,
	NATURALS,
	NEGATIVE_RATIONALS,
	NEGATIVE_REALS,
	NORM,
	NOT_EQUAL,
	NOT_IN,
	NUMBER,
	ODOT,
	OUTER_PRODUCT,
	PARTIAL,
	PLUS_MINUS_BINARY,
	PLUS_MINUS_UNARY,
	POSITIVE_RATIONALS,
	POSITIVE_REALS,
	POWER,
	PRINT,
	PRODUCT,
	PROPORTIONAL,
	QUATERNIONS,
	INTERVAL_INTEGER,
	INTERVAL_CLOSE_CLOSE,
	INTERVAL_CLOSE_OPEN,
	INTERVAL_OPEN_CLOSE,
	INTERVAL_OPEN_OPEN,
	RATIONALS,
	REALS,
	ROOT,
	SET_BUILDER,
	SET_ENUMERATED,
	SET_LITERAL_DIMENSIONS,
	SEQUENCE_ENUMERATED,
	SQRT,
	SUBSCRIPT_ACCESS,
	SUBSCRIPT_PARTIAL,
	SUBTRACTION,
	SUMMATION,
	SUPREMUM,
	TICK_DERIVATIVE,
	TRANSPOSE,
	TRUE,
	CLOSED_VOLUME_INTEGRAL,
	TRIPLE_INTEGRAL,
	UNARY_MINUS,
	UNION,
	VEE,
	WEDGE,
	WHILE,
};

#define NEB_DECLARE_NODE_LABELS \
static const QString labels[129] { \
	"| |", \
	"⁜→", \
	"⁜ā", \
	"⁜ă", \
	"⁜⋯", \
	"⁜ä", \
	"⁜ȧ", \
	"⁜â", \
	"⁜ã", \
	"+", \
	"ℵ", \
	"alg", \
	"\\\\", \
	"ℶ", \
	"⁜b", \
	"block", \
	"𝔹", \
	"f( )", \
	"⁜c", \
	"⌈ ⌉", \
	"ℂ ", \
	"P( | )", \
	"∮", \
	"×", \
	"∇×", \
	"$", \
	"€", \
	"£", \
	"†", \
	"x⁻", \
	"≔", \
	"ⅆy/ⅆx", \
	"∇⋅", \
	"÷", \
	"⋅", \
	"#", \
	"∯", \
	"∬", \
	"∅", \
	"=", \
	"err", \
	"⁜_|", \
	"expr\\nstmt", \
	"!", \
	"false", \
	"⌊ ⌋", \
	"/", \
	"⁜f", \
	"f:()↦", \
	"f:×→", \
	"∇", \
	">", \
	"≥", \
	"[ ]", \
	"( )", \
	"id: ", \
	"if", \
	"*imp", \
	"∈", \
	"x⁺", \
	"inf", \
	"∞", \
	"⟨ | ⟩", \
	"ℤ", \
	"∫", \
	"∩", \
	"<", \
	"≤", \
	"lim", \
	"∧", \
	"¬", \
	"∨", \
	"⁜⊞", \
	"max", \
	"min", \
	"∓", \
	"∓", \
	"%", \
	"*", \
	"⁜⋂", \
	"⁜⋃", \
	"⁜⨄", \
	"ℕ", \
	"ℚ⁻", \
	"ℝ⁻", \
	"‖ ‖", \
	"≠", \
	"∉", \
	"", \
	"⨀", \
	"⊗", \
	"∂y/∂x", \
	"±", \
	"±", \
	"ℚ⁺", \
	"ℝ⁺", \
	"^", \
	"print", \
	"∏", \
	"∝", \
	"ℍ", \
	"⟦,⟧", \
	"[,]", \
	"[,)", \
	"(,]", \
	"(,)", \
	"ℚ", \
	"ℝ", \
	"⁜∛", \
	"{ | }", \
	"{,,,}", \
	"dim", \
	"(,,,)", \
	"⁜√", \
	"xᵢ", \
	"∂", \
	"-", \
	"Σ", \
	"sup", \
	"'", \
	"⊤", \
	"true", \
	"∰", \
	"∭", \
	"-", \
	"∪", \
	"∨", \
	"∧", \
	"while", \
};

#define NEB_NUM_NODETYPES 129

}

#endif
