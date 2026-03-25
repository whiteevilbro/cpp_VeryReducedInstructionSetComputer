#ifndef EMULATOR_H_
#define EMULATOR_H_

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace Emulator {
  typedef std::uint32_t word;

  enum Reg {
    R0 = 0x0,
    R1 = 0x1,
    R2 = 0x2,
    R3 = 0x3,
  };

  namespace Instructions {
    class MovReg;
    class MovImm;
    class AddReg;
    class AddImm;
    class SubReg;
    class SubImm;
    class MulReg;
    class MulImm;
    class DivReg;
    class DivImm;
    class Load;
    class Store;
    class Jmp;
    class Jmpz;
  }; // namespace Instructions


  class EmulatorState;

  class Instruction {
  public:
    virtual void eval(EmulatorState& state) const = 0;
    virtual ~Instruction(){};
  };

  class EmulatorState {
    friend Emulator::Instructions::MovReg;
    friend Emulator::Instructions::MovImm;
    friend Emulator::Instructions::AddReg;
    friend Emulator::Instructions::AddImm;
    friend Emulator::Instructions::SubReg;
    friend Emulator::Instructions::SubImm;
    friend Emulator::Instructions::MulReg;
    friend Emulator::Instructions::MulImm;
    friend Emulator::Instructions::DivReg;
    friend Emulator::Instructions::DivImm;
    friend Emulator::Instructions::Load;
    friend Emulator::Instructions::Store;
    friend Emulator::Instructions::Jmp;
    friend Emulator::Instructions::Jmpz;
    friend word emulate(const std::vector<Instruction*>& program);

    static const size_t regs_size = 4;
    static const size_t mem_size  = 1024;

    std::vector<word> registers = std::vector<word>(regs_size);
    std::vector<word> mem       = std::vector<word>(mem_size);

    size_t pc; // I really don't like this decision, but otherwise it wouldn't make sense to make insctuctions into objects
  };

  word emulate(const std::vector<Instruction*>& program);
  word emulate(const std::string& program);

  std::vector<Instruction*> parse(const std::string& program);

  namespace Instructions {
    class MovReg: public Instruction {
      Reg to, from;

    public:
      MovReg(Reg to_, Reg from_):
          to(to_), from(from_) {}

      void eval(EmulatorState& state) const override final {
        state.registers[static_cast<size_t>(to)] = state.registers[static_cast<size_t>(from)];
      }

      ~MovReg() = default;
    };

    class MovImm: public Instruction {
      Reg to;
      word imm;

    public:
      MovImm(Reg to_, word imm_):
          to(to_), imm(imm_) {}

      void eval(EmulatorState& state) const override final {
        state.registers[static_cast<size_t>(to)] = imm;
      }

      ~MovImm() = default;
    };

    class AddReg: public Instruction {
      Reg to, from;

    public:
      AddReg(Reg to_, Reg from_):
          to(to_), from(from_) {}

      void eval(EmulatorState& state) const override final {
        state.registers[static_cast<size_t>(to)] += state.registers[static_cast<size_t>(from)];
      }

      ~AddReg() = default;
    };

    class AddImm: public Instruction {
      Reg to;
      word imm;

    public:
      AddImm(Reg to_, word imm_):
          to(to_), imm(imm_) {}

      void eval(EmulatorState& state) const override final {
        state.registers[static_cast<size_t>(to)] += imm;
      }

      ~AddImm() = default;
    };

    class SubReg: public Instruction {
      Reg to, from;

    public:
      SubReg(Reg to_, Reg from_):
          to(to_), from(from_) {}

      void eval(EmulatorState& state) const override final {
        state.registers[static_cast<size_t>(to)] -= state.registers[static_cast<size_t>(from)];
      }

      ~SubReg() = default;
    };

    class SubImm: public Instruction {
      Reg to;
      word imm;

    public:
      SubImm(Reg to_, word imm_):
          to(to_), imm(imm_) {}

      void eval(EmulatorState& state) const override final {
        state.registers[static_cast<size_t>(to)] -= imm;
      }

      ~SubImm() = default;
    };

    class MulReg: public Instruction {
      Reg to, from;

    public:
      MulReg(Reg to_, Reg from_):
          to(to_), from(from_) {}

      void eval(EmulatorState& state) const override final {
        state.registers[static_cast<size_t>(to)] *= state.registers[static_cast<size_t>(from)];
      }

      ~MulReg() = default;
    };

    class MulImm: public Instruction {
      Reg to;
      word imm;

    public:
      MulImm(Reg to_, word imm_):
          to(to_), imm(imm_) {}

      void eval(EmulatorState& state) const override final {
        state.registers[static_cast<size_t>(to)] *= imm;
      }

      ~MulImm() = default;
    };

    class DivReg: public Instruction {
      Reg to, from;

    public:
      DivReg(Reg to_, Reg from_):
          to(to_), from(from_) {}

      void eval(EmulatorState& state) const override final {
        state.registers[static_cast<size_t>(to)] /= state.registers[static_cast<size_t>(from)];
      }

      ~DivReg() = default;
    };

    class DivImm: public Instruction {
      Reg to;
      word imm;

    public:
      DivImm(Reg to_, word imm_):
          to(to_), imm(imm_) {}

      void eval(EmulatorState& state) const override final {
        state.registers[static_cast<size_t>(to)] /= imm;
      }

      ~DivImm() = default;
    };

    class Load: public Instruction {
      Reg to;
      word address;

    public:
      Load(Reg to_, word address_):
          to(to_), address(address_) {}

      void eval(EmulatorState& state) const override final {
        state.registers[static_cast<size_t>(to)] = state.mem[static_cast<size_t>(address)];
      }

      ~Load() = default;
    };

    class Store: public Instruction {
      Reg from;
      word address;

    public:
      Store(Reg from_, word address_):
          from(from_), address(address_) {}

      void eval(EmulatorState& state) const override final {
        state.mem[static_cast<size_t>(address)] = state.registers[static_cast<size_t>(from)];
      }

      ~Store() = default;
    };

    class Jmp: public Instruction {
      word address;

    public:
      Jmp(word address_):
          address(--address_) {}

      void eval(EmulatorState& state) const override final {
        state.pc = address;
      }

      ~Jmp() = default;
    };

    class Jmpz: public Instruction {
      word address;

    public:
      Jmpz(word address_):
          address(--address_) {}

      void eval(EmulatorState& state) const override final {
        if (state.registers[static_cast<size_t>(R0)] == 0)
          state.pc = address;
      }

      ~Jmpz() = default;
    };
  } // namespace Instructions
} // namespace Emulator

#endif
