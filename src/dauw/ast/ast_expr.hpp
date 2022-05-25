#pragma once

#include <dauw/common.hpp>
#include <dauw/ast/ast_node.hpp>
#include <dauw/ast/ast_type_expr.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/frontend/token.hpp>
#include <dauw/internals/type.hpp>
#include <dauw/internals/value.hpp>


namespace dauw::ast
{
  // Forward declarations
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

  // Type definitions for pointers
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
      internals::Value value_;

      // The location of the literal expression
      frontend::Location location_;


    public:
      // Constructor
      ExprLiteral(internals::Value value, frontend::Location location);

      // Return the value of the literal expression
      internals::Value& value();

      // Expression implementation
      virtual frontend::Location& location() override;
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
      frontend::Token token_;

      // The items of the sequence expression
      sequence_type items_;


    public:
      // Constructor
      ExprSequence(frontend::Token token, sequence_type items);

      // Return the items of the sequence expression
      sequence_type items();

      // Iterate over the items of the sequence expression
      sequence_type::const_iterator begin();
      sequence_type::const_iterator end();

      // Expression implementation
      virtual frontend::Location& location() override;
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
      frontend::Token token_;

      // The items of the record expression
      record_type items_;


    public:
      // Constructor
      ExprRecord(frontend::Token token, record_type items);

      // Return the items of the record expression
      record_type items();

      // Iterate over the items of the record expression
      record_type::const_iterator begin();
      record_type::const_iterator end();

      // Expression implementation
      virtual frontend::Location& location() override;
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
      frontend::Token token_;

      // The parameters of the function expression
      parameters_type parameters_;

      // The return type of the function expression
      std::optional<type_expr_ptr> return_type_;

      // The body of the function expression
      expr_ptr body_;


    public:
      // Constructor
      ExprFunction(frontend::Token token, parameters_type parameters, std::optional<type_expr_ptr> return_type, expr_ptr body);

      // Return the parameters of the function expression
      parameters_type parameters();

      // Return the return type of the function expression
      type_expr_ptr return_type();

      // Return if the function definition has a return type
      bool has_return_type();

      // Return the body of the function expression
      expr_ptr body();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;


      // Return the parameter name for a parameter

