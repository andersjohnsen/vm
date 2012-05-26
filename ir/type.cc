#include "type.h"

using namespace IR;

void VoidType::print(std::ostream &stream) const {
  stream << "void";
}

VoidType *VoidType::instance = new VoidType();

void IntType::print(std::ostream &stream) const {
  stream << "int64";
}

IntType *IntType::instance = new IntType();

