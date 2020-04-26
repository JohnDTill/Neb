//CODE-GEN FILE
//This file is generated from subfolder "meta".
//Changes to this file must be made in the meta project.

#ifndef NEB_TOKENTYPE_H
#define NEB_TOKENTYPE_H

#include <QString>
#include <QHash>

enum TokenType{
	Aleph,
	At,
	Backslash,
	Bar,
	Because,
	Beth,
	Cap,
	Caret,
	Colon,
	Comma,
	Conjunction,
	ContourIntegral,
	Cos,
	Cup,
	Dagger,
	DefEquals,
	Degree,
	Disjunction,
	Divide,
	Dollar,
	DotProduct,
	DoubleBar,
	DoubleDagger,
	DoubleExclam,
	DoubleGreater,
	DoubleLess,
	Doublestruck_B,
	Doublestruck_C,
	Doublestruck_d,
	Doublestruck_H,
	Doublestruck_i,
	Doublestruck_J,
	Doublestruck_N,
	Doublestruck_P,
	Doublestruck_Q,
	Doublestruck_R,
	Doublestruck_S,
	Doublestruck_W,
	Doublestruck_Z,
	EmptySet,
	EndOfFile,
	Equals,
	Equivalent,
	Error,
	Euro,
	Exclam,
	Exists,
	For,
	ForAll,
	Forwardslash,
	Greater,
	GreaterEqual,
	Hamiltonian,
	Identifier,
	In,
	Infinity,
	Integral,
	Laplace,
	LeftAngle,
	LeftArrow,
	LeftBrace,
	LeftBracket,
	LeftCeil,
	LeftDoubleAngle,
	LeftDoubleBrace,
	LeftFloor,
	LeftParen,
	Less,
	LessEqual,
	Log,
	MapsTo,
	MB_AccentArrow,
	MB_AccentBar,
	MB_AccentBreve,
	MB_AccentDddot,
	MB_AccentDdot,
	MB_AccentDot,
	MB_AccentHat,
	MB_AccentTilde,
	MB_Binomial,
	MB_Cases,
	MB_Close,
	MB_ClosedSurfaceIntegral,
	MB_ClosedVolumeIntegral,
	MB_ContourIntegral,
	MB_Coproduct,
	MB_DoubleIntegral,
	MB_Dualscript,
	MB_EvalBar,
	MB_Fraction,
	MB_GroupingBar,
	MB_GroupingBracket,
	MB_GroupingCeil,
	MB_GroupingDoubleBars,
	MB_GroupingFloor,
	MB_GroupingParen,
	MB_Infimum,
	MB_Integral,
	MB_Intersection,
	MB_Lim,
	MB_Matrix,
	MB_Max,
	MB_Min,
	MB_Open,
	MB_Product,
	MB_Root,
	MB_Subscript,
	MB_Sum,
	MB_Superscript,
	MB_Supremum,
	MB_TripleIntegral,
	MB_Union,
	MB_UnionPlus,
	Minus,
	MinusPlus,
	Multiply,
	Nabla,
	NaturalLog,
	Newline,
	Ni,
	Not,
	NotEqual,
	NotExist,
	NotGreater,
	NotIn,
	NotLess,
	NotNi,
	NotSubset,
	NotSubsetEq,
	Number,
	ODot,
	OuterProduct,
	Partial,
	Percent,
	Period,
	Perp,
	PlanckConst,
	Plus,
	PlusMinus,
	Pound,
	PoundSterling,
	Prec,
	Proportional,
	Qed,
	RightAngle,
	RightArrow,
	RightArrowDouble,
	RightBrace,
	RightBracket,
	RightCeil,
	RightDoubleAngle,
	RightDoubleBrace,
	RightFloor,
	RightParen,
	Semicolon,
	Sin,
	SubscriptIdentifier,
	SubscriptLeftParen,
	SubscriptMinus,
	SubscriptNumber,
	SubscriptPlus,
	SubscriptRightParen,
	Subset,
	SubsetEq,
	Succ,
	SuperscriptIdentifier,
	SuperscriptLeftParen,
	SuperscriptMinus,
	SuperscriptNumber,
	SuperscriptPlus,
	SuperscriptRightParen,
	SuperscriptVee,
	SuperscriptWedge,
	SuspensionPoint,
	Tangent,
	Therefore,
	Thermod,
	Tick,
	Tilde,
	Times,
	Transpose,
	TripleGreater,
	TripleLess,
	Where,
	While,
};