      // Return the type for a parameter
  };


  // Class that defines a function parameter expression
  class ExprFunctionParameter : public Expr, public std::enable_shared_from_this<ExprFunctionParameter>
  {
    private:
      // The name token of the function parameter expression
      frontend::Token name_;

      // The type of the function parameter expression
      type_expr_ptr type_;


    public:
      // Constructor
      ExprFunctionParameter(frontend::Token name, type_expr_ptr type);

      // Return the name of the function parameter expression
      string_t name();

      // Return the type of the function parameter expression
      type_expr_ptr type();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a name expression
  class ExprName : public Expr, public std::enable_shared_from_this<ExprName>
  {
    private:
      // The name token of the name expression
      frontend::Token name_;


    public:
      // Constructor
      ExprName(frontend::Token name);

      // Return the name of the name expression
      string_t name();

      // Expression implementation
      virtual frontend::Location& location() override;
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
      virtual frontend::Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a call expression
  class ExprCall : public Expr, public std::enable_shared_from_this<ExprCall>
  {
    private:
      // The callee of the call expression
      expr_ptr callee_;

      // The token of the call expression
      frontend::Token token_;

      // The arguments of the call expression
      expr_sequence_ptr arguments_;


    public:
      // Constructor
      ExprCall(expr_ptr callee, frontend::Token token, expr_sequence_ptr arguments);

      // Return the callee of the call expression
      expr_ptr callee();

      // Return the arguments of the call expression
      expr_sequence_ptr arguments();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a get expression
  class ExprGet : public Expr, public std::enable_shared_from_this<ExprGet>
  {
    private:
      // The object of the get expression
      expr_ptr object_;

      // The name token of the get expression
      frontend::Token name_;


    public:
      // Constructor
      ExprGet(expr_ptr object, frontend::Token name);

      // Return the object of the get expression
      expr_ptr object();

      // Return the name of the get expression
      string_t name();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines an unary expression
  class ExprUnary : public Expr, public std::enable_shared_from_this<ExprUnary>
  {
    private:
      // The operator token of the unary expression
      frontend::Token op_;

      // The operand of the unary expression
      expr_ptr right_;


    public:
      // Constructor
      ExprUnary(frontend::Token op, expr_ptr right);

      // Return the operator of the unary expression
      frontend::TokenKind op();

      // Return the operand of the unary expression
      expr_ptr right();

      // Return if the resolved types of the operand matches the specified type
      bool check_operand_type(internals::Type right_type);

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a binary expression
  class ExprBinary : public Expr, public std::enable_shared_from_this<ExprBinary>
  {
    private:
      // The operator token of the binary expression
      frontend::Token op_;

      // The operands of the binary expression
      expr_ptr left_;
      expr_ptr right_;


    public:
      // Constructor
      ExprBinary(expr_ptr left, frontend::Token op, expr_ptr right);

      // Return the operator of the unary expression
      frontend::TokenKind op();

      // Return the operands of the unary expression
      expr_ptr left();
      expr_ptr right();

      // Return if the resolved types of the operands match the specified types
      bool check_operand_type(internals::Type left_type, internals::Type right_type);

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines an echo expression
  class ExprEcho : public Expr, public std::enable_shared_from_this<ExprEcho>
  {
    private:
      // The keyword token of the echo expression
      frontend::Token keyword_;

      // The nested expression of the echo expression
      expr_ptr expr_;


    public:
      // Constructor
      ExprEcho(frontend::Token keyword, expr_ptr expr);

      // Return the nested expression of the echo expression
      expr_ptr expr();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines an if expression
  class ExprIf : public Expr, public std::enable_shared_from_this<ExprIf>
  {
    private:
      // The keyword token of the if expression
      frontend::Token keyword_;

      // The condition of the if expression
      expr_ptr condition_;

      // The true branch of the if expression
      expr_ptr true_branch_;

      // The false branch of the if expression
      std::optional<expr_ptr> false_branch_;


    public:
      // Constructor
      ExprIf(frontend::Token keyword, expr_ptr condition, expr_ptr true_branch, std::optional<expr_ptr> false_branch);

      // Return the condition of the if expression
      expr_ptr condition();

      // Return the branches of the if expression
      expr_ptr true_branch();

      // Return the false branch of the if expression
      expr_ptr false_branch();

      // Return if the if expression has a false branch
      bool has_false_branch();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a for expression
  class ExprFor : public Expr, public std::enable_shared_from_this<ExprFor>
  {
    private:
      // The keyword token of the for expression
      frontend::Token keyword_;

      // The name token of the for expression
      frontend::Token name_;

      // The iterable of the for expression
      expr_ptr iterable_;

      // The body of the for expression
      expr_ptr body_;


    public:
      // Constructor
      ExprFor(frontend::Token keyword, frontend::Token name, expr_ptr iterable, expr_ptr body);

      // Return the name of the for expression
      string_t name();

      // Return the iterable of the for expression
      expr_ptr iterable();

      // Return the body of the for expression
      expr_ptr body();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a while expression
  class ExprWhile : public Expr, public std::enable_shared_from_this<ExprWhile>
  {
    private:
      // The keyword token of the while expression
      frontend::Token keyword_;

      // The condition of the while expression
      expr_ptr condition_;

      // The body of the while expression
      expr_ptr body_;


    public:
      // Constructor
      ExprWhile(frontend::Token keyword, expr_ptr condition, expr_ptr body);

      // Return the condition of the while expression
      expr_ptr condition();

      // Return the body of the while expression
      expr_ptr body();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines an until expression
  class ExprUntil : public Expr, public std::enable_shared_from_this<ExprUntil>
  {
    private:
      // The keyword token of the until expression
      frontend::Token keyword_;

      // The condition of the until expression
      expr_ptr condition_;

      // The body of the until expression
      expr_ptr body_;


    public:
      // Constructor
      ExprUntil(frontend::Token keyword, expr_ptr condition, expr_ptr body);

      // Return the condition of the until expression
      expr_ptr condition();

      // Return the body of the until expression
      expr_ptr body();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a block expression
  class ExprBlock : public Expr, public std::enable_shared_from_this<ExprBlock>
  {
    private:
      // The expressions of the block expression
      std::vector<expr_ptr> exprs_;


    public:
      // Constructor
      ExprBlock(std::vector<expr_ptr> exprs);

      // Return the expressions of the block expression
      std::vector<expr_ptr> exprs();

      // Iterate over the expressions of the block expression
      std::vector<expr_ptr>::const_iterator begin();
      std::vector<expr_ptr>::const_iterator end();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };


  // Class that defines a def expression
  class ExprDef : public Expr, public std::enable_shared_from_this<ExprDef>
  {
    private:
      // The name token of the def expression
      frontend::Token name_;

      // The type of the def expression
      std::optional<type_expr_ptr> type_;

      // The value of the def expression
      expr_ptr value_;


    public:
      // Constructor
      ExprDef(frontend::Token name, std::optional<type_expr_ptr> type, expr_ptr value);

      // Return the name of the def expression
      string_t name();

      // Return the type of the def expression
      type_expr_ptr type();

      // Return if the def expression has a type
      bool has_type();

      // Return the value of the def expression
      expr_ptr value();

      // Expression implementation
      virtual frontend::Location& location() override;
      virtual void accept(const expr_visitor_ptr& visitor) override;
  };
}
