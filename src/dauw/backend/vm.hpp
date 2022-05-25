#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/backend/code.hpp>
#include <dauw/backend/disassembler.hpp>
#include <dauw/internals/object.hpp>
#include <dauw/internals/string_object.hpp>
#include <dauw/internals/value.hpp>

#include <cmath>
#include <cstdlib>
#include <forward_list>
#include <new>


namespace dauw::backend
{
  // Enum that defines the result of executing code in the virtual machine
  enum class VMResult
  {
    SUCCESS,        // Exit the VM with success
    COMPILE_ERROR,  // Exit the VM with a compile error
    RUNTIME_ERROR,  // Exit the VM with a runtime error
  };


  // Class that defines the virtual machine
  class VM : public ReporterAware
  {
    private:
      // The code being executed by the virtual machine
      Code* code_;

      // The instruction pointer of the code being executed
      size_t ip_;

      // The stack of the virtual machine
      std::vector<internals::Value> stack_;

      // Linked list of defined objects
      std::forward_list<internals::Obj*> objects_;


    public:
      // Constructor
      VM(Reporter* reporter);

      // Descructor
      ~VM();

      // Allocate a string
      internals::String* allocate_string(const char* bytes);

      // Execute a block of code
      VMResult run(Code* code);

      // Execute an instruction
      void run_nil();
      void run_false();
      void run_true();
      void run_iconst(size_t constant_index);
      void run_rconst(size_t constant_index);
      void run_uconst(size_t constant_index);
      void run_not();
      void run_ineg();
      void run_rneg();
      void run_iabs();
      void run_rabs();
      void run_isign();
      void run_rsign();
      void run_iadd();
      void run_radd();
      void run_isub();
      void run_rsub();
      void run_imul();
      void run_rmul();
      void run_idiv();
      void run_rdiv();
      void run_iquo();
      void run_rquo();
      void run_irem();
      void run_rrem();
      void run_beq();
      void run_icmp();
      void run_rcmp();
      void run_ucmp();
      void run_ieqz();
      void run_inez();
      void run_iltz();
      void run_ilez();
      void run_igtz();
      void run_igez();
      void run_reqz();
      void run_rnan();
      void run_itor();
      void run_itou();
      void run_rtrunc();
      void run_rfloor();
      void run_rceil();
      void run_utoi();
      void run_pop();
      void run_echo();

      // Manipulate the stack of the virtual machine
      void push_stack(internals::Value value);
      internals::Value pop_stack();

      // Return the byte or location corresponding to the code at the instruction pointer
      inline uint8_t current_byte() { return code_->byte_at(ip_); }
      inline frontend::Location current_byte_location() { return code_->byte_location_at(ip_); }
  };
}
