#pragma once

#include <dauw/common.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/frontend/token.hpp>
#include <dauw/internals/type.hpp>
#include <dauw/internals/value.hpp>


namespace dauw
{
  // Forward declarations
  class Node;

  class ExprVisitor;
  class Expr;
  class ExprLiteral;
  class ExprSequence;
  class ExprRecord;
  class ExprName;
  class ExprFunction;
  class ExprFunctionParameter;
  class ExprGrouped;
  class ExprCall;
  class ExprGet;
  class ExprUnary;
  class ExprBinary;
  class ExprEcho;
  class ExprIf;
  class ExprFor;
  class ExprWhile;
  class ExprUntil;
  class ExprBlock;
  class ExprDef;

  class TypeExprVisitor;
  class TypeExpr;
  class TypeExprName;
  class TypeExprGrouped;
  class TypeExprGeneric;
  class TypeExprMaybe;
  class TypeExprIntersection;
  class TypeExprUnion;

  // Type definitions for pointers
  using node_ptr = std::shared_ptr<Node>;

  using expr_visitor_ptr = std::shared_ptr<ExprVisitor>;
  using expr_ptr = std::shared_ptr<Expr>;
  using expr_literal_ptr = std::shared_ptr<ExprLiteral>;
  using expr_sequence_ptr = std::shared_ptr<ExprSequence>;
  using expr_record_ptr = std::shared_ptr<ExprRecord>;
  using expr_name_ptr = std::shared_ptr<ExprName>;
  using expr_function_ptr = std::shared_ptr<ExprFunction>;
  using expr_function_parameter_ptr = std::shared_ptr<ExprFunctionParameter>;
  using expr_grouped_ptr = std::shared_ptr<ExprGrouped>;
  using expr_call_ptr = std::shared_ptr<ExprCall>;
  using expr_get_ptr = std::shared_ptr<ExprGet>;
  using expr_unary_ptr = std::shared_ptr<ExprUnary>;
  using expr_binary_ptr = std::shared_ptr<ExprBinary>;
  using expr_echo_ptr = std::shared_ptr<ExprEcho>;
  using expr_if_ptr = std::shared_ptr<ExprIf>;
  using expr_for_ptr = std::shared_ptr<ExprFor>;
  using expr_while_ptr = std::shared_ptr<ExprWhile>;
  using expr_until_ptr = std::shared_ptr<ExprUntil>;
  using expr_block_ptr = std::shared_ptr<ExprBlock>;
  using expr_def_ptr = std::shared_ptr<ExprDef>;

  using type_expr_visitor_ptr = std::shared_ptr<TypeExprVisitor>;
  using type_expr_ptr = std::shared_ptr<TypeExpr>;
  using type_expr_name_ptr = std::shared_ptr<TypeExprName>;
  using type_expr_grouped_ptr = std::shared_ptr<TypeExprGrouped>;
  using type_expr_generic_ptr = std::shared_ptr<TypeExprGeneric>;
  using type_expr_maybe_ptr = std::shared_ptr<TypeExprMaybe>;
  using type_expr_intersection_ptr = std::shared_ptr<TypeExprIntersection>;
  using type_expr_union_ptr = std::shared_ptr<TypeExprUnion>;


  // Base class that defines a node in the abstract syntax tree
  class Node
  {
    private:
      // The resolved type of the node
      type_ptr resolved_type_ = nullptr;


    public:
      // Destructor
      virtual ~Node() = default;

      // Return the resolved type of the node
      type_ptr& resolved_type();

      // Return if the node has a resolved type
      bool has_resolved_type();

      // Set the resolved type of the node
      void set_resolved_type(type_ptr& type);
      void set_resolved_type_from(node_ptr node);

      // Return the location of the node
      virtual Location& location() = 0;
  };


  // Base class that defines an expression visitor
  class ExprVisitor
  {
    public:
      // Destructor
      virtual ~ExprVisitor() = default;

