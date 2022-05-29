#include "vm.hpp"

namespace dauw::backend
{
  // Constructor
  VM::VM(Reporter* reporter)
    : ReporterAware(reporter)
  {
  }

  // Descructor
  VM::~VM()
  {
    while (!objects_.empty())
    {
      auto object = objects_.front();
      objects_.pop_front();

      delete object;
    }
  }

  // Allocate a string
  internals::String* VM::VM::allocate_string(const char* bytes)
  {
    auto string = new internals::String(bytes);
    objects_.push_front(string);
    return string;
  }

  // Execute a block of code
  VMResult VM::run(Code* code)
  {
    // Set the block of code_
    code_ = code;

    // Set the disassembler for the block of code
    Disassembler disassembler(code_);

    // Iterate over the bytes in the block of code using the instruction pointer
    for (ip_ = 0; ip_ < code_->bytes_length(); ip_ ++)
    {
      // Catch runtime errors
      try
      {
        //#define DAUW_DEBUG
        #ifdef DAUW_DEBUG
          // DEBUG: Print the values currently on the stack
          for (auto value : stack_)
            fmt::print(fmt::fg(fmt::color::gray), "[{}] ", value);
          fmt::print(fmt::fg(fmt::color::gray), "\n");

          // DEBUG: Disassemble the code
          disassembler.disassemble(ip_);
        #endif

        // Handle the instruction at the instruction pointer
        auto instruction = static_cast<Instruction>(current_byte());
        switch (instruction)
        {
          // Valid instruction
          case Instruction::NOP: break;
          case Instruction::NIL: run_nil(); break;
          case Instruction::FALSE: run_false(); break;
          case Instruction::TRUE: run_true(); break;
          case Instruction::ICONST: run_iconst(code_->byte_at(++ip_)); break;
          case Instruction::RCONST: run_rconst(code_->byte_at(++ip_)); break;
          case Instruction::UCONST: run_uconst(code_->byte_at(++ip_)); break;
          case Instruction::NOT: run_not(); break;
          case Instruction::INEG: run_ineg(); break;
          case Instruction::RNEG: run_rneg(); break;
          case Instruction::IABS: run_iabs(); break;
          case Instruction::RABS: run_rabs(); break;
          case Instruction::ISIGN: run_isign(); break;
          case Instruction::RSIGN: run_rsign(); break;
          case Instruction::IADD: run_iadd(); break;
          case Instruction::RADD: run_radd(); break;
          case Instruction::ISUB: run_isub(); break;
          case Instruction::RSUB: run_rsub(); break;
          case Instruction::IMUL: run_imul(); break;
          case Instruction::RMUL: run_rmul(); break;
          case Instruction::IDIV: run_idiv(); break;
          case Instruction::RDIV: run_rdiv(); break;
          case Instruction::IQUO: run_iquo(); break;
          case Instruction::RQUO: run_rquo(); break;
          case Instruction::IREM: run_irem(); break;
          case Instruction::RREM: run_rrem(); break;
          case Instruction::BEQ: run_beq(); break;
          case Instruction::ICMP: run_icmp(); break;
          case Instruction::RCMP: run_rcmp(); break;
          case Instruction::UCMP: run_ucmp(); break;
          case Instruction::IEQZ: run_ieqz(); break;
          case Instruction::INEZ: run_inez(); break;
          case Instruction::ILTZ: run_iltz(); break;
          case Instruction::ILEZ: run_ilez(); break;
          case Instruction::IGTZ: run_igtz(); break;
          case Instruction::IGEZ: run_igez(); break;
          case Instruction::REQZ: run_reqz(); break;
          case Instruction::RNAN: run_rnan(); break;
          case Instruction::ITOR: run_itor(); break;
          case Instruction::ITOU: run_itou(); break;
          case Instruction::RTRUNC: run_rtrunc(); break;
          case Instruction::RFLOOR: run_rfloor(); break;
          case Instruction::RCEIL: run_rceil(); break;
          case Instruction::UTOI: run_utoi(); break;
          case Instruction::POP: run_pop(); break;
          case Instruction::ECHO: run_echo(); break;

          // Invalid instruction
          default:
            report<RuntimeError>(current_byte_location(), fmt::format("Invalid virtual machine instruction {:02X}", current_byte()));
            return VMResult::RUNTIME_ERROR;
        }
      }
      catch (RuntimeError& ex)
      {
        // Report a runtime error
        report<RuntimeError>(ex.location(), ex.message(), ex.previous());

        // Exit the virtual machine with an error
        return VMResult::RUNTIME_ERROR;
      }
      catch (Exception& ex)
      {
        // Report a runtime error
        report<RuntimeError>(current_byte_location(), ex.message(), ex.previous());

        // Exit the virtual machine with an error
        return VMResult::COMPILE_ERROR;
      }
    }

    return VMResult::SUCCESS;
  }

  // Execute a NIL instruction
  void VM::run_nil()
  {
    push_stack(internals::Value::value_nothing);
  }

