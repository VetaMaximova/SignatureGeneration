///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include "FileWriter.h"

#include <iostream>

FileWriter::FileWriter(const std::string &filename) {
  stream.open(filename.data(), std::ios::out);
  if (!stream) {
    std::cerr << "You should specify a correct input file path" << std::endl;
    throw std::runtime_error("Can't open file to read: " + filename);
  }

  if (!stream.is_open()) {
    std::cerr << "Problems with opening input file" << std::endl;
    throw std::runtime_error("File for read is close!");
  }
}

void FileWriter::WriteResult(const std::uint32_t data) {
  std::lock_guard<std::mutex> lock(write_mutex);
  hash_res ^= data;
}

FileWriter::~FileWriter() {
  stream.clear();
  stream << std::hex << hash_res;
  stream.close();
}
