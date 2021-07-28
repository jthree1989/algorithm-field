#include <catch2/catch.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h> // NOTE Include this header for log Eigen matrix
#include <iostream>

#include "ring_buffer.hpp"

TEST_CASE("ring_buffer_test","[ring_buffer]"){
  yvr::ring_buffer<int> rb_int(10);
  rb_int.push_back(10);
  rb_int.push_back(20);

  auto back = rb_int.back();
  std::cout << "back: " << back << std::endl;

  rb_int.push_back(1);
  rb_int.pop_front();
  rb_int.pop_front();
  back = rb_int.back();
  

  std::cout << "back: " << back << std::endl;

  rb_int.pop_front();
  rb_int.pop_front();
  std::cout << "back: " << rb_int.back() << std::endl;

}