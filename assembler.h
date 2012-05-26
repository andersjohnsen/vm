#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdint.h>

template<unsigned long N>
struct bin {
    enum { value = (N%10)+2*bin<N/10>::value };
} ;

template<>
struct bin<0> {
    enum { value = 0 };
};

class Page {
public:
  Page(intptr_t min_size);

public:
  bool makeExecutable();

  uint8_t *getData() const { return page; }

  template<typename T>
  void setAt(intptr_t offset, T t) {
    reinterpret_cast<T*>(page + offset)[0] = t;
  }

private:
  uint8_t* page;
  intptr_t size;
};

class Assembler {
public:
  enum Register {
    RAX,
    RBX
  };

  const static uint8_t REX_W = bin<1001000>::value;

public:
  Assembler(Page *page);

public:
  void add(Register rm64, Register r64);

  void mov(Register rm64, Register r64);
  void mov(uint64_t imm64, Register r64);

  void ret();

private:
  uint8_t rd(Register w);

  uint8_t modRM(Register m, Register r);

  const char *registerToString(Register r) {
    switch (r) {
      case RAX: return "%rax";
      case RBX: return "%rbx";
    }
    return "";
  }

private:
  Page *page;
  intptr_t offset;
};

#endif // ASSEMBLER_H
