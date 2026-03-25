#include "emulator.hpp"

#include <algorithm>
#include <cctype>
#include <exception>
#include <sstream>
#include <vector>

using std::string;
using std::vector;

bool ichar_eq(char left, char right) {
  return std::tolower(left) == std::tolower(right);
}

bool istring_cmp(const string& left, const string& right) {
  return (left.size() == right.size()) && std::equal(left.begin(), left.end(), right.begin(), ichar_eq);
}

namespace Emulator {
  word emulate(const string& program) {
    vector<Instruction*> code = parse(program);
    int result                = emulate(code);
    for (size_t i = 0; i < code.size(); i++) {
      delete code[i];
    }
    return result;
  }

  word emulate(const vector<Instruction*>& program) {
    EmulatorState state;

    for (; state.pc < program.size(); state.pc++) {
      program[state.pc]->eval(state);
    }

    return state.registers[static_cast<size_t>(R0)];
  }

  vector<Instruction*> parse(const string& program) {
    std::stringstream ss(program);
    vector<Instruction*> result;

    string instr, left, right;
    while (true) {
      ss >> instr;
      if (ss.eof())
        break;
      // I absolutely LOVE parsing. Definitely.
      if (istring_cmp(instr, "mov")) {
        ss >> left >> right;
        if (std::tolower(right[0]) == 'r') {
          result.push_back(new Instructions::MovReg(static_cast<Reg>(std::stoi(left.substr(1))), static_cast<Reg>(std::stoi(right.substr(1)))));
        } else {
          result.push_back(new Instructions::MovImm(static_cast<Reg>(std::stoi(left.substr(1))), static_cast<word>(std::stoi(right))));
        }
      } else if (istring_cmp(instr, "add")) {
        ss >> left >> right;
        if (std::tolower(right[0]) == 'r') {
          result.push_back(new Instructions::AddReg(static_cast<Reg>(std::stoi(left.substr(1))), static_cast<Reg>(std::stoi(right.substr(1)))));
        } else {
          result.push_back(new Instructions::AddImm(static_cast<Reg>(std::stoi(left.substr(1))), static_cast<word>(std::stoi(right))));
        }
      } else if (istring_cmp(instr, "sub")) {
        ss >> left >> right;
        if (std::tolower(right[0]) == 'r') {
          result.push_back(new Instructions::SubReg(static_cast<Reg>(std::stoi(left.substr(1))), static_cast<Reg>(std::stoi(right.substr(1)))));
        } else {
          result.push_back(new Instructions::SubImm(static_cast<Reg>(std::stoi(left.substr(1))), static_cast<word>(std::stoi(right))));
        }
      } else if (istring_cmp(instr, "mul")) {
        ss >> left >> right;
        if (std::tolower(right[0]) == 'r') {
          result.push_back(new Instructions::MulReg(static_cast<Reg>(std::stoi(left.substr(1))), static_cast<Reg>(std::stoi(right.substr(1)))));
        } else {
          result.push_back(new Instructions::MulImm(static_cast<Reg>(std::stoi(left.substr(1))), static_cast<word>(std::stoi(right))));
        }
      } else if (istring_cmp(instr, "div")) {
        ss >> left >> right;
        if (std::tolower(right[0]) == 'r') {
          result.push_back(new Instructions::DivReg(static_cast<Reg>(std::stoi(left.substr(1))), static_cast<Reg>(std::stoi(right.substr(1)))));
        } else {
          result.push_back(new Instructions::DivImm(static_cast<Reg>(std::stoi(left.substr(1))), static_cast<word>(std::stoi(right))));
        }
      } else if (istring_cmp(instr, "load")) {
        ss >> left >> right;
        result.push_back(new Instructions::Load(static_cast<Reg>(std::stoi(left.substr(1))), static_cast<word>(std::stoi(right))));
      } else if (istring_cmp(instr, "store")) {
        ss >> left >> right;
        result.push_back(new Instructions::Store(static_cast<Reg>(std::stoi(left.substr(1))), static_cast<word>(std::stoi(right))));
      } else if (istring_cmp(instr, "jmp")) {
        ss >> left;
        result.push_back(new Instructions::Jmp(static_cast<word>(std::stoi(left))));
      } else if (istring_cmp(instr, "jmpz")) {
        ss >> left;
        result.push_back(new Instructions::Jmpz(static_cast<word>(std::stoi(left))));
      } else {
        throw std::exception();
      }
    }

    return result;
  }
} // namespace Emulator