      // Visit expressions
      virtual void visit_literal(const expr_literal_ptr& expr) = 0;
      virtual void visit_sequence(const expr_sequence_ptr& expr) = 0;
      virtual void visit_record(const expr_record_ptr& expr) = 0;
      virtual void visit_name(const expr_name_ptr& expr) = 0;
      virtual void visit_function(const expr_function_ptr& expr) = 0;
      virtual void visit_function_parameter(const expr_function_parameter_ptr& expr) = 0;
      virtual void visit_grouped(const expr_grouped_ptr& expr) = 0;
      virtual void visit_call(const expr_call_ptr& expr) = 0;
      virtual void visit_get(const expr_get_ptr& expr) = 0;
      virtual void visit_unary(const expr_unary_ptr& expr) = 0;
      virtual void visit_binary(const expr_binary_ptr& expr) = 0;
      virtual void visit_echo(const expr_echo_ptr& expr) = 0;
      virtual void visit_if(const expr_if_ptr& expr) = 0;
      virtual void visit_for(const expr_for_ptr& expr) = 0;
      virtual void visit_while(const expr_while_ptr& expr) = 0;
      virtual void visit_until(const expr_until_ptr& expr) = 0;
      virtual void visit_block(const expr_block_ptr& expr) = 0;
      virtual void visit_def(const expr_def_ptr& expr) = 0;
  };


  // Base class thet defines an expression
  class Expr : public Node
  {
    public:
      // Destructor
      virtual ~Expr() = default;

      // Accept a visitor on the expression
      virtual void accept(const expr_visitor_ptr& visitor) = 0;
  };


  // Class that defines a literal expression
  class ExprLiteral : public Expr, public std::enable_shared_from_this<ExprLiteral>
  {
    private:
      // The value of the literal expression
      Value value_;

      // The location of the literal expression
      Location location_;


    public:
      // Constructor
      ExprLiteral(Value value, Location location);

      // Return the value of the literal expression
      Value value();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a sequence expression
  class ExprSequence : public Expr, public std::enable_shared_from_this<ExprSequence>
  {
    public:
      // Type definition for the underlying sequence container
      using sequence_type = std::vector<expr_ptr>;


    private:
      // The token of the sequence expression
      Token token_;

      // The items of the sequence expression
      sequence_type items_;


    public:
      // Constructor
      ExprSequence(Token token, sequence_type items);

      // Return the items of the sequence expression
      sequence_type items();

      // Iterate over the items of the sequence expression
      sequence_type::const_iterator begin();
      sequence_type::const_iterator end();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a record expression
  class ExprRecord : public Expr, public std::enable_shared_from_this<ExprRecord>
  {
    public:
      // Type definition for the underlying record container
      using record_type = std::unordered_map<string_t, expr_ptr>;


    private:
      // The token of the record expression
      Token token_;

      // The items of the record expression
      record_type items_;


    public:
      // Constructor
      ExprRecord(Token token, record_type items);

      // Return the items of the record expression
      record_type items();

      // Iterate over the items of the record expression
      record_type::const_iterator begin();
      record_type::const_iterator end();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a function expression
  class ExprFunction : public Expr, public std::enable_shared_from_this<ExprFunction>
  {
    public:
      // Type declaration for the parameter expression
      using parameters_type = std::vector<expr_function_parameter_ptr>;


    private:
      // The token of the function expression
      Token token_;

      // The parameters of the function expression
      parameters_type parameters_;

      // The return type of the function expression
      std::optional<type_expr_ptr> return_type_;

      // The body of the function expression
      expr_ptr body_;


    public:
      // Constructor
      ExprFunction(Token token, parameters_type parameters, std::optional<type_expr_ptr> return_type, expr_ptr body);

      // Return the parameters of the function expression
      parameters_type parameters();

      // Return the return type of the function expression
      type_expr_ptr return_type();

      // Return if the function definition has a return type
      bool has_return_type();

      // Return the body of the function expression
      expr_ptr body();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;


      // Return the parameter name for a parameter

      // Return the type for a parameter
  };


  // Class that defines a function parameter expression
  class ExprFunctionParameter : public Expr, public std::enable_shared_from_this<ExprFunctionParameter>
  {
    private:
      // The name token of the function parameter expression
      Token name_;

      // The type of the function parameter expression
      type_expr_ptr type_;


    public:
      // Constructor
      ExprFunctionParameter(Token name, type_expr_ptr type);

      // Return the name of the function parameter expression
      string_t name();

      // Return the type of the function parameter expression
      type_expr_ptr type();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a name expression
  class ExprName : public Expr, public std::enable_shared_from_this<ExprName>
  {
    private:
      // The name token of the name expression
      Token name_;


    public:
      // Constructor
      ExprName(Token name);

      // Return the name of the name expression
      string_t name();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a grouped expression
  class ExprGrouped : public Expr, public std::enable_shared_from_this<ExprGrouped>
  {
    private:
      // The nested expression of the grouped expression
      expr_ptr expr_;


