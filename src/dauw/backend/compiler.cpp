#include "compiler.hpp"

namespace dauw::backend
{
  // Constructor for the compiler
  Compiler::Compiler(Reporter* reporter, Code* code)
    : ReporterAware(reporter), code_(code)
  {
  }

  // Compile an expression
  void Compiler::compile(const ast::expr_ptr& expr)
  {
    expr->accept(shared_from_this());
  }

  // Compile a type expression
  void Compiler::compile(const ast::type_expr_ptr& expr)
  {
    expr->accept(shared_from_this());
  }

  // --------------------------------------------------------------------------
  // EMITTING BYTES TO THE CODE
  // --------------------------------------------------------------------------

  // Emit a byte to the code
  void Compiler::emit(Code::byte_type byte, frontend::Location location)
  {
    code_->append_byte(byte, location);
  }

  // Emit a list of bytes to the code
  void Compiler::emit(std::initializer_list<Code::byte_type> bytes, frontend::Location location)
  {
    for (Code::byte_type byte : bytes)
      emit(byte, location);
  }

  // Emit an instruction to the code
  void Compiler::emit(Instruction instruction, frontend::Location location)
  {
    emit(static_cast<Code::byte_type>(instruction), location);
  }

  // Emit an instruction followed by a byte to the code
  void Compiler::emit(Instruction instruction, Code::byte_type byte, frontend::Location location)
  {
    emit(instruction, location);
    emit(byte, location);
  }

  // Emit an instruction followed by a list of bytes to the code
  void Compiler::emit(Instruction instruction, std::initializer_list<Code::byte_type> bytes, frontend::Location location)
  {
    emit(instruction, location);
    emit(bytes, location);
  }

  // Emit a constant instruction to the code
  void Compiler::emit_constant(internals::Value value, frontend::Location location)
  {
    if (value.is_nothing())
      emit(Instruction::NIL, location);
    else if (value.is_false())
      emit(Instruction::FALSE, location);
    else if (value.is_true())
      emit(Instruction::TRUE, location);
    else if (value.is_int())
      emit(Instruction::ICONST, code_->append_constant(value), location);
    else if (value.is_real())
      emit(Instruction::RCONST, code_->append_constant(value), location);
    else if (value.is_rune())
      emit(Instruction::UCONST, code_->append_constant(value), location);

    // TODO: Handle invalid constant value
  }

  // --------------------------------------------------------------------------
  // EXPRESSION VISITOR IMPLEMENTATION
  // --------------------------------------------------------------------------

  // Visit a literal expression
  void Compiler::visit_literal(const ast::expr_literal_ptr& expr)
  {
    // Compile the value of the listeral
    emit_constant(expr->value(), expr->location());
  }

  // Visit a sequence expression
  void Compiler::visit_sequence(const ast::expr_sequence_ptr& expr)
  {
    // TODO: Implement compiling sequence expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling sequence expression");

    /*
    // Compile the items of the sequence expression
    for (auto item_expr : *expr)
      compile(item_expr);
    */
  }

  // Visit a record expression
  void Compiler::visit_record(const ast::expr_record_ptr& expr)
  {
    // TODO: Implement compiling record expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling record expression");

    /*
    // Compile the items of the record expression
    for (auto item_expr : *expr)
      compile(std::get<1>(item_expr));
    */
  }

  // Visit a name expression
  void Compiler::visit_name(const ast::expr_name_ptr& expr)
  {
    // TODO: Implement compiling name expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling name expression");
  }

  // Visit a function expression
  void Compiler::visit_function(const ast::expr_function_ptr& expr)
  {
    // TODO: Implement compiling function expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling function expression");
  }

  // Visit a function parameter expression
  void Compiler::visit_function_parameter(const ast::expr_function_parameter_ptr& expr)
  {
    // TODO: Implement compiling function parameter expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling function parameter expression");
  }

  // Visit a grouped expression
  void Compiler::visit_grouped(const ast::expr_grouped_ptr& expr)
  {
    // Compile the nested expression
    compile(expr->expr());
  }