#ifndef Neb_NDebug
static const QHash<TokenType, QString> token_labels {
	{Aleph, "ℵ"},
	{At, "@"},
	{Backslash, "\\"},
	{Bar, "|"},
	{Because, "∵"},
	{Beth, "ℶ"},
	{Cap, "∩"},
	{Caret, "^"},
	{Colon, ":"},
	{Comma, ","},
	{Conjunction, "∧"},
	{ContourIntegral, "∮"},
	{Cos, "cos"},
	{Cup, "∪"},
	{Dagger, "†"},
	{DefEquals, "≔"},
	{Degree, "°"},
	{Disjunction, "∨"},
	{Divide, "÷"},
	{Dollar, "$"},
	{DotProduct, "⋅"},
	{DoubleBar, "‖"},
	{DoubleDagger, "‡"},
	{DoubleExclam, "‼️"},
	{DoubleGreater, "≫"},
	{DoubleLess, "≪"},
	{Doublestruck_B, "𝔹"},
	{Doublestruck_C, "ℂ"},
	{Doublestruck_d, "ⅆ"},
	{Doublestruck_H, "ℍ"},
	{Doublestruck_i, "ⅈ"},
	{Doublestruck_J, "𝕁"},
	{Doublestruck_N, "ℕ"},
	{Doublestruck_P, "ℙ"},
	{Doublestruck_Q, "ℚ"},
	{Doublestruck_R, "ℝ"},
	{Doublestruck_S, "𝕊"},
	{Doublestruck_W, "𝕎"},
	{Doublestruck_Z, "ℤ"},
	{EmptySet, "∅"},
	{EndOfFile, "EndOfFile"},
	{Equals, "="},
	{Equivalent, "≡"},
	{Error, "ERR"},
	{Euro, "€"},
	{Exclam, "!"},
	{Exists, "∃"},
	{For, "for"},
	{ForAll, "∀"},
	{Forwardslash, "/"},
	{Greater, ">"},
	{GreaterEqual, "≥"},
	{Hamiltonian, "ℋ"},
	{Identifier, "ID"},
	{In, "∈"},
	{Infinity, "∞"},
	{Integral, "∫"},
	{Laplace, "ℒ"},
	{LeftAngle, "⟨"},
	{LeftArrow, "←"},
	{LeftBrace, "["},
	{LeftBracket, "{"},
	{LeftCeil, "⌈"},
	{LeftDoubleAngle, "⟪"},
	{LeftDoubleBrace, "⟦"},
	{LeftFloor, "⌊"},
	{LeftParen, "("},
	{Less, "<"},
	{LessEqual, "≤"},
	{Log, "log"},
	{MapsTo, "↦"},
	{MB_AccentArrow, "⁜→"},
	{MB_AccentBar, "⁜ā"},
	{MB_AccentBreve, "⁜ă"},
	{MB_AccentDddot, "⁜⋯"},
	{MB_AccentDdot, "⁜ä"},
	{MB_AccentDot, "⁜ȧ"},
	{MB_AccentHat, "⁜â"},
	{MB_AccentTilde, "⁜ã"},
	{MB_Binomial, "⁜b"},
	{MB_Cases, "⁜c"},
	{MB_Close, "⏵"},
	{MB_ClosedSurfaceIntegral, "⁜∯"},
	{MB_ClosedVolumeIntegral, "⁜∰"},
	{MB_ContourIntegral, "⁜∮"},
	{MB_Coproduct, "⁜∐"},
	{MB_DoubleIntegral, "⁜∬"},
	{MB_Dualscript, "⁜Δ"},
	{MB_EvalBar, "⁜┊"},
	{MB_Fraction, "⁜f"},
	{MB_GroupingBar, "⁜|"},
	{MB_GroupingBracket, "⁜["},
	{MB_GroupingCeil, "⁜⌈"},
	{MB_GroupingDoubleBars, "⁜‖"},
	{MB_GroupingFloor, "⁜⌊"},
	{MB_GroupingParen, "⁜("},
	{MB_Infimum, "⁜↘"},
	{MB_Integral, "⁜∫"},
	{MB_Intersection, "⁜⋂"},
	{MB_Lim, "⁜l"},
	{MB_Matrix, "⁜⊞"},
	{MB_Max, "⁜↑"},
	{MB_Min, "⁜↓"},
	{MB_Open, "⏴"},
	{MB_Product, "⁜∏"},
	{MB_Root, "⁜√"},
	{MB_Subscript, "⁜_"},
	{MB_Sum, "⁜∑"},
	{MB_Superscript, "⁜^"},
	{MB_Supremum, "⁜↗"},
	{MB_TripleIntegral, "⁜∭"},
	{MB_Union, "⁜⋃"},
	{MB_UnionPlus, "⁜⨄"},
	{Minus, "-"},
	{MinusPlus, "∓"},
	{Multiply, "*"},
	{Nabla, "∇"},
	{NaturalLog, "ln"},
	{Newline, "\n"},
	{Ni, "∋"},
	{Not, "¬"},
	{NotEqual, "≠"},
	{NotExist, "∄"},
	{NotGreater, "≯"},
	{NotIn, "∉"},
	{NotLess, "≮"},
	{NotNi, "∌"},
	{NotSubset, "⊄"},
	{NotSubsetEq, "⊈"},
	{Number, ""},
	{ODot, "⨀"},
	{OuterProduct, "⊗"},
	{Partial, "∂"},
	{Percent, "%"},
	{Period, "."},
	{Perp, "⊥"},
	{PlanckConst, "ℎ"},
	{Plus, "+"},
	{PlusMinus, "±"},
	{Pound, "#"},
	{PoundSterling, "£"},
	{Prec, "≺"},
	{Proportional, "∝"},
	{Qed, "■"},
	{RightAngle, "⟩"},
	{RightArrow, "→"},
	{RightArrowDouble, "⇒"},
	{RightBrace, "]"},
	{RightBracket, "}"},
	{RightCeil, "⌉"},
	{RightDoubleAngle, "⟫"},
	{RightDoubleBrace, "⟧"},
	{RightFloor, "⌋"},
	{RightParen, ")"},
	{Semicolon, ";"},
	{Sin, "sin"},
	{SubscriptIdentifier, "sub-"},
	{SubscriptLeftParen, "₍"},
	{SubscriptMinus, "₋"},
	{SubscriptNumber, "sub-"},
	{SubscriptPlus, "₊"},
	{SubscriptRightParen, "₎"},
	{Subset, "⊂"},
	{SubsetEq, "⊆"},
	{Succ, "≻"},
	{SuperscriptIdentifier, "sup-"},
	{SuperscriptLeftParen, "⁽"},
	{SuperscriptMinus, "⁻"},
	{SuperscriptNumber, "sup-"},
	{SuperscriptPlus, "⁺"},
	{SuperscriptRightParen, "⁾"},
	{SuperscriptVee, "ⱽ"},
	{SuperscriptWedge, "ᶺ"},
	{SuspensionPoint, "…"},
	{Tangent, "tan"},
	{Therefore, "∴"},
	{Thermod, "⧧"},
	{Tick, "'"},
	{Tilde, "~"},
	{Times, "×"},
	{Transpose, "⊤"},
	{TripleGreater, "⋙"},
	{TripleLess, "⋘"},
	{Where, "where"},
	{While, "while"},
};
#endif

