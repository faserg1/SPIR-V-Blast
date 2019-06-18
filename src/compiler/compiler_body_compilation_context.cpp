#include "compiler_body_compilation_context.hpp"
#include "compiler_context.hpp"
#include "compiler_helper.hpp"

CompilerBodyCompilationContext::CompilerBodyCompilationContext(CompilerContext *ctx, std::shared_ptr<CompilerBodyCompilationContext> parent) :
	ctx_(ctx), parent_(parent)
{
}

std::shared_ptr<CompilerBodyCompilationContext> CompilerBodyCompilationContext::create(CompilerContext *ctx, std::shared_ptr<CompilerBodyCompilationContext> parent)
{
	auto ptr = std::shared_ptr<CompilerBodyCompilationContext>(new CompilerBodyCompilationContext(ctx, parent));
	if (parent)
		parent->addChild(ptr);
	return ptr;
}

Id CompilerBodyCompilationContext::createLabelId(const std::string &name)
{
	using namespace std::string_literals;
	auto id = ctx_->createId("label_"s + name);
	auto debugOp = CompilerHelper::debugOp(id);
	ctx_->addDebug(debugOp);
	return id;
}

Id CompilerBodyCompilationContext::getLabel(const std::string &name, bool recursive)
{
	auto it = labels_.find(name);
	if (it != labels_.end())
		return it->second;
	if (recursive)
	{
		auto parent = parent_.lock();
		if (parent)
			return parent->getLabel(name, recursive);
	}
	return {};
}

void CompilerBodyCompilationContext::addChild(std::shared_ptr<CompilerBodyCompilationContext> child)
{
	childs_.push_back(child);
}
