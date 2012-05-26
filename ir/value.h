#ifndef VALUE_H
#define VALUE_H

#include <vector>
#include <map>
#include <string>
#include <ostream>

#include <stdint.h>

#include "type.h"

namespace IR {
  class BasicBlock;

  class Value {
  protected:
    Value(Type *type) : type(type) {}

  public:
    Type *getType() const { return type; }

    virtual std::vector<Value *> getUsedValues() const {
      return std::vector<Value *>();
    }

    virtual std::vector<Value *> getUsedValuesNotConstant() const {
      return std::vector<Value *>();
    }

  public:
    virtual void print(std::ostream &stream) = 0;
    virtual std::string referenceString();
    virtual bool isConstant() { return false; }

  private:
    Type *type;
  };

  class Constant : public Value {
  protected:
    Constant(Type *type) : Value(type) {}

  public:
    bool isConstant() { return true; }
  };

  class ConstantInt : public Constant {
  public:
    ConstantInt(uint64_t value);

  public:
    void print(std::ostream &stream);

  private:
    uint64_t value;
  };

  class Instruction : public Value {
  protected:
    Instruction(Type *type) : Value(type), basicBlock(0) {}

  public:
    BasicBlock *getBasicBlock() { return basicBlock; }

    std::string referenceString();

  private:
    friend class BasicBlock;
    BasicBlock *basicBlock;
  };

  class ReturnInst : public Instruction {
  public:
    ReturnInst() : Instruction(VoidType::get()), value(0) {}
    ReturnInst(Value *value) : Instruction(value->getType()), value(value) {}

  public:
    void print(std::ostream &stream);

    std::vector<Value *> getUsedValues() const {
      std::vector<Value *> values;
      if (value) values.push_back(value);
      return values;
    }

    std::vector<Value *> getUsedValuesNotConstant() const {
      std::vector<Value *> values;
      if (value && !value->isConstant()) values.push_back(value);
      return values;
    }

  private:
    Value *value;
  };

  class BinaryOperator : public Instruction {
  public:
    enum Opcode {
      Add,
      Sub,
      Mul,
      Div
    };

  public:
    BinaryOperator(Opcode op, Value *left, Value *right, Type *type);

  public:
    void print(std::ostream &stream);

    std::vector<Value *> getUsedValues() const {
      std::vector<Value *> values;
      values.push_back(left);
      values.push_back(right);
      return values;
    }

    std::vector<Value *> getUsedValuesNotConstant() const {
      std::vector<Value *> values;
      if (!left->isConstant()) values.push_back(left);
      if (!right->isConstant()) values.push_back(right);
      return values;
    }

  private:
    Opcode op;
    Value *left;
    Value *right;
    Type *type;
  };

  class BasicBlock : public Value {
  public:
    BasicBlock(const std::string &name)
        : Value(VoidType::get()), name(name), nextNameSuffix(0) {}

  public:
    void addInstruction(Instruction *inst);

    const std::vector<Instruction *> &getInstructions() const {
      return instructions;
    }

    void calculateLiveness();

    const std::string &getName() { return name; }

    const std::vector<Value *> &getInstLiveness(Instruction *inst) const {
      return liveness.find(inst)->second;
    }

  public:
    void print(std::ostream &stream);
    std::string getNameOf(Instruction *inst);

  private:
    std::string nextName();

  private:
    std::vector<Instruction *> instructions;
    std::map<Instruction *, std::string> symbolTable;
    std::map<Instruction *, std::vector<Value *> > liveness;
    const std::string name;
    int nextNameSuffix;
  };
}

#endif // VALUE_H
