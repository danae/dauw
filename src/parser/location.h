#pragma once


namespace dauw
{
  // Class that defines a location in a source string
  class Location
  {
  public:
    // The line and column of the location
    unsigned int line;
    unsigned int col;

    // Constructor for a locatiom
    Location(unsigned int line, unsigned int col);
    Location();
  };
}
