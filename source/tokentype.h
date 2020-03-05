//CODE-GEN FILE
//This file is generated from subfolder "meta".
//Changes to this file must be made in the meta project.

#include <QString>
#include <QHash>

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
	Complex,
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
	Infimum,
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
	Lim,
	MapsTo,
	Matrix,
	Max,
	Min,
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
	Supremum,
	Tick,
	Tilde,
	TildeAccent,
	Times,
	Transpose,
	UnderscriptedWord,
	Boolean,
	Irrational,
	Sphere,
	Whole,
};

static const QHash<TokenType, QString> token_names {
	{ArrowAccent, "⁜→"},
	{At, "@"},
	{Backslash, "\\"},
	{Bar, "|"},
	{BarAccent, "⁜ā"},
	{BigProduct, "⁜∏"},
	{BigSum, "⁜∑"},
	{Binomial, "⁜b"},
	{Cap, "∩"},
	{Cos, "cos"},
	{Cup, "∪"},
	{Caret, "^"},
	{Cases, "⁜c"},
	{Comma, ","},
	{Complex, "ℂ "},
	{Colon, ":"},
	{Conjunction, "∧"},
	{Dagger, "†"},
	{DefEquals, "≔"},
	{Disjunction, "∨"},
	{Divide, "÷"},
	{Dollar, "$"},
	{DotAccent, "⁜ȧ"},
	{DotAccentDouble, "⁜ä"},
	{DotAccentTriple, "⁜⋯"},
	{DotProduct, "⋅"},
	{DoubleBar, "‖"},
	{DoubleDagger, "‡"},
	{DoubleExclam, "‼️"},
	{Dualscript, "⁜Δ"},
	{EmptySet, "∅"},
	{Equals, "="},
	{Exclam, "!"},
	{Exists, "∃"},
	{ForAll, "∀"},
	{Forwardslash, "/"},
	{Fraction, "⁜f"},
	{Greater, ">"},
	{GreaterEqual, "≥"},
	{Hash, "#"},
	{Hat, "⁜â"},
	{Identifier, "ID"},
	{In, "∈"},
	{Infimum, "inf"},
	{Infinity, "∞"},
	{Integer, "ℤ"},
	{Integral, "⁜∫"},
	{LeftAngle, "⟨"},
	{LeftArrow, "←"},
	{LeftBrace, "["},
	{LeftBracket, "{"},
	{LeftCeil, "⌈"},
	{LeftDoubleAngle, "⟪"},
	{LeftFloor, "⌊"},
	{LeftParen, "("},
	{Less, "<"},
	{LessEqual, "≤"},
	{Lim, "lim"},
	{MapsTo, "↦"},
	{Matrix, "⁜⊞"},
	{Max, "max"},
	{Min, "min"},
	{Minus, "-"},
	{Multiply, "*"},
	{Natural, "ℕ"},
	{Newline, "\\n"},
	{Not, "¬"},
	{NotEqual, "≠"},
	{Number, "num"},
	{Partial, "∂"},
	{Percent, "%"},
	{Period, "."},
	{PlanckConst, "ℎ"},
	{Plus, "+"},
	{Prime, "ℙ"},
	{Quaternion, "ℍ"},
	{Rational, "ℚ"},
	{Real, "ℝ"},
	{RightAngle, "⟩"},
	{RightArrow, "→"},
	{RightBrace, "]"},
	{RightBracket, "}"},
	{RightCeil, "⌉"},
	{RightDoubleAngle, "⟫"},
	{RightFloor, "⌋"},
	{RightParen, ")"},
	{Root, "⁜√"},
	{Semicolon, ";"},
	{Sin, "sin"},
	{SpecialClose, "⏵"},
	{SpecialEscape, "⁜"},
	{SpecialOpen, "⏴"},
	{Subscript, "⁜_"},
	{Superscript, "⁜^"},
	{Supremum, "sup"},
	{Tick, "'"},
	{Tilde, "~"},
	{TildeAccent, "⁜ã"},
	{Times, "×"},
	{Transpose, "⊤"},
	{UnderscriptedWord, "⁜w"},
};

static const QHash<QString, TokenType> keywords {
	{"cos", Cos},
	{"inf", Infimum},
	{"lim", Lim},
	{"max", Max},
	{"min", Min},
	{"sin", Sin},
	{"sup", Supremum},
};

static const QHash<QString, TokenType> UTF32_map {
	{"𝔹", Boolean},
	{"𝕁", Irrational},
	{"𝕊", Sphere},
	{"𝕎", Whole},
};
