//CODE-GEN FILE
//This file is generated from subfolder "meta".
//Changes to this file must be made in the meta project.

#include <QString>
#include <QHash>

enum TokenType {
	AccentArrow,
	AccentBar,
	AccentBreve,
	AccentDot,
	AccentDdot,
	AccentDddot,
	AccentHat,
	AccentTilde,
	At,
	Backslash,
	Bar,
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
	Differential,
	Disjunction,
	Divide,
	Dollar,
	DotProduct,
	DoubleBar,
	DoubleDagger,
	DoubleExclam,
	Dualscript,
	EmptySet,
	Equals,
	Euro,
	Exclam,
	Exists,
	ForAll,
	Forwardslash,
	Fraction,
	Greater,
	GreaterEqual,
	Hash,
	Identifier,
	In,
	Infimum,
	Infinity,
	Integer,
	Integral,
	IntegralUnicode,
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
	MinusPlus,
	Multiply,
	Nabla,
	Natural,
	Newline,
	Not,
	NotEqual,
	Number,
	ODot,
	OuterProduct,
	Partial,
	Percent,
	Period,
	PlanckConst,
	Plus,
	PlusMinus,
	PoundSterling,
	Prime,
	Proportional,
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
	SuspensionPoint,
	Tick,
	Tilde,
	Times,
	Transpose,
	UnderscriptedWord,
	Boolean,
	Irrational,
	Sphere,
	Whole,
};

static const QHash<TokenType, QString> token_names {
	{AccentArrow, "⁜→"},
	{AccentBar, "⁜ā"},
	{AccentBreve, "⁜ă"},
	{AccentDot, "⁜ȧ"},
	{AccentDdot, "⁜ä"},
	{AccentDddot, "⁜⋯"},
	{AccentHat, "⁜â"},
	{AccentTilde, "⁜ã"},
	{At, "@"},
	{Backslash, "\\"},
	{Bar, "|"},
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
	{Differential, "ⅆ"},
	{Disjunction, "∨"},
	{Divide, "÷"},
	{Dollar, "$"},
	{DotProduct, "⋅"},
	{DoubleBar, "‖"},
	{DoubleDagger, "‡"},
	{DoubleExclam, "‼️"},
	{Dualscript, "⁜Δ"},
	{EmptySet, "∅"},
	{Equals, "="},
	{Euro, "€"},
	{Exclam, "!"},
	{Exists, "∃"},
	{ForAll, "∀"},
	{Forwardslash, "/"},
	{Fraction, "⁜f"},
	{Greater, ">"},
	{GreaterEqual, "≥"},
	{Hash, "#"},
	{Identifier, "ID"},
	{In, "∈"},
	{Infimum, "inf"},
	{Infinity, "∞"},
	{Integer, "ℤ"},
	{Integral, "⁜∫"},
	{IntegralUnicode, "∫ "},
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
	{MinusPlus, "∓"},
	{Multiply, "*"},
	{Nabla, "∇ "},
	{Natural, "ℕ"},
	{Newline, "\\n"},
	{Not, "¬"},
	{NotEqual, "≠"},
	{Number, "num"},
	{ODot, "⨀"},
	{OuterProduct, "⊗"},
	{Partial, "∂"},
	{Percent, "%"},
	{Period, "."},
	{PlanckConst, "ℎ"},
	{Plus, "+"},
	{PlusMinus, "± "},
	{PoundSterling, "£"},
	{Prime, "ℙ"},
	{Proportional, "∝"},
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
	{SuspensionPoint, "…"},
	{Tick, "'"},
	{Tilde, "~"},
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

