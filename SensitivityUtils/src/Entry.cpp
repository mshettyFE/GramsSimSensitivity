#include <iostream>
#include <variant>
#include <stdexcept>
#include <string>
#include <any>

#include "Entry.h"

template <typename DType>
DType Entry::get_value(unsigned long int index) const{
  throw std::invalid_argument("DType is not int, double, float or string");
}

  // specializations of get_value to int, double, std::string
template <>  int Entry::get_value<int>(unsigned long int index) const{
  if (index >= data.size()){
      throw std::out_of_range("Index larger than datasize");
  }
  // get what type is stored at index
  int type_index = data[index].index();
  if(type_index != 0){ 
      char buff[100];
      snprintf(buff, sizeof(buff), "Data at index %ld is not an int",index);
      std::string err_msg = buff;
      throw std::out_of_range(err_msg);        
  }
  return std::get<int>(data[index]);
};

template <>  double Entry::get_value<double>(const unsigned long int index) const{
  if (index >= data.size()){
      throw std::out_of_range("Index larger than datasize");
  }
  // get what type is stored at index
  int type_index = data[index].index();
  if(type_index != 1){
      char buff[100];
      snprintf(buff, sizeof(buff), "Data at index %ld is not an double",index);
      std::string err_msg = buff;
      throw std::out_of_range(err_msg);        
  }
  return std::get<double>(data[index]);
};

template <>  float  Entry::get_value<float>(const unsigned long int index) const{
  if (index >= data.size()){
      throw std::out_of_range("Index larger than datasize");
  }
  // get what type is stored at index
  int type_index = data[index].index();
  if(type_index != 2){
      char buff[100];
      snprintf(buff, sizeof(buff), "Data at index %ld is not an float",index);
      std::string err_msg = buff;
      throw std::out_of_range(err_msg);        
  }
  return std::get<float>(data[index]);
};

template <>  std::string Entry::get_value<std::string>(const unsigned long int index) const{
  if (index >= data.size()){
      throw std::out_of_range("Index larger than datasize");
  }
  // get what type is stored at index
  int type_index = data[index].index();
  if(type_index != 3){
      char buff[100];
      snprintf(buff, sizeof(buff), "Data at index %ld is not an string",index);
      std::string err_msg = buff;
      throw std::out_of_range(err_msg);        
  }
  return std::get<std::string>(data[index]);
};

EntryRow Entry::DumpData(){
  return this->data;
}


void printEntries(const std::vector<Entry> rows){
  for(auto& row: rows){
    row.print();
  }
}
