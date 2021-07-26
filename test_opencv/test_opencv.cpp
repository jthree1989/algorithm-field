#include <catch2/catch.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h> // NOTE Include this header for log Eigen matrix

#include <opencv2/core/core.hpp>

TEST_CASE("Mat", "[core]"){
	SECTION("Mat Constructor") {
    cv::Mat default_matrix = (cv::Mat_<double>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);;
    SPDLOG_INFO("Default mat of opencv_{}x{}:\n{}", 
                default_matrix.rows,
                default_matrix.cols,
                default_matrix);
  }
}