static const QHash<QString, TokenType> keywords {
	{"cos", Cos},
	{"for", For},
	{"log", Log},
	{"ln", NaturalLog},
	{"sin", Sin},
	{"tan", Tangent},
	{"where", Where},
	{"while", While},
};

#define NEB_ONE_TO_ONE_CASES_MACRO_EXPANSION\
	 case 8501: return createToken(Aleph);\
	 case 64: return createToken(At);\
	 case 92: return createToken(Backslash);\
	 case 124: return createToken(Bar);\
	 case 8757: return createToken(Because);\
	 case 8502: return createToken(Beth);\
	 case 8745: return createToken(Cap);\
	 case 94: return createToken(Caret);\
	 case 58: return createToken(Colon);\
	 case 44: return createToken(Comma);\
	 case 8743: return createToken(Conjunction);\
	 case 8750: return createToken(ContourIntegral);\
	 case 8746: return createToken(Cup);\
	 case 8224: return createToken(Dagger);\
	 case 8788: return createToken(DefEquals);\
	 case 176: return createToken(Degree);\
	 case 8744: return createToken(Disjunction);\
	 case 247: return createToken(Divide);\
	 case 36: return createToken(Dollar);\
	 case 8901: return createToken(DotProduct);\
	 case 8214: return createToken(DoubleBar);\
	 case 8225: return createToken(DoubleDagger);\
	 case 8252: return createToken(DoubleExclam);\
	 case 8811: return createToken(DoubleGreater);\
	 case 8810: return createToken(DoubleLess);\
	 case 8450: return createToken(Doublestruck_C);\
	 case 8518: return createToken(Doublestruck_d);\
	 case 8461: return createToken(Doublestruck_H);\
	 case 8520: return createToken(Doublestruck_i);\
	 case 8469: return createToken(Doublestruck_N);\
	 case 8473: return createToken(Doublestruck_P);\
	 case 8474: return createToken(Doublestruck_Q);\
	 case 8477: return createToken(Doublestruck_R);\
	 case 8484: return createToken(Doublestruck_Z);\
	 case 8709: return createToken(EmptySet);\
	 case 61: return createToken(Equals);\
	 case 8801: return createToken(Equivalent);\
	 case 8364: return createToken(Euro);\
	 case 33: return createToken(Exclam);\
	 case 8707: return createToken(Exists);\
	 case 8704: return createToken(ForAll);\
	 case 47: return createToken(Forwardslash);\
	 case 62: return createToken(Greater);\
	 case 8805: return createToken(GreaterEqual);\
	 case 8459: return createToken(Hamiltonian);\
	 case 8712: return createToken(In);\
	 case 8734: return createToken(Infinity);\
	 case 8747: return createToken(Integral);\
	 case 8466: return createToken(Laplace);\
	 case 10216: return createToken(LeftAngle);\
	 case 8592: return createToken(LeftArrow);\
	 case 91: return createToken(LeftBrace);\
	 case 123: return createToken(LeftBracket);\
	 case 8968: return createToken(LeftCeil);\
	 case 10218: return createToken(LeftDoubleAngle);\
	 case 10214: return createToken(LeftDoubleBrace);\
	 case 8970: return createToken(LeftFloor);\
	 case 40: return createToken(LeftParen);\
	 case 60: return createToken(Less);\
	 case 8804: return createToken(LessEqual);\
	 case 8614: return createToken(MapsTo);\
	 case 9205: return createToken(MB_Close);\
	 case 9204: return createToken(MB_Open);\
	 case 45: return createToken(Minus);\
	 case 8723: return createToken(MinusPlus);\
	 case 42: return createToken(Multiply);\
	 case 8711: return createToken(Nabla);\
	 case 10: return createToken(Newline);\
	 case 8715: return createToken(Ni);\
	 case 172: return createToken(Not);\
	 case 8800: return createToken(NotEqual);\
	 case 8708: return createToken(NotExist);\
	 case 8815: return createToken(NotGreater);\
	 case 8713: return createToken(NotIn);\
	 case 8814: return createToken(NotLess);\
	 case 8716: return createToken(NotNi);\
	 case 8836: return createToken(NotSubset);\
	 case 8840: return createToken(NotSubsetEq);\
	 case 10752: return createToken(ODot);\
	 case 8855: return createToken(OuterProduct);\
	 case 8706: return createToken(Partial);\
	 case 37: return createToken(Percent);\
	 case 46: return createToken(Period);\
	 case 8869: return createToken(Perp);\
	 case 8462: return createToken(PlanckConst);\
	 case 43: return createToken(Plus);\
	 case 177: return createToken(PlusMinus);\
	 case 35: return createToken(Pound);\
	 case 163: return createToken(PoundSterling);\
	 case 8826: return createToken(Prec);\
	 case 8733: return createToken(Proportional);\
	 case 9632: return createToken(Qed);\
	 case 10217: return createToken(RightAngle);\
	 case 8594: return createToken(RightArrow);\
	 case 8658: return createToken(RightArrowDouble);\
	 case 93: return createToken(RightBrace);\
	 case 125: return createToken(RightBracket);\
	 case 8969: return createToken(RightCeil);\
	 case 10219: return createToken(RightDoubleAngle);\
	 case 10215: return createToken(RightDoubleBrace);\
	 case 8971: return createToken(RightFloor);\
	 case 41: return createToken(RightParen);\
	 case 59: return createToken(Semicolon);\
	 case 8333: return createToken(SubscriptLeftParen);\
	 case 8331: return createToken(SubscriptMinus);\
	 case 8330: return createToken(SubscriptPlus);\
	 case 8334: return createToken(SubscriptRightParen);\
	 case 8834: return createToken(Subset);\
	 case 8838: return createToken(SubsetEq);\
	 case 8827: return createToken(Succ);\
	 case 8317: return createToken(SuperscriptLeftParen);\
	 case 8315: return createToken(SuperscriptMinus);\
	 case 8314: return createToken(SuperscriptPlus);\
	 case 8318: return createToken(SuperscriptRightParen);\
	 case 11389: return createToken(SuperscriptVee);\
	 case 7610: return createToken(SuperscriptWedge);\
	 case 8230: return createToken(SuspensionPoint);\
	 case 8756: return createToken(Therefore);\
	 case 10727: return createToken(Thermod);\
	 case 39: return createToken(Tick);\
	 case 126: return createToken(Tilde);\
	 case 215: return createToken(Times);\
	 case 8868: return createToken(Transpose);\
	 case 8921: return createToken(TripleGreater);\
	 case 8920: return createToken(TripleLess);

