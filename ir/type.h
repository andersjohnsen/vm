#ifndef TYPE_H
#define TYPE_H

#include <iostream>

namespace IR {
  class Type {
  protected:
    Type() {}

  public:
    virtual void print(std::ostream &stream) const = 0;
  };

  class VoidType : public Type {
  private:
    VoidType() {
    }

  public:
    void print(std::ostream &stream) const;

  public:
    static VoidType *get() { return instance; }

  private:
    static VoidType *instance;
  };

  class IntType : public Type {
  private:
    IntType() {
    }

  public:
    void print(std::ostream &stream) const;

  public:
    static IntType *get() { return instance; }

  private:
    static IntType *instance;
  };
}

#endif // TYPE_H
