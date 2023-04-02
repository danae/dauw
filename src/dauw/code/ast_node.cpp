#include "ast_node.hpp"

namespace dauw
{
  // Return the type of the node
  Type& Node::type()
  {
    return type_.value();
  }

  // Return if the node has a type
  bool Node::has_type()
  {
    return type_.has_value();
  }

  // Set the type of the node
  void Node::set_type(Type& type)
  {
    type_ = std::make_optional(type);
  }

  // Set the type of the node to that of another node
  void Node::set_type_from(node_ptr node)
  {
    if (node->has_type())
      set_type(node->type());
  }

  // Return if the type matches the specified type
  bool Node::check_type(Type& type_to_check)
  {
    if (has_type())
      return type() == type_to_check;
    else
      return false;
  }
}
