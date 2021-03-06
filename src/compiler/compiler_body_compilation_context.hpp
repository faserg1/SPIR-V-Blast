#ifndef COMPILER_BODY_COMPILATION_CONTEXT
#define COMPILER_BODY_COMPILATION_CONTEXT

#include <memory>
#include <string>
#include <map>
#include "compiler_data.hpp"

class CompilerContext;

class CompilerBodyCompilationContext :
	public std::enable_shared_from_this<CompilerBodyCompilationContext>
{
public:
	static std::shared_ptr<CompilerBodyCompilationContext> create(CompilerContext *ctx, std::shared_ptr<CompilerBodyCompilationContext> parent = {});

	Id createLabelId(const std::string &name);
	Id getLabel(const std::string &name, bool recursive = true);
protected:
	CompilerBodyCompilationContext(CompilerContext *ctx, std::shared_ptr<CompilerBodyCompilationContext> parent = {});
private:
	CompilerContext *ctx_;
	std::weak_ptr<CompilerBodyCompilationContext> parent_;
	std::vector<std::shared_ptr<CompilerBodyCompilationContext>> childs_;
	std::map<std::string, Id> labels_;
private:
	void addChild(std::shared_ptr<CompilerBodyCompilationContext> child);
};

#endif