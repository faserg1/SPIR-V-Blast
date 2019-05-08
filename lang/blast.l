%{
#include <memory>
#include <sstream>
#include "blast_tokens.hpp"
#include "location.hh"

typedef gen::BlastParser::symbol_type symbol_t;
typedef std::shared_ptr<symbol_t> s_symbol_t;

s_symbol_t makeStringLiteral(std::string str, const gen::BlastParser::location_type& loc);
s_symbol_t makeILiteral(std::string str, const gen::BlastParser::location_type& loc);
s_symbol_t makeULiteral(std::string str, const gen::BlastParser::location_type& loc);
s_symbol_t makeDLiteral(std::string str, const gen::BlastParser::location_type& loc);

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
		template <class Func, class ... TArgs>
		int s_make(Func f, TArgs ... args)
		{
			currentSymbol_ = std::make_shared<symbol_t>(f(args..., currentLocation_));
			return 0;
		}
	};
}

#define YY_USER_ACTION {updateLocation(); text_ = std::string(yytext_ptr);}

%}

%option yyclass="gen::BlastScanner"
%option noyywrap
%option yylineno
%option c++

%%
[ \t] ; //Do nothing

[0-9]+ {return makeILiteral();}
[0-9]+u {return makeULiteral();}
[0-9]+f {return makeDLiteral();}
[0-9]+.[0-9]* {return makeDLiteral();}

<<EOF>> {return stop();}

%%

/*
[a-zA-Z_][a-zA-Z_0-9]+
{
	std::string ident(yytext);
	auto type = lc.getIdentifierType(ident);
}
*/

gen::BlastParser::symbol_type gen::BlastScanner::yylex(LexContext &lc)
{
	this->lc = &lc;
	if (this->yylex())
		return *currentSymbol_;
	/*
	TODO: [OOKAMI] Throw error?
	*/
	return {};
}

gen::BlastScanner::BlastScanner(const std::string &source)
{
	source_ << source;
	switch_streams(&source_);
}

void gen::BlastScanner::updateLocation()
{
	location l(nullptr, yylineno, column_);
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