  // Execute a FALSE instruction
  void VM::run_false()
  {
    push_stack(internals::Value::value_false);
  }

  // Execute a TRUE instruction
  void VM::run_true()
  {
    push_stack(internals::Value::value_true);
  }

  // Execute a ICONST instruction
  void VM::run_iconst(size_t constant_index)
  {
    // TODO: Add type check for the constant
    auto constant = code_->constant_at(constant_index);
    push_stack(constant);
  }

  // Execute a RCONST instruction
  void VM::run_rconst(size_t constant_index)
  {
    // TODO: Add type check for the constant
    auto constant = code_->constant_at(constant_index);
    push_stack(constant);
  }

  // Execute a UCONST instruction
  void VM::run_uconst(size_t constant_index)
  {
    // TODO: Add type check for the constant
    auto constant = code_->constant_at(constant_index);
    push_stack(constant);
  }

  // Execute a NOT instruction
  void VM::run_not()
  {
    auto a = pop_stack().as_bool();
    push_stack(internals::Value::of_bool(!a));
  }

  // Execute a INEG instruction
  void VM::run_ineg()
  {
    auto a = pop_stack().as_int();
    push_stack(internals::Value::of_int(-a));
  }

  // Execute a RNEG instruction
  void VM::run_rneg()
  {
    auto a = pop_stack().as_real();
    push_stack(internals::Value::of_real(-a));
  }

  // Execute a IABS instruction
  void VM::run_iabs()
  {
    auto a = pop_stack().as_int();
    push_stack(internals::Value::of_int(std::abs(a)));
  }

  // Execute a RABS instruction
  void VM::run_rabs()
  {
    auto a = pop_stack().as_real();
    push_stack(internals::Value::of_real(std::abs(a)));
  }

  // Execute a ISIGN instruction
  void VM::run_isign()
  {
    auto a = pop_stack().as_int();
    push_stack(internals::Value::of_int(utils::sign(a)));
  }

  // Execute a RSIGN instruction
  void VM::run_rsign()
  {
    auto a = pop_stack().as_real();
    push_stack(internals::Value::of_real(utils::sign(a)));
  }

  // Execute a IADD instruction
  void VM::run_iadd()
  {
    auto b = pop_stack().as_int();
    auto a = pop_stack().as_int();

    push_stack(internals::Value::of_int(a + b));
  }

  // Execute a RADD instruction
  void VM::run_radd()
  {
    auto b = pop_stack().as_real();
    auto a = pop_stack().as_real();

    push_stack(internals::Value::of_real(a + b));
  }

  // Execute a ISUB instruction
  void VM::run_isub()
  {
    auto b = pop_stack().as_int();
    auto a = pop_stack().as_int();

    push_stack(internals::Value::of_int(a - b));
  }

  // Execute a RSUB instruction
  void VM::run_rsub()
  {
    auto b = pop_stack().as_real();
    auto a = pop_stack().as_real();

    push_stack(internals::Value::of_real(a - b));
  }

  // Execute a IMUL instruction
  void VM::run_imul()
  {
    auto b = pop_stack().as_int();
    auto a = pop_stack().as_int();

    push_stack(internals::Value::of_int(a * b));
  }

  // Execute a RMUL instruction
  void VM::run_rmul()
  {
    auto b = pop_stack().as_real();
    auto a = pop_stack().as_real();

    push_stack(internals::Value::of_real(a * b));
  }

  // Execute a IDIV instruction
  void VM::run_idiv()
  {
    auto b = pop_stack().as_int();
    auto a = pop_stack().as_int();

    push_stack(internals::Value::of_real(static_cast<dauw_real_t>(a) / static_cast<dauw_real_t>(b)));
  }

  // Execute a RDIV instruction
  void VM::run_rdiv()
  {
    auto b = pop_stack().as_real();
    auto a = pop_stack().as_real();

    push_stack(internals::Value::of_real(a / b));
  }

  // Execute a IQUO instruction
  void VM::run_iquo()
  {
    auto b = pop_stack().as_int();
    auto a = pop_stack().as_int();

    push_stack(internals::Value::of_int(utils::floordiv(a, b)));
  }

  // Execute a RQUO instruction
  void VM::run_rquo()
  {
    auto b = pop_stack().as_real();
    auto a = pop_stack().as_real();

    push_stack(internals::Value::of_real(utils::floordiv(a, b)));
  }

  // Execute a IREM instruction
  void VM::run_irem()
  {
    auto b = pop_stack().as_int();
    auto a = pop_stack().as_int();

    push_stack(internals::Value::of_int(utils::floormod(a, b)));
  }

  // Execute a RREM instruction
  void VM::run_rrem()
  {
    auto b = pop_stack().as_real();
    auto a = pop_stack().as_real();

    push_stack(internals::Value::of_real(utils::floormod(a, b)));
  }

  // Execute a BEQ instruction
  void VM::run_beq()
  {
    auto a = pop_stack().as_bool();
    auto b = pop_stack().as_bool();

    push_stack(internals::Value::of_bool(a == b));
  }

