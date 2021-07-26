#include <catch2/catch.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h> // NOTE Include this header for log Eigen matrix

#include <Eigen/Core>

TEST_CASE("Matrix", "[Core]"){
	SECTION("Matrix Constructor") {
    Eigen::Matrix<float, 3, 3> default_matrix;
    default_matrix << 1, 2, 3, 
                      4, 5, 6, 
                      7, 8, 9;
    SPDLOG_INFO("Default matrix of Eigen(ColumnMajor)_{}x{}:\n{}", 
                default_matrix.rows(),
                default_matrix.cols(),
                default_matrix);
  }
}
