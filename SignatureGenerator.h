///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#ifndef _SIGNATUREGENERATOR_H_
#define _SIGNATUREGENERATOR_H_
#include "FileReader.h"
#include "FileWriter.h"
#include "LoadBalancer.h"

#include <memory>
#include <string>

class SignatureGenerator {
public:
  SignatureGenerator(const std::string &input_path,
                     const std::string &output_path, const size_t block_size);
  void ProcessFile();

private:
  std::string input_path;
  std::string output_path;
  std::uint32_t block_size;

  std::unique_ptr<FileReader> file_reader;
  std::shared_ptr<FileWriter> file_writer;
  std::unique_ptr<LoadBalancer> load_balancer;
};

#endif //_SIGNATUREGENERATOR_H_
