#pragma once

#include <dauw/common.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/internals/type.hpp>

namespace dauw::ast
{
  // Forward declarations
  class Node;

  // Type definitions for pointers
  using node_ptr = std::shared_ptr<Node>;


  // Base class that defines a node in the abstract syntax tree
  class Node
  {
    private:
      // The resolved type of the node
      std::optional<internals::Type> resolved_type_ = std::nullopt;


    public:
      // Destructor
      virtual ~Node() = default;

      // Return the resolved type of the node
      internals::Type& resolved_type();

      // Return if the node has a resolved type
      bool has_resolved_type();

      // Set the resolved type of the node
      void set_resolved_type(internals::Type& type);
      void set_resolved_type_from(node_ptr node);

      // Return if the resolved type matches the specified type
      bool check_type(internals::Type& type);

      // Return the location of the node
      virtual frontend::Location& location() = 0;
  };
}