#define NEB_BIG_ONE_TO_ONE_CASES_MACRO_EXPANSION\
	 case 56633: return Token(Doublestruck_B, curr-2, 2);\
	 case 56641: return Token(Doublestruck_J, curr-2, 2);\
	 case 56650: return Token(Doublestruck_S, curr-2, 2);\
	 case 56654: return Token(Doublestruck_W, curr-2, 2);

#define NEB_MATHBRAN_CASES_MACRO_EXPANSION\
	 case 8594: return Token(MB_AccentArrow, curr-2, 2);\
	 case 257: return Token(MB_AccentBar, curr-2, 2);\
	 case 259: return Token(MB_AccentBreve, curr-2, 2);\
	 case 8943: return Token(MB_AccentDddot, curr-2, 2);\
	 case 228: return Token(MB_AccentDdot, curr-2, 2);\
	 case 551: return Token(MB_AccentDot, curr-2, 2);\
	 case 226: return Token(MB_AccentHat, curr-2, 2);\
	 case 227: return Token(MB_AccentTilde, curr-2, 2);\
	 case 98: return Token(MB_Binomial, curr-2, 2);\
	 case 99: return Token(MB_Cases, curr-2, 2);\
	 case 8751: return Token(MB_ClosedSurfaceIntegral, curr-2, 2);\
	 case 8752: return Token(MB_ClosedVolumeIntegral, curr-2, 2);\
	 case 8750: return Token(MB_ContourIntegral, curr-2, 2);\
	 case 8720: return Token(MB_Coproduct, curr-2, 2);\
	 case 8748: return Token(MB_DoubleIntegral, curr-2, 2);\
	 case 916: return Token(MB_Dualscript, curr-2, 2);\
	 case 9482: return Token(MB_EvalBar, curr-2, 2);\
	 case 102: return Token(MB_Fraction, curr-2, 2);\
	 case 124: return Token(MB_GroupingBar, curr-2, 2);\
	 case 91: return Token(MB_GroupingBracket, curr-2, 2);\
	 case 8968: return Token(MB_GroupingCeil, curr-2, 2);\
	 case 8214: return Token(MB_GroupingDoubleBars, curr-2, 2);\
	 case 8970: return Token(MB_GroupingFloor, curr-2, 2);\
	 case 40: return Token(MB_GroupingParen, curr-2, 2);\
	 case 8600: return Token(MB_Infimum, curr-2, 2);\
	 case 8747: return Token(MB_Integral, curr-2, 2);\
	 case 8898: return Token(MB_Intersection, curr-2, 2);\
	 case 108: return Token(MB_Lim, curr-2, 2);\
	 case 8862: return Token(MB_Matrix, curr-2, 2);\
	 case 8593: return Token(MB_Max, curr-2, 2);\
	 case 8595: return Token(MB_Min, curr-2, 2);\
	 case 8719: return Token(MB_Product, curr-2, 2);\
	 case 8730: return Token(MB_Root, curr-2, 2);\
	 case 95: return Token(MB_Subscript, curr-2, 2);\
	 case 8721: return Token(MB_Sum, curr-2, 2);\
	 case 94: return Token(MB_Superscript, curr-2, 2);\
	 case 8599: return Token(MB_Supremum, curr-2, 2);\
	 case 8749: return Token(MB_TripleIntegral, curr-2, 2);\
	 case 8899: return Token(MB_Union, curr-2, 2);\
	 case 10756: return Token(MB_UnionPlus, curr-2, 2);

