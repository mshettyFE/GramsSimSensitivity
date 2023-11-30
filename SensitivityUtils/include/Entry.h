#ifndef ENTRY
#define ENTRY

#include "TTree.h"

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <variant>
#include "UsefulTypeDefs.h"

class Entry{
    // Entry is an abstract base class that must be inherited to be used
    // The inherited type should properly implement a valid contructor/descructor pair to populate data member
    // extract_key should also return a vector of appropriate size for the key
  public:
    Entry(){};
    ~Entry(){};
    // Generic printing method for Entry
    virtual void print() const = 0;
    // Get data[index]. The generic function defaults to returning an error.
    // You need to specialize the function to the correct datatype to get useful output (see associated .cpp file)
    template <typename DType>
    DType get_value(const unsigned long int index) const;
    // extract key from Entry (preferable should a std::tuple of some kind)
    virtual std::vector<int> extract_key() const = 0;
    // Interpret == and != as stating weather the events came from the same gamma ray
    EntryRow DumpData();
  protected:
    EntryRow data;
};

void printEntries(const std::vector<Entry> rows) ;

#endif
