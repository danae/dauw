#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/ast/ast_expr.hpp>
#include <dauw/ast/ast_type_expr.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/internals/type.hpp>
#include <dauw/internals/value.hpp>


namespace dauw::backend
{
  // Class that defines the type resolver
  class TypeResolver : public ast::ExprVisitor, public ast::TypeExprVisitor, public ReporterAware, public std::enable_shared_from_this<TypeResolver>
  {
    public:
      // Constructor
      TypeResolver(Reporter* reporter);

      // Resolve the type of an expression
      void resolve(const ast::expr_ptr& expr, bool throw_if_failed = true);

      // Resolve if a type is a subtype of another type
      void resolve_subtype_of(const ast::expr_ptr& expr, internals::Type& type);

      // Visitor implementation
      virtual void visit_literal(const ast::expr_literal_ptr& expr) override;
      virtual void visit_sequence(const ast::expr_sequence_ptr& expr) override;
      virtual void visit_record(const ast::expr_record_ptr& expr) override;
      virtual void visit_name(const ast::expr_name_ptr& expr) override;
      virtual void visit_function(const ast::expr_function_ptr& expr) override;
      virtual void visit_function_parameter(const ast::expr_function_parameter_ptr& expr) override;
      virtual void visit_grouped(const ast::expr_grouped_ptr& expr) override;
      virtual void visit_call(const ast::expr_call_ptr& expr) override;
      virtual void visit_get(const ast::expr_get_ptr& expr) override;
      virtual void visit_unary(const ast::expr_unary_ptr& expr) override;
      virtual void visit_binary(const ast::expr_binary_ptr& expr) override;
      virtual void visit_echo(const ast::expr_echo_ptr& expr) override;
      virtual void visit_if(const ast::expr_if_ptr& expr) override;
      virtual void visit_for(const ast::expr_for_ptr& expr) override;
      virtual void visit_while(const ast::expr_while_ptr& expr) override;
      virtual void visit_until(const ast::expr_until_ptr& expr) override;
      virtual void visit_block(const ast::expr_block_ptr& expr) override;
      virtual void visit_def(const ast::expr_def_ptr& expr) override;

      // Type expression visitor implementation
      virtual void visit_type_name(const ast::type_expr_name_ptr& expr) override;
      virtual void visit_type_grouped(const ast::type_expr_grouped_ptr& expr) override;
      virtual void visit_type_generic(const ast::type_expr_generic_ptr& expr) override;
      virtual void visit_type_maybe(const ast::type_expr_maybe_ptr& expr) override;
      virtual void visit_type_intersection(const ast::type_expr_intersection_ptr& expr) override;
      virtual void visit_type_union(const ast::type_expr_union_ptr& expr) override;
  };
}
