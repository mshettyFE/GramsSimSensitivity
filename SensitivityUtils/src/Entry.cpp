#include <iostream>
#include <variant>
#include <stdexcept>
#include <format>
#include <any>

#include "Entry.h"

void Entry::print(){
    if(data.size == 0){
        std::cout << "no data in Entry";
    }
    // Adapted from here: https://en.cppreference.com/w/cpp/utility/variant/visit
    for (const auto& v: data){
        std::visit([](auto&& arg){ std::cout << arg << ' '; }, v);
    }
    std::cout << std::endl;
}


template <typename DType>
DType Entry::get_value(int index){
  throw std::invalid_argument("DType is not int, double, or string");
}

  // specializations of get_value to int, double, std::string
  template <> void Entry::get_value<int>(int index){
    if (index < 0){
        throw std::out_of_range("Index below 0");
    }
    if (index >= data.size){
        throw std::out_of_range("Index larger than datasize");
    }
    // get what type is stored at index
    int type_index = data[index].index()
    if(type_index != 0){
        std::string err_msg = std::format("Data at index {} is not an int",index);
        throw std::out_of_range(err_msg);        
    }
    return std::get<int>data[index];
  };

  template <> void Entry::get_value<double>(int index){
    if (index < 0){
        throw std::out_of_range("Index below 0");
    }
    if (index >= data.size){
        throw std::out_of_range("Index larger than datasize");
    }
    // get what type is stored at index
    int type_index = data[index].index()
    if(type_index != 1){
        std::string err_msg = std::format("Data at index {} is not an double",index);
        throw std::out_of_range(err_msg);        
    }
    return std::get<double>data[index];
  };

  template <> void Entry::get_value<float>(int index){
    if (index < 0){
        throw std::out_of_range("Index below 0");
    }
    if (index >= data.size){
        throw std::out_of_range("Index larger than datasize");
    }
    // get what type is stored at index
    int type_index = data[index].index()
    if(type_index != 2){
        std::string err_msg = std::format("Data at index {} is not an float",index);
        throw std::out_of_range(err_msg);        
    }
    return std::get<float>data[index];
  };

  template <> void Entry::get_value<std::string>(int index){
    if (index < 0){
        throw std::out_of_range("Index below 0");
    }
    if (index >= data.size){
        throw std::out_of_range("Index larger than datasize");
    }
    // get what type is stored at index
    int type_index = data[index].index()
    if(type_index != 3){
        std::string err_msg = std::format("Data at index {} is not an string",index);
        throw std::out_of_range(err_msg);        
    }
    return std::get<std::string>data[index];
  };