#ifndef INST_LOWER_H
#define INST_LOWER_H

#include "ir/value.h"
#include "register_allocator.h"

class InstLower : public Visitor {
public:
  InstLower(Assembler *assembler);

public:
  void lower(IR::BasicBlock *block, RegisterAllocator *allocator);

private:
  Assembler *assembler;
};

#endif // INST_LOWER_H