  // Execute a ICMP instruction
  void VM::run_icmp()
  {
    auto b = pop_stack().as_int();
    auto a = pop_stack().as_int();

    if (a < b)
      push_stack(internals::Value::of_int(-1));
    else if (a > b)
      push_stack(internals::Value::of_int(1));
    else
      push_stack(internals::Value::of_int(0));
  }

  // Execute a RCMP instruction
  void VM::run_rcmp()
  {
    auto b = pop_stack().as_real();
    auto a = pop_stack().as_real();

    if (a < b)
      push_stack(internals::Value::of_int(-1));
    else if (a > b)
      push_stack(internals::Value::of_int(1));
    else
      push_stack(internals::Value::of_int(0));
  }

  // Execute a UCMP instruction
  void VM::run_ucmp()
  {
    auto b = pop_stack().as_rune();
    auto a = pop_stack().as_rune();

    if (a < b)
      push_stack(internals::Value::of_int(-1));
    else if (a > b)
      push_stack(internals::Value::of_int(1));
    else
      push_stack(internals::Value::of_int(0));
  }

  // Execute a IEQZ instruction
  void VM::run_ieqz()
  {
    auto a = pop_stack().as_int();
    push_stack(internals::Value::of_bool(a == 0));
  }

  // Execute a INEZ instruction
  void VM::run_inez()
  {
    auto a = pop_stack().as_int();
    push_stack(internals::Value::of_bool(a != 0));
  }

  // Execute a ILTZ instruction
  void VM::run_iltz()
  {
    auto a = pop_stack().as_int();
    push_stack(internals::Value::of_bool(a < 0));
  }

  // Execute a ILEZ instruction
  void VM::run_ilez()
  {
    auto a = pop_stack().as_int();
    push_stack(internals::Value::of_bool(a <= 0));
  }

  // Execute a IGTZ instruction
  void VM::run_igtz()
  {
    auto a = pop_stack().as_int();
    push_stack(internals::Value::of_bool(a > 0));
  }

  // Execute a IGEZ instruction
  void VM::run_igez()
  {
    auto a = pop_stack().as_int();
    push_stack(internals::Value::of_bool(a >= 0));
  }

  // Execute a REQZ instruction
  void VM::run_reqz()
  {
    auto a = pop_stack().as_real();
    push_stack(internals::Value::of_bool(a == 0.0));
  }

  // Execute a RNAN instruction
  void VM::run_rnan()
  {
    auto a = pop_stack().as_real();
    push_stack(internals::Value::of_bool(std::isnan(a)));
  }

  // Execute a ITOR instruction
  void VM::run_itor()
  {
    auto a = pop_stack().as_int();
    push_stack(internals::Value::of_real(static_cast<dauw_real_t>(a)));
  }

  // Execute a ITOU instruction
  void VM::run_itou()
  {
    auto a = pop_stack().as_int();
    push_stack(internals::Value::of_rune(static_cast<dauw_real_t>(a)));
  }

  // Execute a RTRUNC instruction
  void VM::run_rtrunc()
  {
    auto a = pop_stack().as_real();
    if (std::isnan(a))
      report<ConversionError>(current_byte_location(), "Cannot truncate a NaN real value");

    push_stack(internals::Value::of_int(static_cast<dauw_real_t>(std::trunc(a))));
  }

  // Execute a RFLOOR instruction
  void VM::run_rfloor()
  {
    auto a = pop_stack().as_real();
    if (std::isnan(a))
      report<ConversionError>(current_byte_location(), "Cannot floor a NaN real value");

    push_stack(internals::Value::of_int(static_cast<dauw_real_t>(std::floor(a))));
  }

  // Execute a RCEIL instruction
  void VM::run_rceil()
  {
    auto a = pop_stack().as_real();
    if (std::isnan(a))
      report<ConversionError>(current_byte_location(), "Cannot ceil a NaN real value");

    push_stack(internals::Value::of_int(static_cast<dauw_real_t>(std::ceil(a))));
  }

  // Execute a UTOI instruction
  void VM::run_utoi()
  {
    auto a = pop_stack().as_rune();
    push_stack(internals::Value::of_int(static_cast<dauw_real_t>(a)));
  }

  // Execute a POP instruction
  void VM::run_pop()
  {
    pop_stack();
  }

  // Execute a ECHO instruction
  void VM::run_echo()
  {
    auto value = pop_stack();
    fmt::print("{}\n", value);
  }

  // Push a value onto the stack of the virtual machine
  void VM::push_stack(internals::Value value)
  {
    try
    {
      stack_.push_back(value);
    }
    catch (std::bad_alloc& ex)
    {
      report<StackOverflowError>(current_byte_location(), "Stack overflow");
    }
  }

  // Pop a value from the stack of the virtual machine
  internals::Value VM::pop_stack()
  {
    if (stack_.empty())
      report<StackUnderflowError>(current_byte_location(), "Stack underflow");

    auto value = stack_.back();
    stack_.pop_back();
    return value;
  }
}
