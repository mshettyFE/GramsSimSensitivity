#ifndef ENTRY
#define ENTRY

#include <string>
#include <vector>
#include <variant>

class Entry{
  public:
// Generic printing method for Entry
    virtual void print()  = 0;
// Get data[index] value out of Entry
    template <typename T>
    std::variant<int,double,std::string> get_value(int index);

// Empty constructor and destructor
    Entry(){};
    ~Entry(){};
  protected:
    std::vector<std::variant<int, double, std::string>> data;
};

#endif
