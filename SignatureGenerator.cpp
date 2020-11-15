///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///

#include "SignatureGenerator.h"

#include <iostream>

SignatureGenerator::SignatureGenerator(const std::string &input_path,
                                       const std::string &output_path,
                                       const size_t block_size)
    : input_path(input_path), output_path(output_path), block_size(block_size) {
}

void SignatureGenerator::ProcessFile() {
  try {
    file_reader = std::unique_ptr<FileReader>(new FileReader(input_path));
  } catch (std::bad_alloc) {
    std::cerr << "Can't open input file" << std::endl;
    throw std::runtime_error("Can't open file to read: " + input_path);
  }

  try {
    file_writer = std::make_shared<FileWriter>(output_path);
  } catch (std::bad_alloc) {
    std::cerr << "Can't open output file" << std::endl;
    throw std::runtime_error("Can't open file to write: " + output_path);
  }

  try {
    load_balancer = std::unique_ptr<LoadBalancer>(
        new LoadBalancer(file_writer, block_size));
  } catch (std::bad_alloc) {
    std::cerr << "Can't create load balancer for reading" << std::endl;
    throw std::runtime_error("Can't create load balancer for reading");
  }

  while (!file_reader->ReadFinished()) {
    load_balancer->AddData(file_reader->ReadData(block_size));
  }

  load_balancer->Stop();
}
