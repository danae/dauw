#include "disassemble.hpp"

namespace dauw
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
    auto location = code_->location_at(offset);
    auto byte = code_->byte_at(offset);
    fmt::print("{:04X}  {:>4d}:{:<4d}  {:02X} ", offset, location.line(), location.col(), byte);

    // Select the instruction
    auto instruction = static_cast<Instruction>(byte);
    switch (instruction)
    {
      case Instruction::NOP:
        return disassemble_simple("nop", offset);
      case Instruction::RETURN:
        return disassemble_simple("return", offset);

      case Instruction::NIL:
        return disassemble_simple("nil", offset);
      case Instruction::BOOL_FALSE:
        return disassemble_simple("bool.false", offset);
      case Instruction::BOOL_TRUE:
        return disassemble_simple("bool.true", offset);

      case Instruction::INT_CONST:
        return disassemble_constant("int.const", offset);
      case Instruction::REAL_CONST:
        return disassemble_constant("real.const", offset);
      case Instruction::RUNE_CONST:
        return disassemble_constant("rune.const", offset);

      case Instruction::INT_ISZERO:
        return disassemble_simple("int.iszero", offset);
      case Instruction::INT_CMP:
        return disassemble_simple("int.cmp", offset);
      case Instruction::INT_EQ:
        return disassemble_simple("int.eq", offset);
      case Instruction::INT_NEQ:
        return disassemble_simple("int.neq", offset);
      case Instruction::INT_LT:
        return disassemble_simple("int.lt", offset);
      case Instruction::INT_LTE:
        return disassemble_simple("int.lte", offset);
      case Instruction::INT_GT:
        return disassemble_simple("int.gt", offset);
      case Instruction::INT_GTE:
        return disassemble_simple("int.gte", offset);
      case Instruction::INT_ADD:
        return disassemble_simple("int.add", offset);
      case Instruction::INT_SUB:
        return disassemble_simple("int.sub", offset);
      case Instruction::INT_MUL:
        return disassemble_simple("int.mul", offset);
      case Instruction::INT_FDIV:
        return disassemble_simple("int.fdiv", offset);
      case Instruction::INT_MOD:
        return disassemble_simple("int.mod", offset);
      case Instruction::INT_NEG:
        return disassemble_simple("int.neg", offset);
      case Instruction::INT_ABS:
        return disassemble_simple("int.abs", offset);
      case Instruction::INT_SIGN:
        return disassemble_simple("int.sign", offset);
      case Instruction::INT_CONV_REAL:
        return disassemble_simple("int.conv_real", offset);
      case Instruction::INT_CONV_RUNE:
        return disassemble_simple("int.conv_rune", offset);

      case Instruction::REAL_ISNAN:
        return disassemble_simple("real.isnan", offset);
      case Instruction::REAL_ISZERO:
        return disassemble_simple("real.iszero", offset);
      case Instruction::REAL_CMP:
        return disassemble_simple("real.cmp", offset);
      case Instruction::REAL_EQ:
        return disassemble_simple("real.eq", offset);
      case Instruction::REAL_NEQ:
        return disassemble_simple("real.neq", offset);
      case Instruction::REAL_LT:
        return disassemble_simple("real.lt", offset);
      case Instruction::REAL_LTE:
        return disassemble_simple("real.lte", offset);
      case Instruction::REAL_GT:
        return disassemble_simple("real.gt", offset);
      case Instruction::REAL_GTE:
        return disassemble_simple("real.gte", offset);
      case Instruction::REAL_ADD:
        return disassemble_simple("real.add", offset);
      case Instruction::REAL_SUB:
        return disassemble_simple("real.sub", offset);
      case Instruction::REAL_MUL:
        return disassemble_simple("real.mul", offset);
      case Instruction::REAL_DIV:
        return disassemble_simple("real.div", offset);
      case Instruction::REAL_FDIV:
        return disassemble_simple("real.fdiv", offset);
      case Instruction::REAL_MOD:
        return disassemble_simple("real.mod", offset);
      case Instruction::REAL_NEG:
        return disassemble_simple("real.neg", offset);
      case Instruction::REAL_ABS:
        return disassemble_simple("real.abs", offset);
      case Instruction::REAL_SIGN:
        return disassemble_simple("real.sign", offset);
      case Instruction::REAL_CONV_TRUNC:
        return disassemble_simple("real.conv_trunc", offset);
      case Instruction::REAL_CONV_FLOOR:
        return disassemble_simple("real.conv_floor", offset);
      case Instruction::REAL_CONV_CEIL:
        return disassemble_simple("real.conv_ceil", offset);

      case Instruction::RUNE_CONV_INT:
        return disassemble_simple("rune.conv_int", offset);

      default:
        fmt::print("<Unknown instruction>\n", static_cast<uint8_t>(instruction));
        return offset + 1;
    }
  }

  // Disassemble an instruction without arguments
  size_t Disassembler::disassemble_simple(string_t name, size_t offset)
  {
    fmt::print("{:16s}\n", name);
    return offset + 1;
  }

  // Disassemble a constant instruction
  size_t Disassembler::disassemble_constant(string_t name, size_t offset)
  {
    auto constant_index = code_->byte_at(offset + 1);
    auto constant = code_->constant_at(constant_index);
    fmt::print("{:16s} {} ({})\n", name, constant_index, constant);
    return offset + 2;
  }
}
