#include "location.h"

//-----------------------------------------------------------------------------

// Constructor for a location
dauw::Location::Location(unsigned int line, unsigned int col)
{
  this->line = line;
  this->col = col;
}
dauw::Location::Location()
 : Location(0, 0)
{
}

//-----------------------------------------------------------------------------
