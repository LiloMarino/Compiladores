#include "variable.hpp"
#include "function.hpp"
#include "mips.hpp"

Variable::Variable(VariableCategory category, std::unique_ptr<Type> type, const std::string &identifier)
    : type(std::move(type)), category(category), identifier(identifier) {}

Variable::Variable(const std::string &identifier, const int value)
    : category(VariableCategory::CONSTANT), identifier(identifier), value(value) {}

std::string Variable::getIdentifier() const
{
    return identifier;
}

void Variable::translate(Function *func_context)
{
    switch (category)
    {
    case VariableCategory::CONSTANT:
        MIPS::createConstant(identifier, value);
        break;
    case VariableCategory::GLOBAL_VARIABLE:
        MIPS::createGlobalVar(identifier, value);
        break;
    case VariableCategory::LOCAL_VARIABLE:
        func_context->addRegister(identifier, MIPS::getSaveRegister());
        break;
    case VariableCategory::PARAMETER:
        func_context->addRegister(identifier, MIPS::getArgumentRegister());
        break;
    }
}