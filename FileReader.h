///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#ifndef _FILEREADER_H_
#define _FILEREADER_H_
#include <fstream>
#include <memory>

class FileReader {
public:
  FileReader(const std::string &filename);
  ~FileReader();
  bool ReadFinished();
  std::shared_ptr<char> ReadData(size_t read_size);

private:
  std::ifstream stream;
  std::uint32_t file_size;
};

#endif //_FILEREADER_H_
