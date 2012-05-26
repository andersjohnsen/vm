#include <iostream>

#include <stdlib.h>
#include <sys/mman.h>

#include "assembler.h"

Page::Page(intptr_t min_size) {
  std::cout << "Creating Page of minimum " << min_size << " bytes\n";

  intptr_t page_size = sysconf(_SC_PAGESIZE);
  std::cout << " - OS memory page size: " << page_size << "\n";

  size = (min_size / page_size) * page_size + (min_size % page_size > 0 ? page_size : 0);
  std::cout << " - Allocating " << size << " bytes\n";

  posix_memalign(reinterpret_cast<void **>(&page), page_size, size);
  std::cout << " - Page start address: " << reinterpret_cast<void **>(page) << "\n";
}

bool Page::makeExecutable() {
  bool success = mprotect(page,
                          size,
                          PROT_EXEC | PROT_READ | PROT_WRITE) == 0;
  if (success) {
    std::cout << "Made page " << reinterpret_cast<void **>(page) << " executable\n";
  } else {
    std::cout << "Failed to make page " << reinterpret_cast<void **>(page) << " executable\n";
  }
  return success;
}

Assembler::Assembler(Page *page) :
    page(page), offset(0) {
}

void Assembler::add(Register rm64, Register r64) {
  page->setAt(offset++, REX_W);
  page->setAt(offset++, (uint8_t)0x03);
  page->setAt(offset++, modRM(rm64, r64));
  std::cout << "  add   "
            << registerToString(rm64)
            << ","
            << registerToString(r64)
            << "\n";
}

void Assembler::mov(Register rm64, Register r64) {
  page->setAt(offset++, REX_W);
  page->setAt(offset++, (uint8_t)0x8B);
  page->setAt(offset++, modRM(rm64, r64));
  std::cout << "  mov   "
            << registerToString(rm64)
            << ","
            << registerToString(r64)
            << "\n";
}

void Assembler::mov(uint64_t imm64, Register r64) {
  page->setAt(offset++, REX_W);
  page->setAt(offset++, (uint8_t)(0xB8 + rd(r64)));
  page->setAt(offset, imm64);
  offset += sizeof(imm64);
  std::cout << "  mov   $"
            << imm64
            << ","
            << registerToString(r64)
            << "\n";
}

void Assembler::ret() {
  page->setAt(offset++, (uint8_t)0xC3);
  std::cout << "  ret\n";
}

uint8_t Assembler::rd(Register w) {
  switch (w) {
    case RAX: return 0;
    case RBX: return 1;

    default:
      std::cout << "Invalid register '" << w << "' in 'Assembler::rd\n'";
      exit(1);
  }
  return 0;
}

uint8_t Assembler::modRM(Register m, Register r) {
  return bin<11000000>::value | rd(r) << 3 | rd(m);
}
