#ifndef PARSER_STATES_GENERATOR
#define PARSER_STATES_GENERATOR

#include <vector>
#include <memory>

class IParserState;

std::vector<std::shared_ptr<IParserState>> generateParserStates();

#endif