    public:
      // Constructor
      ExprGrouped(expr_ptr expr);

      // Return the nested expression of the grouped expression
      expr_ptr expr();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a call expression
  class ExprCall : public Expr, public std::enable_shared_from_this<ExprCall>
  {
    private:
      // The callee of the call expression
      expr_ptr callee_;

      // The token of the call expression
      Token token_;

      // The arguments of the call expression
      expr_sequence_ptr arguments_;


    public:
      // Constructor
      ExprCall(expr_ptr callee, Token token, expr_sequence_ptr arguments);

      // Return the callee of the call expression
      expr_ptr callee();

      // Return the arguments of the call expression
      expr_sequence_ptr arguments();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a get expression
  class ExprGet : public Expr, public std::enable_shared_from_this<ExprGet>
  {
    private:
      // The object of the get expression
      expr_ptr object_;

      // The name token of the get expression
      Token name_;


    public:
      // Constructor
      ExprGet(expr_ptr object, Token name);

      // Return the object of the get expression
      expr_ptr object();

      // Return the name of the get expression
      string_t name();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines an unary expression
  class ExprUnary : public Expr, public std::enable_shared_from_this<ExprUnary>
  {
    private:
      // The operator token of the unary expression
      Token op_;

      // The operand of the unary expression
      expr_ptr right_;


    public:
      // Constructor
      ExprUnary(Token op, expr_ptr right);

      // Return the operator of the unary expression
      string_t op();

      // Return the operand of the unary expression
      expr_ptr right();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a binary expression
  class ExprBinary : public Expr, public std::enable_shared_from_this<ExprBinary>
  {
    private:
      // The operator token of the binary expression
      Token op_;

      // The operands of the binary expression
      expr_ptr left_;
      expr_ptr right_;


    public:
      // Constructor
      ExprBinary(expr_ptr left, Token op, expr_ptr right);

      // Return the operator of the unary expression
      string_t op();

      // Return the operands of the unary expression
      expr_ptr left();
      expr_ptr right();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines an echo expression
  class ExprEcho : public Expr, public std::enable_shared_from_this<ExprEcho>
  {
    private:
      // The keyword token of the echo expression
      Token keyword_;

      // The nested expression of the echo expression
      expr_ptr expr_;


    public:
      // Constructor
      ExprEcho(Token keyword, expr_ptr expr);

      // Return the nested expression of the echo expression
      expr_ptr expr();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines an if expression
  class ExprIf : public Expr, public std::enable_shared_from_this<ExprIf>
  {
    private:
      // The keyword token of the if expression
      Token keyword_;

      // The condition of the if expression
      expr_ptr condition_;

      // The true branch of the if expression
      expr_ptr true_branch_;

      // The false branch of the if expression
      std::optional<expr_ptr> false_branch_;


    public:
      // Constructor
      ExprIf(Token keyword, expr_ptr condition, expr_ptr true_branch, std::optional<expr_ptr> false_branch);

      // Return the condition of the if expression
      expr_ptr condition();

      // Return the branches of the if expression
      expr_ptr true_branch();

      // Return the false branch of the if expression
      expr_ptr false_branch();

      // Return if the if expression has a false branch
      bool has_false_branch();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a for expression
  class ExprFor : public Expr, public std::enable_shared_from_this<ExprFor>
  {
    private:
      // The keyword token of the for expression
      Token keyword_;

      // The name token of the for expression
      Token name_;

      // The iterable of the for expression
      expr_ptr iterable_;

      // The body of the for expression
      expr_ptr body_;


    public:
      // Constructor
      ExprFor(Token keyword, Token name, expr_ptr iterable, expr_ptr body);

      // Return the name of the for expression
      string_t name();

      // Return the iterable of the for expression
      expr_ptr iterable();

      // Return the body of the for expression
      expr_ptr body();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a while expression
  class ExprWhile : public Expr, public std::enable_shared_from_this<ExprWhile>
  {
    private:
      // The keyword token of the while expression
      Token keyword_;

      // The condition of the while expression
      expr_ptr condition_;

      // The body of the while expression
      expr_ptr body_;


    public:
      // Constructor
      ExprWhile(Token keyword, expr_ptr condition, expr_ptr body);

      // Return the condition of the while expression
      expr_ptr condition();

