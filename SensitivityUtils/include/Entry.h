#ifndef ENTRY
#define ENTRY

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <variant>
#include "UsefulTypeDefs.h"

class Entry{
  // Entry is an abstract base class that must be inherited to be used
  // The inherited type should properly implement a valid contructor/descructor pair to populate data member
  // extract_key should also return a vector
  public:
    Entry(){};
    ~Entry(){};
  // Generic printing method for Entry
  virtual void print() = 0;
  // Get data[index]. The generic function defaults to returning an error.
  // You need to specialize the function to the correct datatype to get useful output (see associated .cpp file)
  template <typename DType>
  DType get_value(int index);
  // extract key from Entry (preferable should a std::tuple of some kind)
  virtual std::vector<int> extract_key() = 0;
  // Interpret == and != as stating weather the events came from the same gamma ray
  protected:
    EntryRow data;
};

void printEntries(std::vector<std::unique_ptr<Entry>> rows){
  for(auto& row: rows){
    row->print();
  }
}

#endif
