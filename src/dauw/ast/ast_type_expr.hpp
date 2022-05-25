#pragma once

#include <dauw/ast/ast_node.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/frontend/token.hpp>


namespace dauw::ast
{
  // Forward declarations
  class TypeExprVisitor;
  class TypeExpr;
  class TypeExprName;
  class TypeExprGrouped;
  class TypeExprGeneric;
  class TypeExprMaybe;
  class TypeExprIntersection;
  class TypeExprUnion;

  // Type definitions for pointers
  using type_expr_visitor_ptr = std::shared_ptr<TypeExprVisitor>;
  using type_expr_ptr = std::shared_ptr<TypeExpr>;
  using type_expr_name_ptr = std::shared_ptr<TypeExprName>;
  using type_expr_grouped_ptr = std::shared_ptr<TypeExprGrouped>;
  using type_expr_generic_ptr = std::shared_ptr<TypeExprGeneric>;
  using type_expr_maybe_ptr = std::shared_ptr<TypeExprMaybe>;
  using type_expr_intersection_ptr = std::shared_ptr<TypeExprIntersection>;
  using type_expr_union_ptr = std::shared_ptr<TypeExprUnion>;


  // Base class that defines an expression visitor
  class TypeExprVisitor
  {
    public:
      // Destructor
      virtual ~TypeExprVisitor() = default;

      // Visit type expressions
      virtual void visit_type_name(const type_expr_name_ptr& expr) = 0;
      virtual void visit_type_grouped(const type_expr_grouped_ptr& expr) = 0;
      virtual void visit_type_generic(const type_expr_generic_ptr& expr) = 0;
      virtual void visit_type_maybe(const type_expr_maybe_ptr& expr) = 0;
      virtual void visit_type_intersection(const type_expr_intersection_ptr& expr) = 0;
      virtual void visit_type_union(const type_expr_union_ptr& expr) = 0;
  };


  // Base class thet defines a type expression
  class TypeExpr : public Node
  {
    public:
      // Destructor
      virtual ~TypeExpr() = default;

      // Accept a visitor on the type expression
      virtual void accept(const type_expr_visitor_ptr& visitor) = 0;
  };


  // Class that defines a name type expression
  class TypeExprName : public TypeExpr, public std::enable_shared_from_this<TypeExprName>
  {
    private:
      // The name token of the name type expression
      frontend::Token name_;


    public:
      // Constructor
      TypeExprName(frontend::Token name);

      // Return the name of the name type expression
      string_t name();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const type_expr_visitor_ptr& visitor) override;
  };


  // Class that defines a grouped type expression
  class TypeExprGrouped : public TypeExpr, public std::enable_shared_from_this<TypeExprGrouped>
  {
    private:
      // The nested type expression of the grouped type expression
      type_expr_ptr expr_;


    public:
      // Constructor
      TypeExprGrouped(type_expr_ptr expr);

      // Return the nested type expression of the grouped type expression
      type_expr_ptr expr();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const type_expr_visitor_ptr& visitor) override;
  };


  // Class that defines a generic type expression
  class TypeExprGeneric : public TypeExpr, public std::enable_shared_from_this<TypeExprGeneric>
  {
    private:
      // The base type of the generic type expression
      type_expr_ptr base_;

      // The token of the generic type expression
      frontend::Token token_;

      // The generic arguments of the generic type expression
      std::vector<type_expr_ptr> arguments_;


    public:
      // Constructor
      TypeExprGeneric(type_expr_ptr base, frontend::Token token, std::vector<type_expr_ptr> arguments);

      // Return the base of the generic type expression
      type_expr_ptr base();

      // Return the generic arguments of the generic type expression
      std::vector<type_expr_ptr> arguments();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const type_expr_visitor_ptr& visitor) override;
  };


  // Class that defines a maybe type expression
  class TypeExprMaybe : public TypeExpr, public std::enable_shared_from_this<TypeExprMaybe>
  {
    private:
      // The base type of the maybe type expression
      type_expr_ptr base_;

      // The operator of the maybe type expression
      frontend::Token op_;


    public:
      // Constructor
      TypeExprMaybe(type_expr_ptr base, frontend::Token op);

      // Return the base of the maybe type expression
      type_expr_ptr base();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const type_expr_visitor_ptr& visitor) override;
  };


  // Class that defines an intersection type expression
  class TypeExprIntersection : public TypeExpr, public std::enable_shared_from_this<TypeExprIntersection>
  {
    private:
      // The operator of the intersection type expression
      frontend::Token op_;

      // The operands of the intersection type expression
      type_expr_ptr left_;
      type_expr_ptr right_;


    public:
      // Constructor
      TypeExprIntersection(type_expr_ptr left, frontend::Token op, type_expr_ptr right);

      // Return the operands of the intersection type expression
      type_expr_ptr left();
      type_expr_ptr right();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const type_expr_visitor_ptr& visitor) override;
  };


  // Class that defines an union type expression
  class TypeExprUnion : public TypeExpr, public std::enable_shared_from_this<TypeExprUnion>
  {
    private:
      // The operator of the intersection type expression
      frontend::Token op_;

      // The operands of the union type expression
      type_expr_ptr left_;
      type_expr_ptr right_;


    public:
      // Constructor
      TypeExprUnion(type_expr_ptr left, frontend::Token op, type_expr_ptr right);

      // Return the operands of the union type expression
      type_expr_ptr left();
      type_expr_ptr right();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const type_expr_visitor_ptr& visitor) override;
  };
}