  // Visit a call expression
  void Compiler::visit_call(const ast::expr_call_ptr& expr)
  {
    // TODO: Implement compiling call expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling call expression");

    /*
    // Compile and validate the callee
    compile(expr->callee());

    // Compile and validate the arguments
    compile(expr->arguments());
    */
  }

  // Visit a get expression
  void Compiler::visit_get(const ast::expr_get_ptr& expr)
  {
    // TODO: Implement compiling get expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling get expression");

    /*
    // Compile and validate the object
    compile(expr->object());
    */
  }

  // Visit an unary expression
  void Compiler::visit_unary(const ast::expr_unary_ptr& expr)
  {
    // Check if the expression is a logic not operation
    if (expr->op() == frontend::TokenKind::OPERATOR_LOGIC_NOT)
    {
      // TODO: Implement compiling logic not operation
      report<UnimplementedError>(expr->location(), "TODO: Implement compiling logic not operation");
    }

    // Compile the operand of the unary expression
    compile(expr->right());

    // Check and compile the operator
    switch (expr->op())
    {
      // Negate operator
      case frontend::TokenKind::OPERATOR_SUBTRACT:
        visit_unary_negate(expr);
        break;

      // Length operator
      case frontend::TokenKind::OPERATOR_LENGTH:
        visit_unary_length(expr);
        break;

      // String operator
      case frontend::TokenKind::OPERATOR_STRING:
        visit_unary_string(expr);
        break;

      // TODO: Unknown unary operator
      default:
        report<UnimplementedError>(expr->location(), "TODO: Unknown unary operator");
        break;
    }
  }

  // Visit a binary expression
  void Compiler::visit_binary(const ast::expr_binary_ptr& expr)
  {
    // Check if the expression is a logic and operation
    if (expr->op() == frontend::TokenKind::OPERATOR_LOGIC_AND)
    {
      // TODO: Implement compiling logic and operation
      report<UnimplementedError>(expr->location(), "TODO: Implement compiling logic and operation");
    }

    // TODO: Check if the expression is a logic or expression
    if (expr->op() == frontend::TokenKind::OPERATOR_LOGIC_OR)
    {
      // TODO: Implement compiling logic and operation
      report<UnimplementedError>(expr->location(), "TODO: Implement compiling logic or operation");
    }

    // Compile the operands of the binary expression
    compile(expr->left());
    compile(expr->right());

    // Check and compile the operator
    switch (expr->op())
    {
      // Multiply operator
      case frontend::TokenKind::OPERATOR_MULTIPLY:
        visit_binary_multiply(expr);
        break;

      // Divide operator
      case frontend::TokenKind::OPERATOR_DIVIDE:
        visit_binary_divide(expr);
        break;

      // Quotient operator
      case frontend::TokenKind::OPERATOR_QUOTIENT:
        visit_binary_quotient(expr);
        break;

      // Remainder operator
      case frontend::TokenKind::OPERATOR_REMAINDER:
        visit_binary_remainder(expr);
        break;

      // Add operator
      case frontend::TokenKind::OPERATOR_ADD:
        visit_binary_add(expr);
        break;

      // Subtract operator
      case frontend::TokenKind::OPERATOR_SUBTRACT:
        visit_binary_subtract(expr);
        break;

      // Range operator
      case frontend::TokenKind::OPERATOR_RANGE:
        visit_binary_range(expr);
        break;

      // Compare operator
      case frontend::TokenKind::OPERATOR_COMPARE:
        visit_binary_compare(expr);
        break;

      // Less than operator
      case frontend::TokenKind::OPERATOR_LESS:
        visit_binary_compare(expr);
        emit(Instruction::ILTZ, expr->location());
        break;

      // Less than or equal operator
      case frontend::TokenKind::OPERATOR_LESS_EQUAL:
        visit_binary_compare(expr);
        emit(Instruction::ILEZ, expr->location());
        break;

      // Greater than operator
      case frontend::TokenKind::OPERATOR_GREATER:
        visit_binary_compare(expr);
        emit(Instruction::IGTZ, expr->location());
        break;

      // Greater than or equal operator
      case frontend::TokenKind::OPERATOR_GREATER_EQUAL:
        visit_binary_compare(expr);
        emit(Instruction::IGEZ, expr->location());
        break;

      // Match operator
      case frontend::TokenKind::OPERATOR_MATCH:
        visit_binary_match(expr);
        break;

      // Not match operator
      case frontend::TokenKind::OPERATOR_NOT_MATCH:
        visit_binary_match(expr);
        emit(Instruction::NOT, expr->location());
        break;

      // Equal operator
      case frontend::TokenKind::OPERATOR_EQUAL:
        visit_binary_equal(expr);
        break;

      // Not equal operator
      case frontend::TokenKind::OPERATOR_NOT_EQUAL:
        visit_binary_equal(expr);
        emit(Instruction::NOT, expr->location());
        break;

      // Identical operator
      case frontend::TokenKind::OPERATOR_IDENTICAL:
        visit_binary_identical(expr);
        break;

      // Not identical operator
      case frontend::TokenKind::OPERATOR_NOT_IDENTICAL:
        visit_binary_identical(expr);
        emit(Instruction::NOT, expr->location());
        break;

      // TODO: Unknown binary operator
      default:
        report<UnimplementedError>(expr->location(), "TODO: Unknown binary operator");
        break;
    }
  }

