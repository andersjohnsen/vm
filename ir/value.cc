#include <cassert>
#include <sstream>
#include <set>

#include "value.h"

using namespace IR;

std::string Value::referenceString() {
  std::ostringstream out;
  print(out);
  return out.str();
}

ConstantInt::ConstantInt(uint64_t value) :
    value(value), Constant(IntType::get()) {
}

void ConstantInt::print(std::ostream &stream) {
  getType()->print(stream);
  stream << " $" << value;
}

std::string Instruction::referenceString() {
  assert(basicBlock);
  return basicBlock->getNameOf(this);
}

void ReturnInst::print(std::ostream &stream) {
  stream << "ret";
  if (value) {
    stream << " " << value->referenceString();
  }
}

BinaryOperator::BinaryOperator(Opcode op, Value *left, Value *right, Type *type)
    : Instruction(type), op(op), left(left), right(right) {
  assert(type == left->getType());
  assert(type == right->getType());
}

void BinaryOperator::print(std::ostream &stream) {
  stream << left->referenceString();
  switch (op) {
    case Add: stream << " + "; break;
    case Sub: stream << " - "; break;
    case Mul: stream << " * "; break;
    case Div: stream << " / "; break;
  }
  stream << right->referenceString();
}

void BasicBlock::addInstruction(Instruction *inst) {
  assert(inst->basicBlock == 0);
  inst->basicBlock = this;
  instructions.push_back(inst);
}

void BasicBlock::calculateLiveness() {
  std::set<Value *> liveValues;
  for (std::vector<Instruction *>::reverse_iterator it = instructions.rbegin(),
       end = instructions.rend(); it != end; it++) {
    // Give the instruction the current live values.
    liveness[*it] = std::vector<Value *>(liveValues.begin(), liveValues.end());
    // Remove the current instruction
    liveValues.erase(*it);
    // Get all used values. We don't care for constants here.
    const std::vector<Value *> &list = (*it)->getUsedValuesNotConstant();
    liveValues.insert(list.begin(), list.end());

  }
}

void BasicBlock::print(std::ostream &stream) {
  stream << name << ":\n";
  for (std::vector<Instruction *>::const_iterator it = instructions.begin(),
       end = instructions.end(); it != end; it++) {
    stream << "  " << getNameOf(*it) << " = ";
    (*it)->print(stream);
    stream << " [";
    for (std::vector<Value *>::iterator live = liveness[*it].begin(),
         end = liveness[*it].end(); live != end; live++) {
      stream << (*live)->referenceString() << ", ";
    }
    stream << "]\n";
  }
  stream << "\n";
}

std::string BasicBlock::getNameOf(Instruction *inst) {
  if (symbolTable.find(inst) == symbolTable.end()) {
    symbolTable[inst] = nextName();
  }
  std::ostringstream out;
  inst->getType()->print(out);
  out << " %" << symbolTable[inst];
  return out.str();
}

std::string BasicBlock::nextName() {
  std::ostringstream out;
  out << "v" << nextNameSuffix++;
  return out.str();
}