      // Return the body of the while expression
      expr_ptr body();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines an until expression
  class ExprUntil : public Expr, public std::enable_shared_from_this<ExprUntil>
  {
    private:
      // The keyword token of the until expression
      Token keyword_;

      // The condition of the until expression
      expr_ptr condition_;

      // The body of the until expression
      expr_ptr body_;


    public:
      // Constructor
      ExprUntil(Token keyword, expr_ptr condition, expr_ptr body);

      // Return the condition of the until expression
      expr_ptr condition();

      // Return the body of the until expression
      expr_ptr body();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a block expression
  class ExprBlock : public Expr, public std::enable_shared_from_this<ExprBlock>
  {
    public:
      // Type definition for the expressions of the block
      using block_type = std::vector<expr_ptr>;

    private:
      // The expressions of the block expression
      block_type exprs_;


    public:
      // Constructor
      ExprBlock(block_type exprs);

      // Return the expressions of the block expression
      block_type exprs();

      // Iterate over the expressions of the block expression
      block_type::const_iterator begin();
      block_type::const_iterator end();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a def expression
  class ExprDef : public Expr, public std::enable_shared_from_this<ExprDef>
  {
    private:
      // The name token of the def expression
      Token name_;

      // The type of the def expression
      std::optional<type_expr_ptr> type_;

      // The value of the def expression
      expr_ptr value_;


    public:
      // Constructor
      ExprDef(Token name, std::optional<type_expr_ptr> type, expr_ptr value);

      // Return the name of the def expression
      string_t name();

      // Return the type of the def expression
      type_expr_ptr type();

      // Return if the def expression has a type
      bool has_type();

      // Return the value of the def expression
      expr_ptr value();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


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
      Token name_;


    public:
      // Constructor
      TypeExprName(Token name);

      // Return the name of the name type expression
      string_t name();

      // Expression implementation
      virtual Location& location() override;
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
      virtual Location& location() override;
      virtual void accept(const type_expr_visitor_ptr& visitor) override;
  };


  // Class that defines a generic type expression
  class TypeExprGeneric : public TypeExpr, public std::enable_shared_from_this<TypeExprGeneric>
  {
    public:
      // Type definition for the underlying generic container
      using generic_type = std::vector<type_expr_ptr>;


    private:
      // The base type of the generic type expression
      type_expr_ptr base_;

      // The token of the generic type expression
      Token token_;

      // The generic arguments of the generic type expression
      generic_type arguments_;


    public:
      // Constructor
      TypeExprGeneric(type_expr_ptr base, Token token, generic_type arguments);

      // Return the base of the generic type expression
      type_expr_ptr base();

      // Return the generic arguments of the generic type expression
      generic_type arguments();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const type_expr_visitor_ptr& visitor) override;
  };


  // Class that defines a maybe type expression
  class TypeExprMaybe : public TypeExpr, public std::enable_shared_from_this<TypeExprMaybe>
  {
    private:
      // The base type of the maybe type expression
      type_expr_ptr base_;

      // The operator of the maybe type expression
      Token op_;


    public:
      // Constructor
      TypeExprMaybe(type_expr_ptr base, Token op);

      // Return the base of the maybe type expression
      type_expr_ptr base();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const type_expr_visitor_ptr& visitor) override;
  };


  // Class that defines an intersection type expression
  class TypeExprIntersection : public TypeExpr, public std::enable_shared_from_this<TypeExprIntersection>
  {
    private:
      // The operator of the intersection type expression
      Token op_;

      // The operands of the intersection type expression
      type_expr_ptr left_;
      type_expr_ptr right_;


    public:
      // Constructor
      TypeExprIntersection(type_expr_ptr left, Token op, type_expr_ptr right);

      // Return the operands of the intersection type expression
      type_expr_ptr left();
      type_expr_ptr right();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const type_expr_visitor_ptr& visitor) override;
  };


  // Class that defines an union type expression
  class TypeExprUnion : public TypeExpr, public std::enable_shared_from_this<TypeExprUnion>
  {
    private:
      // The operator of the intersection type expression
      Token op_;

      // The operands of the union type expression
      type_expr_ptr left_;
      type_expr_ptr right_;


    public:
      // Constructor
      TypeExprUnion(type_expr_ptr left, Token op, type_expr_ptr right);

      // Return the operands of the union type expression
      type_expr_ptr left();
      type_expr_ptr right();

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const type_expr_visitor_ptr& visitor) override;
  };
}
