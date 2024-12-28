#include "variable.hpp"

Variable::Variable(VariableCategory category, std::unique_ptr<Type> type, const std::string &identifier)
    : type(std::move(type)), category(category), identifier(identifier) {}

Variable::Variable(const std::string &identifier, const int value)
    : category(VariableCategory::CONSTANT), identifier(identifier), value(value) {}
