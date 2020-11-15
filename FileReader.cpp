///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include "FileReader.h"

#include <iostream>

FileReader::FileReader(const std::string &filename) {
  stream.open(filename.data(), std::ios::binary);
  if (!stream) {
    std::cerr << "You should specify a correct input file path" << std::endl;
    throw std::runtime_error("Can't open file to read: " + filename);
  }

  if (!stream.is_open()) {
    std::cerr << "Problems with opening input file" << std::endl;
    throw std::runtime_error("File for read is close!");
  }

  stream.seekg(0, std::ios::end);
  file_size = stream.tellg();
  stream.seekg(0, std::ios::beg);
}

FileReader::~FileReader() { stream.close(); }

bool FileReader::ReadFinished() { return file_size <= 0; }

std::shared_ptr<char> FileReader::ReadData(size_t read_size) {
  std::shared_ptr<char> read_block(new char[read_size](),
                                   [](char *buf) { delete[] buf; });
  if (!read_block) {
    std::cerr << "Can't allocate read buffer, file size(rest) " << file_size
              << std::endl;
    throw std::runtime_error("Can't allocate read buffer");
  }

  if (file_size < read_size)
    read_size = file_size;

  try {
    stream.read(read_block.get(), read_size);
  } catch (const std::ios_base::failure &e) {
    std::cerr << "Read block failed, error: " << e.code().message()
              << ", file size(rest) " << file_size << std::endl;
    throw std::runtime_error("Read block failed");
  }

  file_size -= read_size;

  return read_block;
}
