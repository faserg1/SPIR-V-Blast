%{
#include "blast_tokens.hpp"
#include "location.hh"

gen::BlastParser::semantic_type makeStringLiteral(std::string str, const gen::BlastParser::location_type& loc);
gen::BlastParser::semantic_type makeILiteral(std::string str, const gen::BlastParser::location_type& loc);
gen::BlastParser::semantic_type makeULiteral(std::string str, const gen::BlastParser::location_type& loc);
gen::BlastParser::semantic_type makeDLiteral(std::string str, const gen::BlastParser::location_type& loc);

namespace gen
{
	class BlastScanner : public yyFlexLexer
	{
	public:
		gen::BlastParser::symbol_type yylex(LexContext &lc);
		YY_DECL;
	private:
		gen::BlastParser::symbol_type currentSymbol_;
		LexContext *lc = nullptr;
	};
}

%}

%option yyclass="gen::BlastScanner"
%option noyywrap
%option c++

%%
[ \t] ; //Do nothing

[0-9]+ {currentSymbol_ = makeILiteral(std::string(yytext)); return 0;}
[0-9]+u {currentSymbol_ =  makeULiteral(std::string(yytext)); return 0;}
[0-9]+f {currentSymbol_ =  makeDLiteral(std::string(yytext)); return 0;}
[0-9]+.[0-9]* {currentSymbol_ =  makeDLiteral(std::string(yytext)); return 0;}

%%

/*
[a-zA-Z_][a-zA-Z_0-9]+
{
	std::string ident(yytext);
	auto type = lc.getIdentifierType(ident);
}
*/

gen::BlastParser::semantic_type makeStringLiteral(std::string str, const gen::BlastParser::location_type& loc)
{
	Literal l;
	l.type = LiteralType::String;
	l.string = str;
	return gen::BlastParser::make_NUMLITERAL(l, loc);
}

gen::BlastParser::semantic_type makeILiteral(std::string str, const gen::BlastParser::location_type& loc)
{
	Literal l;
	l.type = LiteralType::INumber;
	l.inum = std::stoll(str);
	return gen::BlastParser::make_NUMLITERAL(l, loc);
}

gen::BlastParser::semantic_type makeULiteral(std::string str, const gen::BlastParser::location_type& loc)
{
	Literal l;
	l.type = LiteralType::UNumber;
	l.unum = std::stoull(str);
	return gen::BlastParser::make_NUMLITERAL(l, loc);
}

gen::BlastParser::semantic_type makeDLiteral(std::string str, const gen::BlastParser::location_type& loc)
{
	Literal l;
	l.type = LiteralType::DNumber;
	l.dnum = std::stold(str);
	return gen::BlastParser::make_STRINGLITERAL(l, loc);
}

gen::BlastParser::symbol_type gen::BlastScanner::yylex(LexContext &lc)
{
	this->lc = &lc;
	if (this->yylex())
		return currentSymbol_;
	//TODO: [OOKAMI] Throw error?
	return {};
}

gen::BlastParser::symbol_type yylex(gen::BlastScanner *scanner, LexContext &ctx)
{
	return scanner->yylex(ctx);
}
