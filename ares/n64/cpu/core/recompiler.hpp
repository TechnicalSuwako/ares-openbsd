//{
  struct Allocator {
    auto construct() -> void {
      destruct();
      memory = (u8*)calloc(512_MiB, 1);
      offset = 0;
      mprotect(memory, 512_MiB, PROT_READ | PROT_WRITE | PROT_EXEC);
    }

    auto destruct() -> void {
      free(memory);
      memory = nullptr;
    }

    u8* memory = nullptr;
    u32 offset = 0;
  } allocator;

  struct Block {
    auto execute() -> void {
      auto function = (void (*)())code;
      function();
    }

    u32 step;
    u32 size;
    u8* code;
  };

  struct Pool {
    static auto allocate() -> Pool*;

    Block* blocks[1 << 8];
  };

  auto recompile(u32 address) -> Block*;
  auto recompileEXECUTE(u32 instruction) -> bool;
  auto recompileSPECIAL(u32 instruction) -> bool;
  auto recompileREGIMM(u32 instruction) -> bool;
  auto recompileCOP0(u32 instruction) -> bool;
  auto recompileCOP1(u32 instruction) -> bool;
  auto recompileCheckCOP1() -> bool;

  struct Recompiler {
    auto reset() -> void {
      for(uint index : range(1 << 19)) pools[index] = nullptr;
    }

    Pool* pools[1 << 19];
  } recompiler;
//};
