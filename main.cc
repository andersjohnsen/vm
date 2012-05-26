#include <iostream>

#include <unistd.h>

#include "ir/value.h"
#include "assembler.h"
#include "register_allocator.h"

typedef long long (*CallType)();

int main() {
  std::cout << "A vm\n";

  IR::BasicBlock *block = new IR::BasicBlock("entry");

  IR::ConstantInt *a = new IR::ConstantInt(25);
  IR::ConstantInt *b = new IR::ConstantInt(17);

  IR::BinaryOperator *add = new IR::BinaryOperator(
      IR::BinaryOperator::Add, a, b, a->getType());
  block->addInstruction(add);

  IR::BinaryOperator *mul = new IR::BinaryOperator(
      IR::BinaryOperator::Add, add, new IR::ConstantInt(3), add->getType());
  block->addInstruction(mul);

  IR::BinaryOperator *sub = new IR::BinaryOperator(
      IR::BinaryOperator::Add, mul, add, mul->getType());
  block->addInstruction(sub);

  block->addInstruction(new IR::ReturnInst(sub));

  block->print(std::cout);

  NaiveRegisterAllocator allocator(block);
  allocator.calculate();

  block->print(std::cout);

  allocator.print(std::cout);

  Page *page = new Page(1024 * 16);
  page->makeExecutable();

  Assembler assembler(page);

  assembler.mov((uint64_t)42, Assembler::RAX);
  assembler.mov((uint64_t)6, Assembler::RBX);
  assembler.mov(Assembler::RAX, Assembler::RBX);
  assembler.add(Assembler::RBX, Assembler::RAX);
  assembler.add(Assembler::RAX, Assembler::RBX);
  assembler.mov(Assembler::RBX, Assembler::RAX);
  assembler.ret();

  CallType call = reinterpret_cast<CallType>(page->getData());
  std::cout << "Return value: " << call() << "\n";

  return 0;
}
