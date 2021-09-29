#include <catch2/catch.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h> // NOTE Include this header for log Eigen matrix

#include <Eigen/Core>
#include <Eigen/Geometry>

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

TEST_CASE("Matrix multi", "[Core]"){
  Eigen::Matrix<double, 3, 3> default_matrix;
  default_matrix << 1, 2, 3, 
                    4, 5, 6, 
                    7, 8, 9;
  auto x = Eigen::Matrix3d::Random();
  auto y = Eigen::Matrix3d::Random();
  auto z = x * y;
  Eigen::Matrix3d z_1 = x * y;

  SPDLOG_INFO("x:\n{}, \ny:\n{}, \nz:\n{}, \nz_1:\n{}", x, y, z, z_1); 
}

TEST_CASE("Quaternion Slerp", "[Geometry]"){
  Eigen::Quaterniond q1 = Eigen::Quaterniond::UnitRandom();
  Eigen::Quaterniond q2;
  q2.coeffs() = -q1.coeffs();

  
}
