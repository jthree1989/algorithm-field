/*******************************************************
 * This file is part of PISCES.
 * Author: Leo
 *******************************************************/

#include <catch2/catch.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>  // NOTE Include this header for log Eigen matrix

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Eigenvalues>

using namespace Eigen;

TEST_CASE("Matrix", "[Core][.]") {
  Eigen::Matrix<float, 3, 3> default_matrix;

  SECTION("Matrix Constructor") {
    default_matrix << 1, 2, 3, 4, 5, 6, 7, 8, 9;
    SPDLOG_INFO("Default matrix of Eigen(ColumnMajor)_{}x{}:\n{}", default_matrix.rows(), default_matrix.cols(),
                default_matrix);
  }

  SECTION("Matrix SelfadjointView") {
    default_matrix = default_matrix.selfadjointView<Eigen::Upper>();
    SPDLOG_INFO("SelfadjointView matrix_{}x{}:\n{}", default_matrix.rows(), default_matrix.cols(), default_matrix);
  }
}

TEST_CASE("Matrix multi", "[Core][.]") {
  Eigen::Matrix<double, 3, 3> default_matrix;
  default_matrix << 1, 2, 3, 4, 5, 6, 7, 8, 9;
  auto x = Eigen::Matrix3d::Random();
  auto y = Eigen::Matrix3d::Random();
  auto z = x * y;
  Eigen::Matrix3d z_1 = x * y;

  SPDLOG_INFO("x:\n{}, \ny:\n{}, \nz:\n{}, \nz_1:\n{}", x, y, z, z_1);
}

TEST_CASE("Quaternion Slerp", "[Geometry][.]") {
  Eigen::Quaterniond q1 = Eigen::Quaterniond::UnitRandom();
  Eigen::Quaterniond q2;
  q2.coeffs() = -q1.coeffs();
}

TEST_CASE("SelfAdjointEigenSolver", "[Matrix][.]") {
  // 1. Create a random self-adjoint matrix
  Eigen::Matrix<double, 6, 6> random_6x6 = Eigen::Matrix<double, 6, 6>::Random();
  SPDLOG_INFO("Random_6x6:\n {}", random_6x6);
  Eigen::Matrix<double, 6, 1> eigen_vals = random_6x6.selfadjointView<Eigen::Lower>().eigenvalues();
  SPDLOG_INFO("Eigen values:\n {}", eigen_vals);
}

TEST_CASE("Alias", "[Matrix][.]") {
  Eigen::Matrix3d a;
  a << 1, 2, 3, 4, 5, 6, 7, 8, 9;
  SPDLOG_INFO("Here is the matrix a:\n{}", a);

  a = 0.5 * (a + a.transpose().eval());  // !!! do NOT do this !!!
  a = a.selfadjointView<Eigen::Upper>();
  SPDLOG_INFO("and the result of the aliasing effect:\n{}", a);
}

TEST_CASE("conservativeResizeLike", "[Matrix][.]") {
  Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> a;
  Eigen::Matrix3i tmp;
  tmp << 1, 2, 3, 4, 5, 6, 7, 8, 9;
  a = tmp;
  SPDLOG_INFO("Here is the matrix a:\n{}", a);

  a.conservativeResizeLike(Eigen::MatrixXi::Zero(4, 4));

  SPDLOG_INFO("The matrix a after conservativeResizeLike:\n{}", a);
}

TEST_CASE("Givens Rotation", "[Jacobi][.]") {
  Eigen::JacobiRotation<double> jacobi_rotation;
  Eigen::Vector2d vec_2d = {1, 2};
  jacobi_rotation.makeGivens(vec_2d(0), vec_2d(1));

  SPDLOG_INFO("Givens rotation: {}, {}", jacobi_rotation.c(), jacobi_rotation.s());
  Eigen::Matrix2d givens_mat;
  givens_mat << jacobi_rotation.c(), jacobi_rotation.s(), -jacobi_rotation.s(), jacobi_rotation.c();
  SPDLOG_INFO("Givens matrix: \n{}", givens_mat);
  SPDLOG_INFO("Adjoint of Givens matrix: \n{}", givens_mat.adjoint());
  Eigen::Vector2d vec_2d_givens = givens_mat.adjoint() * vec_2d;
  SPDLOG_INFO("vec_2d_givens: {}", vec_2d_givens);
  vec_2d.applyOnTheLeft(0, 1, jacobi_rotation.adjoint());
  SPDLOG_INFO("vec_2d: {}", vec_2d);
}

TEST_CASE("Basic linear solving", "[Linear algebra and decompositions]") {
  // change log pattern
  spdlog::set_pattern("%v");

  Matrix3f A;
  Vector3f b;
  // clang-format off
  A << 1, 2, 3, 
       4, 5, 6, 
       7, 8, 9;
  b << 3, 3, 4;
  // clang-format on
  SPDLOG_INFO("A:\n{}", A);
  SPDLOG_INFO("b:{}", b.transpose());

  Vector3f x = A.colPivHouseholderQr().solve(b);

  SPDLOG_INFO("x:{}", x.transpose());

  ColPivHouseholderQR<Matrix3f> qr_solver(A);
  x = qr_solver.solve(b);

  SPDLOG_INFO("x:{}", x.transpose());

  Matrix3i P = qr_solver.colsPermutation().toDenseMatrix();
  SPDLOG_INFO("P:\n{}", P);

  Matrix3f q = qr_solver.householderQ();
  SPDLOG_INFO("q:\n{}", q);
  MatrixXf thinQ(A.rows(), A.cols());
  thinQ.setIdentity();
  Matrix3f Q = q * thinQ;
  SPDLOG_INFO("Q:\n{}", Q);

  Matrix3f R = qr_solver.matrixR().template triangularView<Upper>();
  SPDLOG_INFO("R:\n{}", R);

  Matrix3f AP = A * P.cast<float>();
  Matrix3f QR = Q * R;
  SPDLOG_INFO("AP:\n{}", AP);
  SPDLOG_INFO("QR:\n{}", QR);
}
