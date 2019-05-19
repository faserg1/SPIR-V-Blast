%{
#include <memory>
#include <sstream>
#include "blast_tokens.hpp"
#include "location.hh"

typedef gen::BlastParser::symbol_type symbol_t;
typedef std::shared_ptr<symbol_t> s_symbol_t;

struct ResultOf_yylex
{
	enum EResultOf_yylex
	{
		Success,
		Nothing,
		Error,
	};
};


namespace gen
{
	class BlastScanner : public yyFlexLexer
	{
	public:
		BlastScanner(const std::string &source);
		gen::BlastParser::symbol_type yylex(LexContext &lc);
		int yylex() override;
	private:
		std::stringstream source_;
		s_symbol_t currentSymbol_;
		LexContext *lc = nullptr;
		int column_;
		location currentLocation_;
		std::string text_;
	private:
		void updateLocation();
		int stop();
		int makeStringLiteral();
		int makeILiteral();
		int makeULiteral();
		int makeDLiteral();
		int makeIdentifier();
		int makeSymbol();
		template <class Func, class ... TArgs>
		int s_make(Func f, TArgs ... args)
		{
			currentSymbol_ = std::make_shared<symbol_t>(f(args..., currentLocation_));
			return ResultOf_yylex::Success;
		}
	};
}

#define YY_USER_ACTION {text_ = std::string(yytext_ptr); updateLocation();}

%}

%option yyclass="gen::BlastScanner"
%option noyywrap
%option yylineno
%option c++

%%

"do" {return s_make(&gen::BlastParser::make_DO);}
"while" {return s_make(&gen::BlastParser::make_WHILE);}
"if" {return s_make(&gen::BlastParser::make_IF);}
"switch" {return s_make(&gen::BlastParser::make_SWITCH);}
"for" {return s_make(&gen::BlastParser::make_FOR);}


"default" {return s_make(&gen::BlastParser::make_DEFAULT);}
"case" {return s_make(&gen::BlastParser::make_CASE);}


"return" {return s_make(&gen::BlastParser::make_RETURN);}
"break" {return s_make(&gen::BlastParser::make_BREAK);}
"continue" {return s_make(&gen::BlastParser::make_CONTINUE);}


"const" {return s_make(&gen::BlastParser::make_CONST);}


"bit_cast" {return s_make(&gen::BlastParser::make_BITCAST);}


"void" {return s_make(&gen::BlastParser::make_VOID);}
"int" {return s_make(&gen::BlastParser::make_INT);}
"float" {return s_make(&gen::BlastParser::make_FLOAT);}
"bool" {return s_make(&gen::BlastParser::make_BOOL);}

"mat" {return s_make(&gen::BlastParser::make_MATRIX);}
"vec" {return s_make(&gen::BlastParser::make_VECTOR);}

"img" {return s_make(&gen::BlastParser::make_IMAGE);}
"smpl" {return s_make(&gen::BlastParser::make_SAMPLER);}
"simg" {return s_make(&gen::BlastParser::make_SAMPLED_IMAGE);}


"struct" {return s_make(&gen::BlastParser::make_STRUCT);}

<<EOF>> {return stop();}
"\r\n"|"\n\r"|[\n] {return ResultOf_yylex::Nothing;};
[ \t\v\b\f] {return ResultOf_yylex::Nothing;};
"/*"(.|"\r\n"|"\n\r"|[\n])*"*/" {return ResultOf_yylex::Nothing;}
"//"[^\n]* {return ResultOf_yylex::Nothing;}

