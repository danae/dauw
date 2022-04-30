#pragma once

#include "common.h"
#include "errors.h"
#include "token.h"


namespace dauw
{
  // Log an error
  void log_error(std::string message, Location location, std::string source);
  void log_error(Error error, std::string source);
}
