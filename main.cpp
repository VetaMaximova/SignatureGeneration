/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: PC
 *
 * Created on 13 ноября 2020 г., 4:45
 */
#include "SignatureGenerator.h"

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

/*
 *
 */
const size_t cDefaultBlockSize = 1024 * 1024; // 1Mb

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "You should specify at least two args: input path, output path"
              << std::endl;
    throw std::runtime_error("Not enough arguments.");
  }

  std::string input_path = argv[1];
  if (!boost::filesystem::exists(input_path)) {
    std::cerr << "You've specified a wrong input path" << std::endl;
    throw std::runtime_error("File " + input_path + " should exists");
  }

  std::string output_path = argv[2];
  auto output_directory_path =
      boost::filesystem::path(output_path).parent_path();
  if (!boost::filesystem::exists(output_directory_path)) {
    std::cerr << "You've specified a wrong output path" << std::endl;
    throw std::runtime_error("Directory for " + output_path + " should exists");
  }

  size_t block_size = cDefaultBlockSize;
  try {
    block_size =
        argc > 3 ? boost::lexical_cast<size_t>(argv[3]) : cDefaultBlockSize;
  } catch (const boost::bad_lexical_cast &) {
    std::cerr << "Can't parse a block size parameter" << std::endl;
    throw std::runtime_error("Wrong argument block_size.");
  }

  SignatureGenerator signature_generator(input_path, output_path, block_size);
  signature_generator.ProcessFile();
  return 0;
}