#define NEB_NUMBER_CASES_MACRO_EXPANSION\
	 case '0': return scanZeroNumber();\
	 case '1': return scanNonzeroNumber();\
	 case '2': return scanNonzeroNumber();\
	 case '3': return scanNonzeroNumber();\
	 case '4': return scanNonzeroNumber();\
	 case '5': return scanNonzeroNumber();\
	 case '6': return scanNonzeroNumber();\
	 case '7': return scanNonzeroNumber();\
	 case '8': return scanNonzeroNumber();\
	 case '9': return scanNonzeroNumber();

#define NEB_SCRIPTED_NUMBER_CASES_MACRO_EXPANSION\
	 case 8304: return scanSuperscriptZeroNumber();\
	 case 185: return scanSuperscriptNonzeroNumber();\
	 case 178: return scanSuperscriptNonzeroNumber();\
	 case 179: return scanSuperscriptNonzeroNumber();\
	 case 8308: return scanSuperscriptNonzeroNumber();\
	 case 8309: return scanSuperscriptNonzeroNumber();\
	 case 8310: return scanSuperscriptNonzeroNumber();\
	 case 8311: return scanSuperscriptNonzeroNumber();\
	 case 8312: return scanSuperscriptNonzeroNumber();\
	 case 8313: return scanSuperscriptNonzeroNumber();\
	 case 8320: return scanSubscriptZeroNumber();\
	 case 8321: return scanSubscriptNonzeroNumber();\
	 case 8322: return scanSubscriptNonzeroNumber();\
	 case 8323: return scanSubscriptNonzeroNumber();\
	 case 8324: return scanSubscriptNonzeroNumber();\
	 case 8325: return scanSubscriptNonzeroNumber();\
	 case 8326: return scanSubscriptNonzeroNumber();\
	 case 8327: return scanSubscriptNonzeroNumber();\
	 case 8328: return scanSubscriptNonzeroNumber();\
	 case 8329: return scanSubscriptNonzeroNumber();

