#ifndef REGISTER_ALLOCATOR_H
#define REGISTER_ALLOCATOR_H

#include <iostream>

#include "assembler.h"

class RegisterAllocator {
protected:
  RegisterAllocator(IR::BasicBlock *basicBlock) : basicBlock(basicBlock) {}

public:
  virtual void calculate() = 0;

  Assembler::Register getRegisterForValue(IR::Value *value) {
    return valueToRegister[value];
  }

  void print(std::ostream &stream);

protected:
  IR::BasicBlock *basicBlock;
  std::map<IR::Value *, Assembler::Register> valueToRegister;
};

class NaiveRegisterAllocator : public RegisterAllocator {
public:
  NaiveRegisterAllocator(IR::BasicBlock *basicBlock)
      : RegisterAllocator(basicBlock) {}

public:
  void calculate();
};

#endif // REGISTER_ALLOCATOR_H
