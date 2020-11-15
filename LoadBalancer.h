///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#ifndef _LOADBALANCER_H_
#define _LOADBALANCER_H_
#include "HashWriter.h"

#include <boost/thread.hpp>
#include <memory>

class LoadBalancer {
public:
  LoadBalancer(const std::shared_ptr<FileWriter> file_writer,
               const size_t block_size);
  void Stop();
  void AddData(std::shared_ptr<char> data);

private:
  std::vector<std::unique_ptr<HashWriter>> hash_writers;
  boost::thread_group thread_group;

  const uint32_t min_threads_count = 4;
};

#endif //_LOADBALANCER_H_
