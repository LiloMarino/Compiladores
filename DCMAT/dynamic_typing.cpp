#include "dynamic_typing.hpp"

DynamicTyping::Data::Data() : number(0.0) {}
DynamicTyping::Data::~Data() {}

DynamicTyping::DynamicTyping() : type(DataType::NONE) {}
DynamicTyping::~DynamicTyping()
{
    clear();
}

void DynamicTyping::clear()
{
    if (type == DataType::MATRIX)
    {
        data.matrix.~Matrix(); // Chamamos explicitamente o destrutor
    }
    type = DataType::NONE;
}

 
void DynamicTyping::setNumber(double num)
{
    clear();
    data.number = num;
    type = DataType::NUMBER;
}


void DynamicTyping::setMatrix(Matrix &&m)
{
    clear();
    new (&data.matrix) Matrix(std::move(m));
    type = DataType::MATRIX;
}

 
bool DynamicTyping::isNumber() const
{
    return type == DataType::NUMBER;
}

 
bool DynamicTyping::isMatrix() const
{
    return type == DataType::MATRIX;
}

 
double DynamicTyping::getNumber() const
{
    if (!isNumber())
    {
        throw std::runtime_error("DynamicTyping does not contain a number.");
    }
    return data.number;
}

 
const Matrix &DynamicTyping::getMatrix() const
{
    if (!isMatrix())
    {
        throw std::runtime_error("DynamicTyping does not contain a matrix.");
    }
    return data.matrix;
}