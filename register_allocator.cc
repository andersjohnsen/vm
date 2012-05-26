#include <set>

#include "ir/value.h"

#include "assembler.h"
#include "register_allocator.h"


void RegisterAllocator::print(std::ostream &stream) {
  for (std::map<IR::Value *, Assembler::Register>::iterator it = valueToRegister.begin(),
       end = valueToRegister.end(); it != end; it++) {
    stream << it->first->referenceString() << " : " << it->second << "\n";
  }
}

void NaiveRegisterAllocator::calculate() {
  std::vector<Assembler::Register> freeRegisters;
  freeRegisters.push_back(Assembler::RBX);
  freeRegisters.push_back(Assembler::RAX);

  basicBlock->calculateLiveness();

  std::set<IR::Value *> live;

  const std::vector<IR::Instruction *> &instructions = basicBlock->getInstructions();
  for (std::vector<IR::Instruction *>::const_iterator inst = instructions.begin(),
       end = instructions.end(); inst != end; inst++) {

    valueToRegister[*inst] = freeRegisters.back();
    freeRegisters.pop_back();
    live.insert(*inst);

    std::set<IR::Value *> liveness(basicBlock->getInstLiveness(*inst).begin(),
                                   basicBlock->getInstLiveness(*inst).end());
    for (std::set<IR::Value *>::const_iterator it = live.begin(),
         end = live.end(); it != end; it++) {
      if (liveness.find(*it) == liveness.end()) {
        freeRegisters.push_back(valueToRegister[*it]);
        live.erase(it);
      }
    }
  }
}