"\""[^\"]*"\"" {return makeStringLiteral();}
[0-9]+[u] {return makeULiteral();}
[0-9]+[f] {return makeDLiteral();}
[0-9]+ {return makeILiteral();}
[0-9]+\.[0-9]* {return makeDLiteral();}

"true" {return s_make(&gen::BlastParser::make_C_TRUE);}
"false" {return s_make(&gen::BlastParser::make_C_FALSE);}

[a-zA-Z_][a-zA-Z_0-9]* {return makeIdentifier();}

"[[" {return s_make(&gen::BlastParser::make_ATTR_OPEN);}
"]]" {return s_make(&gen::BlastParser::make_ATTR_CLOSE);}

"||" {return s_make(&gen::BlastParser::make_OR);}
"&&" {return s_make(&gen::BlastParser::make_AND);}
"==" {return s_make(&gen::BlastParser::make_EQ);}
"!=" {return s_make(&gen::BlastParser::make_NE);}

'<' {return s_make(&gen::BlastParser::make_LESS);}
'>' {return s_make(&gen::BlastParser::make_MORE);}
"<=" {return s_make(&gen::BlastParser::make_LESS_EQ);}
">=" {return s_make(&gen::BlastParser::make_MORE_EQ);}

"/%" {return s_make(&gen::BlastParser::make_MOD);}

"++" {return s_make(&gen::BlastParser::make_INC);}
"--" {return s_make(&gen::BlastParser::make_DEC);}

"<<" {return s_make(&gen::BlastParser::make_SHIFT_LEFT_A);}
">>" {return s_make(&gen::BlastParser::make_SHIFT_RIGHT_A);}
"!<<" {return s_make(&gen::BlastParser::make_SHIFT_LEFT_L);}
"!>>" {return s_make(&gen::BlastParser::make_SHIFT_RIGHT_L);}

"+=" {return s_make(&gen::BlastParser::make_PL_EQ);}
"-=" {return s_make(&gen::BlastParser::make_MI_EQ);}
"*=" {return s_make(&gen::BlastParser::make_MUL_EQ);}
"/=" {return s_make(&gen::BlastParser::make_DIV_EQ);}
"/%=" {return s_make(&gen::BlastParser::make_MOD_EQ);}
"%=" {return s_make(&gen::BlastParser::make_REM_EQ);}
"&=" {return s_make(&gen::BlastParser::make_AND_EQ);}
"|=" {return s_make(&gen::BlastParser::make_OR_EQ);}
"^=" {return s_make(&gen::BlastParser::make_XOR_EQ);}

"<<=" {return s_make(&gen::BlastParser::make_SHIFT_LEFT_A_EQ);}
">>=" {return s_make(&gen::BlastParser::make_SHIFT_RIGHT_A_EQ);}
"!<<=" {return s_make(&gen::BlastParser::make_SHIFT_LEFT_L_EQ);}
"!>>=" {return s_make(&gen::BlastParser::make_SHIFT_RIGHT_L_EQ);}

"->" {return s_make(&gen::BlastParser::make_PTR_ACCESS);}

. {return makeSymbol();}

%%

/*

*/

gen::BlastParser::symbol_type gen::BlastScanner::yylex(LexContext &lc)
{
	this->lc = &lc;
	int result;
	do
	{
		result = this->yylex();
	} while (result == ResultOf_yylex::Nothing);
	if (result == ResultOf_yylex::Success)
		return *currentSymbol_;
	/*
	TODO: [OOKAMI] Throw error?
	*/
	return {};
}

gen::BlastScanner::BlastScanner(const std::string &source) : column_(0)
{
	source_ << source;
	switch_streams(&source_);
}

void gen::BlastScanner::updateLocation()
{
	currentLocation_.step();
	auto newLines = std::count(text_.begin(), text_.end(), '\n');
	currentLocation_.lines(newLines);
	size_t columns = 0;
	for (auto it = text_.rbegin(); it != text_.rend(); columns++, it++)
	{
		if (*it == '\n')
			break;
	}
	currentLocation_.columns(columns);
}

int gen::BlastScanner::stop()
{
	return s_make(&gen::BlastParser::make_END);
}

gen::BlastParser::symbol_type yylex(gen::BlastScanner *scanner, LexContext &ctx)
{
	return scanner->yylex(ctx);
}

int gen::BlastScanner::makeStringLiteral()
{
	Literal l;
	l.type = LiteralType::String;
	l.string = text_;
	return s_make(&gen::BlastParser::make_STRINGLITERAL, l);
}

int gen::BlastScanner::makeILiteral()
{
	Literal l;
	l.type = LiteralType::INumber;
	l.inum = std::stoll(text_);
	return s_make(&gen::BlastParser::make_NUMLITERAL, l);
}

int gen::BlastScanner::makeULiteral()
{
	Literal l;
	l.type = LiteralType::UNumber;
	l.unum = std::stoull(text_);
	return s_make(&gen::BlastParser::make_NUMLITERAL, l);
}

int gen::BlastScanner::makeDLiteral()
{
	Literal l;
	l.type = LiteralType::DNumber;
	l.dnum = std::stold(text_);
	return s_make(&gen::BlastParser::make_NUMLITERAL, l);
}

int gen::BlastScanner::makeIdentifier()
{
	auto type = lc->getIdentifierType(text_);
	switch (type)
	{
		case IdentifierType::Structure:
			return s_make(&gen::BlastParser::make_USER_DEFINED_TYPE, text_);
		default:
			return s_make(&gen::BlastParser::make_IDENTIFIER, text_);
	}
}

int gen::BlastScanner::makeSymbol()
{
	char symbol = text_[0] & 0xFF;
	auto f = [](auto...s){return gen::BlastParser::symbol_type(s...);};
	return s_make(f, gen::BlastParser::token_type(symbol));
}