  // Visit an echo expression
  void Compiler::visit_echo(const ast::expr_echo_ptr& expr)
  {
    // Compile the nested expression
    compile(expr->expr());

    // Emit the echo instruction
    emit(Instruction::ECHO, expr->location());
  }

  // Visit an if expression
  void Compiler::visit_if(const ast::expr_if_ptr& expr)
  {
    // TODO: Implement compiling if expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling if expression");
  }

  // Visit a for expression
  void Compiler::visit_for(const ast::expr_for_ptr& expr)
  {
    // TODO: Implement compiling for expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling for expression");
  }

  // Visit a while expression
  void Compiler::visit_while(const ast::expr_while_ptr& expr)
  {
    // TODO: Implement compiling while expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling while expression");
  }

  // Visit an until expression
  void Compiler::visit_until(const ast::expr_until_ptr& expr)
  {
    // TODO: Implement compiling until expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling until expression");
  }

  // Visit a block expression
  void Compiler::visit_block(const ast::expr_block_ptr& expr)
  {
    // Iterate over the sub-expressions and compile them in order
    for (ast::expr_ptr sub_expr : *expr)
      compile(sub_expr);
  }

  // Visit a def expression
  void Compiler::visit_def(const ast::expr_def_ptr& expr)
  {
    // TODO: Implement compiling def expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling def expression");

    /*
    // Compile the value
    compile(expr->value());
    */
  }

  // --------------------------------------------------------------------------
  // TYPE EXPRESSION VISITOR IMPLEMENTATION
  // --------------------------------------------------------------------------

  // Visit a name type expression
  void Compiler::visit_type_name(const ast::type_expr_name_ptr& expr)
  {
    // TODO: Implement compiling name type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling name type expression");
  }

  // Visit a grouped type expression
  void Compiler::visit_type_grouped(const ast::type_expr_grouped_ptr& expr)
  {
    // TODO: Implement compiling grouped type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling grouped type expression");
  }

  // Visit a generic type expression
  void Compiler::visit_type_generic(const ast::type_expr_generic_ptr& expr)
  {
    // TODO: Implement compiling generic type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling generic type expression");
  }

  // Visit a maybe type expression
  void Compiler::visit_type_maybe(const ast::type_expr_maybe_ptr& expr)
  {
    // TODO: Implement compiling maybe type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling maybe type expression");
  }

  // Visit an intersection type expression
  void Compiler::visit_type_intersection(const ast::type_expr_intersection_ptr& expr)
  {
    // TODO: Implement compiling intersection type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling intersection type expression");
  }

  // Visit an union type expression
  void Compiler::visit_type_union(const ast::type_expr_union_ptr& expr)
  {
    // TODO: Implement compiling union type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling union type expression");
  }

  // --------------------------------------------------------------------------
  // HELPER FUNCTIONS FOR OPERATORS
  // --------------------------------------------------------------------------

