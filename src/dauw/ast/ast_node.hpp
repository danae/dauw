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
      // The type of the node
      std::optional<internals::Type> type_ = std::nullopt;


    public:
      // Destructor
      virtual ~Node() = default;

      // Return the type of the node
      internals::Type& type();

      // Return if the node has a type
      bool has_type();

      // Set the type of the node
      void set_type(internals::Type& type);
      void set_type_from(node_ptr node);

      // Return if the type matches the specified type
      bool check_type(internals::Type& type_to_check);

      // Return the location of the node
      virtual frontend::Location& location() = 0;
  };
}
