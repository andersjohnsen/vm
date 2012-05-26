#include "inst_lower.h"

InstLower::InstLower(Assembler *assembler) : assembler(assembler) {
}

void InstLower::lower(IR::BasicBlock *block, RegisterAllocator *allocator) {
  for (std::vector<IR::Instruction *>::const_iterator it = block->getInstructions().begin(),
       end = block->getInstructions().end(); it != end; it++) {
    (*it)->print(std::cout);
    std::cout << "\n";
  }
}

