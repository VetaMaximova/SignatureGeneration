///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#ifndef CMAKEFILES_FILEWRITER_H_
#define CMAKEFILES_FILEWRITER_H_
#include <fstream>
#include <memory>
#include <mutex>

class FileWriter {
public:
  FileWriter(const std::string &filename);
  ~FileWriter();
  void WriteResult(const std::uint32_t data);

private:
  std::ofstream stream;
  std::uint32_t hash_res;
  std::mutex write_mutex;
};

#endif // CMAKEFILES_FILEWRITER_H_