  // Compile the negate operator
  void Compiler::visit_unary_negate(const ast::expr_unary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_int))
      emit(Instruction::INEG, expr->location());
    else if (expr->check_operand_type(internals::Type::type_real))
      emit(Instruction::RNEG, expr->location());
    else
      report<CompilerError>(expr->location(), fmt::format("Wrong operand type {} for -"));
  }

  // Compile the length operator
  void Compiler::visit_unary_length(const ast::expr_unary_ptr& expr)
  {
    // TODO: Implement compiling length operation
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling length operation");
  }

  // Compile the string operator
  void Compiler::visit_unary_string(const ast::expr_unary_ptr& expr)
  {
    // TODO: Implement compiling string operation
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling string operation");
  }

  // Compile the multiply operator
  void Compiler::visit_binary_multiply(const ast::expr_binary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
      emit(Instruction::IMUL, expr->location());
    else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
      emit(Instruction::RMUL, expr->location());
    else
      report<CompilerError>(expr->location(), "Wrong operand types for *");
  }

  // Compile the divide operator
  void Compiler::visit_binary_divide(const ast::expr_binary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
      emit(Instruction::IDIV, expr->location());
    else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
      emit(Instruction::RDIV, expr->location());
    else
      report<CompilerError>(expr->location(), "Wrong operand types for /");
  }

  // Compile the quotient operator
  void Compiler::visit_binary_quotient(const ast::expr_binary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
      emit(Instruction::IQUO, expr->location());
    else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
      emit(Instruction::RQUO, expr->location());
    else
      report<CompilerError>(expr->location(), "Wrong operand types for //");
  }

  // Compile the remainder operator
  void Compiler::visit_binary_remainder(const ast::expr_binary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
      emit(Instruction::IREM, expr->location());
    else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
      emit(Instruction::RREM, expr->location());
    else
      report<CompilerError>(expr->location(), "Wrong operand types for %");
  }

  // Compile the add operator
  void Compiler::visit_binary_add(const ast::expr_binary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
      emit(Instruction::IADD, expr->location());
    else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
      emit(Instruction::RADD, expr->location());
    else
      report<CompilerError>(expr->location(), "Wrong operand types for +");
  }

  // Compile the subtract operator
  void Compiler::visit_binary_subtract(const ast::expr_binary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
      emit(Instruction::ISUB, expr->location());
    else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
      emit(Instruction::RSUB, expr->location());
    else
      report<CompilerError>(expr->location(), "Wrong operand types for -");
  }

  // Compile the range operator
  void Compiler::visit_binary_range(const ast::expr_binary_ptr& expr)
  {
    // TODO: Implement compiling range operation
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling range operation");
  }

  // Compile the compare operator
  void Compiler::visit_binary_compare(const ast::expr_binary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
      emit(Instruction::ICMP, expr->location());
    else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
      emit(Instruction::RCMP, expr->location());
    else if (expr->check_operand_type(internals::Type::type_rune, internals::Type::type_rune))
      emit(Instruction::UCMP, expr->location());
    else
      report<CompilerError>(expr->location(), "Wrong operand types for <=>");
  }

  // Compile the match operator
  void Compiler::visit_binary_match(const ast::expr_binary_ptr& expr)
  {
    // TODO: Implement compiling match operation
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling match operation");
  }

  // Compile the equal operator
  void Compiler::visit_binary_equal(const ast::expr_binary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_bool, internals::Type::type_bool))
    {
      emit(Instruction::BEQ, expr->location());
    }
    else if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
    {
      emit(Instruction::ICMP, expr->location());
      emit(Instruction::IEQZ, expr->location());
    }
    else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
    {
      emit(Instruction::RCMP, expr->location());
      emit(Instruction::IEQZ, expr->location());
    }
    else if (expr->check_operand_type(internals::Type::type_rune, internals::Type::type_rune))
    {
      emit(Instruction::UCMP, expr->location());
      emit(Instruction::IEQZ, expr->location());
    }
    else
      report<CompilerError>(expr->location(), "Wrong operand types for ==");
  }

  // Compile the identical operator
  void Compiler::visit_binary_identical(const ast::expr_binary_ptr& expr)
  {
    // TODO: Implement compiling of identical operation
    report<UnimplementedError>(expr->location(), "TODO: Implement compiling identical operation");
  }
}
