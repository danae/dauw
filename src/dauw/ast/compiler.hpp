#pragma once

#include <dauw/common.hpp>
#include <dauw/ast/ast.hpp>
#include <dauw/backend/code.hpp>
#include <dauw/internals/value.hpp>


namespace dauw
{
  // Class that defines the compiler
  class Compiler : public ExprVisitor, public TypeExprVisitor, public std::enable_shared_from_this<Compiler>
  {
    private:
      // The code that is being created by the compiler
      Code* code_;


    public:
      // Constructor
      Compiler(Code* code);

      // Compile an expression
      void compile(const expr_ptr& expr);

      // Expression visitor implementation
      virtual void visit_literal(const expr_literal_ptr& expr) override;
      virtual void visit_sequence(const expr_sequence_ptr& expr) override;
      virtual void visit_record(const expr_record_ptr& expr) override;
      virtual void visit_name(const expr_name_ptr& expr) override;
      virtual void visit_function(const expr_function_ptr& expr) override;
      virtual void visit_function_parameter(const expr_function_parameter_ptr& expr) override;
      virtual void visit_grouped(const expr_grouped_ptr& expr) override;
      virtual void visit_call(const expr_call_ptr& expr) override;
      virtual void visit_get(const expr_get_ptr& expr) override;
      virtual void visit_unary(const expr_unary_ptr& expr) override;
      virtual void visit_binary(const expr_binary_ptr& expr) override;
      virtual void visit_if(const expr_if_ptr& expr) override;
      virtual void visit_for(const expr_for_ptr& expr) override;
      virtual void visit_while(const expr_while_ptr& expr) override;
      virtual void visit_until(const expr_until_ptr& expr) override;
      virtual void visit_block(const expr_block_ptr& expr) override;
      virtual void visit_def(const expr_def_ptr& expr) override;

      // Type expression visitor implementation
      virtual void visit_type_name(const type_expr_name_ptr& expr) override;
      virtual void visit_type_generic(const type_expr_generic_ptr& expr) override;
      virtual void visit_type_grouped(const type_expr_grouped_ptr& expr) override;
      virtual void visit_type_maybe(const type_expr_maybe_ptr& expr) override;
      virtual void visit_type_intersection(const type_expr_intersection_ptr& expr) override;
      virtual void visit_type_union(const type_expr_union_ptr& expr) override;
  };
}
