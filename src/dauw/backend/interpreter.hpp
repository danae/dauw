#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/ast/expr.hpp>
#include <dauw/ast/type_expr.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/internals/string_object.hpp>
#include <dauw/internals/type.hpp>
#include <dauw/internals/value.hpp>
#include <dauw/utils/math.hpp>

#include <forward_list>
#include <type_traits>


namespace dauw
{
  // Class that defines the interpreter
  class Interpreter : public ExprVisitor, public TypeExprVisitor, public ReporterAware, public std::enable_shared_from_this<Interpreter>
  {
    private:
      // Return the result of comparing two values
      dauw_int_t op_compare(Location& location, internals::Value left, internals::Value right);

      // Return the result of checking if two values match
      dauw_bool_t op_match(Location& location, internals::Value left, internals::Value right);

      // Return the result of checking if two values are equal
      dauw_bool_t op_equals(Location& location, internals::Value left, internals::Value right);


    public:
      // Constructor
      Interpreter(Reporter* reporter);

      // Evaluate an expression
      internals::Value evaluate(const expr_ptr& expr);

      // Evaluate a type expression
      internals::Type evaluate(const type_expr_ptr& expr);

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
      virtual void visit_echo(const expr_echo_ptr& expr) override;
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