#define NEB_NUM_TOKENTYPES 185

#define NEB_IMPLICIT_MULT_MACRO_EXPANSION {\
	ContourIntegral,\
	Cos,\
	Identifier,\
	Integral,\
	LeftAngle,\
	LeftBrace,\
	LeftCeil,\
	LeftFloor,\
	LeftParen,\
	Log,\
	MB_AccentArrow,\
	MB_AccentBar,\
	MB_AccentBreve,\
	MB_AccentDddot,\
	MB_AccentDdot,\
	MB_AccentDot,\
	MB_AccentHat,\
	MB_AccentTilde,\
	MB_Binomial,\
	MB_Cases,\
	MB_ClosedSurfaceIntegral,\
	MB_ClosedVolumeIntegral,\
	MB_ContourIntegral,\
	MB_Coproduct,\
	MB_DoubleIntegral,\
	MB_Dualscript,\
	MB_Fraction,\
	MB_GroupingBar,\
	MB_GroupingBracket,\
	MB_GroupingCeil,\
	MB_GroupingDoubleBars,\
	MB_GroupingFloor,\
	MB_GroupingParen,\
	MB_Infimum,\
	MB_Integral,\
	MB_Intersection,\
	MB_Lim,\
	MB_Matrix,\
	MB_Max,\
	MB_Min,\
	MB_Product,\
	MB_Root,\
	MB_Subscript,\
	MB_Sum,\
	MB_Superscript,\
	MB_Supremum,\
	MB_TripleIntegral,\
	Nabla,\
	NaturalLog,\
	PlanckConst,\
	Sin,\
	Tangent,\
}

#define NEB_NUM_IMPLICIT_MULT 52

#endif
