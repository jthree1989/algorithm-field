#include <catch2/catch.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h> // NOTE Include this header for log Eigen matrix

#include <filesystem>
namespace fs = std::filesystem;
#include <set>
#include <regex>
#include <sstream>

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>

TEST_CASE("Mat", "[core][.]"){
	SECTION("Mat Constructor") {
    cv::Mat default_matrix = (cv::Mat_<double>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);;
    SPDLOG_INFO("Default mat of opencv_{}x{}:\n{}", 
                default_matrix.rows,
                default_matrix.cols,
                default_matrix);
  }
}

TEST_CASE("Fisheye stereo calibration", "[calib3d]"){
  SECTION("Read and load images"){
    fs::path current_path = fs::current_path();
    SPDLOG_INFO("Current path: {}", current_path);

    fs::path data_path = current_path.parent_path().parent_path() / "data"; 
    SPDLOG_INFO("Data path: {}", data_path);

    fs::path stereo_imgs_path = data_path / "stereo_fisheye_imgs";
    SPDLOG_INFO("Stereo images path: {}", stereo_imgs_path);

    std::map<size_t, std::string> left_img_paths, right_img_paths;
    std::string img_type;
    // Put all image file path into container and order them by number
    std::regex pattern("([a-zA-Z_/-]+)/([a-zA-Z]{4,5})([1-9]{1,2}).([a-zA-Z]+)");
    std::smatch match;
    for(const fs::directory_entry& image_path : fs::directory_iterator(stereo_imgs_path)){
      std::string path_str = image_path.path().string();
      std::regex_search(path_str, match, pattern);
      img_type = match[4];
      std::string stereo_type(match[2]);
      std::stringstream ss(match[3]);
      size_t index;
      ss >> index;

      if(stereo_type == "left")
        left_img_paths[index] = path_str;
      else
        right_img_paths[index] = path_str;
    }

    for(auto left_img_path : left_img_paths){
      SPDLOG_INFO("{} : {}", left_img_path.first, left_img_path.second);
      cv::Mat img = cv::imread(left_img_path.second);
      cv::imshow("left", img);
      cv::waitKey(100);
    }
    for(auto right_img_path : right_img_paths){
      SPDLOG_INFO("{} : {}", right_img_path.first, right_img_path.second);
      cv::Mat img = cv::imread(right_img_path.second);
      cv::imshow("right", img);
      cv::waitKey(100);
    }
  }
  SECTION("fisheye::stereoRectify"){

  }
}

TEST_CASE("Use regex parse string", "[.regex]"){
  std::string test_string("/repo/wy_code/open-source/algorithm-field/test_opencv/data/stereo_fisheye_imgs/right7.jpg");
  std::string simple_string("helloworlD/");
  std::regex pattern("[a-z/]*");

  if(std::regex_match(simple_string, pattern)){
    SPDLOG_INFO("Matched: {}", simple_string);
  }else{
    SPDLOG_INFO("Unmatched: {}", simple_string);
  }

  pattern = "([a-zA-Z_/-]+)/([a-zA-Z]{4,5})([1-9]{1,2}).([a-zA-Z]+)";
  std::smatch match;

  std::regex_search(test_string, match, pattern);
  SPDLOG_INFO("regex_search results:\n{}\n{}\n{}\n{}", match[0], match[1], match[2], match[3]);

}
