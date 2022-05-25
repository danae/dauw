#include "ast_node.hpp"

namespace dauw::ast
{
  // Return the resolved type of the node
  internals::Type& Node::resolved_type()
  {
    return resolved_type_.value();
  }

  // Return if the node has a resolved type
  bool Node::has_resolved_type()
  {
    return resolved_type_.has_value();
  }

  // Set the resolved type of the node
  void Node::set_resolved_type(internals::Type& type)
  {
    resolved_type_ = std::make_optional(type);
  }

  // Set the resolved type of the node to that of another node
  void Node::set_resolved_type_from(node_ptr node)
  {
    if (node->has_resolved_type())
      set_resolved_type(node->resolved_type());
  }

  // Return if the resolved type matches the specified type
  bool Node::check_type(internals::Type& type)
  {
    if (has_resolved_type())
      return resolved_type() == type;
    else
      return false;
  }
}
