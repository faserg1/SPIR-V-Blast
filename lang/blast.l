%{
#include <memory>
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
		gen::BlastParser::symbol_type yylex(LexContext &lc);
		int yylex() override;
	private:
		s_symbol_t currentSymbol_;
		LexContext *lc = nullptr;
		int column_;
		location currentLocation_;
	private:
		void updateLocation();
	};
}

%}

%option yyclass="gen::BlastScanner"
%option noyywrap
%option yylineno
%option c++

%%
[ \t] ; //Do nothing

[0-9]+ {currentSymbol_ = makeILiteral(std::string(yytext), currentLocation_); return 0;}
[0-9]+u {currentSymbol_ = makeULiteral(std::string(yytext), currentLocation_); return 0;}
[0-9]+f {currentSymbol_ = makeDLiteral(std::string(yytext), currentLocation_); return 0;}
[0-9]+.[0-9]* {currentSymbol_ = makeDLiteral(std::string(yytext), currentLocation_); return 0;}

%%

/*
[a-zA-Z_][a-zA-Z_0-9]+
{
	std::string ident(yytext);
	auto type = lc.getIdentifierType(ident);
}
*/

s_symbol_t makeStringLiteral(std::string str, const gen::BlastParser::location_type& loc)
{
	Literal l;
	l.type = LiteralType::String;
	l.string = str;
	return std::make_shared<symbol_t>(gen::BlastParser::make_NUMLITERAL(l, loc));
}

s_symbol_t makeILiteral(std::string str, const gen::BlastParser::location_type& loc)
{
	Literal l;
	l.type = LiteralType::INumber;
	l.inum = std::stoll(str);
	return std::make_shared<symbol_t>(gen::BlastParser::make_NUMLITERAL(l, loc));
}

s_symbol_t makeULiteral(std::string str, const gen::BlastParser::location_type& loc)
{
	Literal l;
	l.type = LiteralType::UNumber;
	l.unum = std::stoull(str);
	return std::make_shared<symbol_t>(gen::BlastParser::make_NUMLITERAL(l, loc));
}

s_symbol_t makeDLiteral(std::string str, const gen::BlastParser::location_type& loc)
{
	Literal l;
	l.type = LiteralType::DNumber;
	l.dnum = std::stold(str);
	return std::make_shared<symbol_t>(gen::BlastParser::make_STRINGLITERAL(l, loc));
}

gen::BlastParser::symbol_type gen::BlastScanner::yylex(LexContext &lc)
{
	this->lc = &lc;
	if (this->yylex())
		return *currentSymbol_;
	currentSymbol_ = {};
	/*
	TODO: [OOKAMI] Throw error?
	*/
	return {};
}

void gen::BlastScanner::updateLocation()
{
	location l(nullptr, yylineno, column_);
}

gen::BlastParser::symbol_type yylex(gen::BlastScanner *scanner, LexContext &ctx)
{
	return scanner->yylex(ctx);
}
