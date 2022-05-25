#include "disassembler.hpp"

namespace dauw::backend
{
  // Constructor for a disassembler
  Disassembler::Disassembler(Code* code)
    : code_(code)
  {
  }

  // Disassemble the byte at the given offset and return the next offset
  size_t Disassembler::disassemble(size_t offset)
  {
    // Print the location of the byte
    auto location = code_->byte_location_at(offset);
    auto byte = code_->byte_at(offset);
    fmt::print(fmt::fg(fmt::color::gray), "{:04X}  {:>4d}:{:<4d}  {:02X} ", offset, location.line() + 1, location.col() + 1, byte);

    // Select the instruction
    auto instruction = static_cast<Instruction>(byte);
    switch (instruction)
    {
      // Valid instruction
      case Instruction::NOP: return disassemble_simple("nop", offset);
      case Instruction::NIL: return disassemble_simple("nil", offset);
      case Instruction::FALSE: return disassemble_simple("false", offset);
      case Instruction::TRUE: return disassemble_simple("true", offset);
      case Instruction::ICONST: return disassemble_constant("iconst", offset);
      case Instruction::RCONST: return disassemble_constant("rconst", offset);
      case Instruction::UCONST: return disassemble_constant("uconst", offset);
      case Instruction::NOT: return disassemble_simple("not", offset);
      case Instruction::INEG: return disassemble_simple("ineg", offset);
      case Instruction::RNEG: return disassemble_simple("rneg", offset);
      case Instruction::IABS: return disassemble_simple("iabs", offset);
      case Instruction::RABS: return disassemble_simple("rabs", offset);
      case Instruction::ISIGN: return disassemble_simple("isign", offset);
      case Instruction::RSIGN: return disassemble_simple("rsign", offset);
      case Instruction::IADD: return disassemble_simple("iadd", offset);
      case Instruction::RADD: return disassemble_simple("radd", offset);
      case Instruction::ISUB: return disassemble_simple("isub", offset);
      case Instruction::RSUB: return disassemble_simple("rsub", offset);
      case Instruction::IMUL: return disassemble_simple("imul", offset);
      case Instruction::RMUL: return disassemble_simple("rmul", offset);
      case Instruction::IDIV: return disassemble_simple("idiv", offset);
      case Instruction::RDIV: return disassemble_simple("rdiv", offset);
      case Instruction::IQUO: return disassemble_simple("iquo", offset);
      case Instruction::RQUO: return disassemble_simple("rquo", offset);
      case Instruction::IREM: return disassemble_simple("irem", offset);
      case Instruction::RREM: return disassemble_simple("rrem", offset);
      case Instruction::BEQ: return disassemble_simple("beq", offset);
      case Instruction::ICMP: return disassemble_simple("icmp", offset);
      case Instruction::RCMP: return disassemble_simple("rcmp", offset);
      case Instruction::UCMP: return disassemble_simple("ucmp", offset);
      case Instruction::IEQZ: return disassemble_simple("ieqz", offset);
      case Instruction::INEZ: return disassemble_simple("inez", offset);
      case Instruction::ILTZ: return disassemble_simple("iltz", offset);
      case Instruction::ILEZ: return disassemble_simple("ilez", offset);
      case Instruction::IGTZ: return disassemble_simple("igtz", offset);
      case Instruction::IGEZ: return disassemble_simple("igez", offset);
      case Instruction::REQZ: return disassemble_simple("reqz", offset);
      case Instruction::RNAN: return disassemble_simple("rnan", offset);
      case Instruction::ITOR: return disassemble_simple("itor", offset);
      case Instruction::ITOU: return disassemble_simple("itou", offset);
      case Instruction::RTRUNC: return disassemble_simple("rtrunc", offset);
      case Instruction::RFLOOR: return disassemble_simple("rfloor", offset);
      case Instruction::RCEIL: return disassemble_simple("rceil", offset);
      case Instruction::UTOI: return disassemble_simple("utoi", offset);
      case Instruction::POP: return disassemble_simple("pop", offset);
      case Instruction::ECHO: return disassemble_simple("echo", offset);

      // Invalid instruction
      default:
        fmt::print(fmt::fg(fmt::color::gray), "<unknown instruction>\n", static_cast<uint8_t>(instruction));
        return offset + 1;
    }
  }

  // Disassemble an instruction without arguments
  size_t Disassembler::disassemble_simple(string_t name, size_t offset)
  {
    fmt::print(fmt::fg(fmt::color::gray), "{:8s}\n", name);
    return offset + 1;
  }

  // Disassemble a constant instruction
  size_t Disassembler::disassemble_constant(string_t name, size_t offset)
  {
    auto constant_index = code_->byte_at(offset + 1);
    auto constant = code_->constant_at(constant_index);
    fmt::print(fmt::fg(fmt::color::gray), "{:8s} {} ({})\n", name, constant_index, constant);
    return offset + 2;
  }
}
