///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include "LoadBalancer.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <thread>

LoadBalancer::LoadBalancer(const std::shared_ptr<FileWriter> file_writer,
                           const size_t block_size) {
  std::uint32_t threads_count =
      std::min(min_threads_count, std::thread::hardware_concurrency());

  for (uint32_t thread_num = 0; thread_num < threads_count; thread_num++) {
    hash_writers.push_back(
        std::unique_ptr<HashWriter>(new HashWriter(file_writer, block_size)));
    thread_group.create_thread(boost::bind(
        &HashWriter::Start, hash_writers[hash_writers.size() - 1].get()));
  }
}

void LoadBalancer::Stop() {
  for (uint16_t thread_num = 0; thread_num < hash_writers.size();
       thread_num++) {
    hash_writers[thread_num].get()->Stop();
  }
  thread_group.join_all();
}

void LoadBalancer::AddData(std::shared_ptr<char> data) {
  uint32_t min_thread_load = std::numeric_limits<uint32_t>::max();
  uint32_t selected_thread = 0;
  for (uint16_t thread_num = 0; thread_num < hash_writers.size();
       thread_num++) {
    auto thread_load = hash_writers[thread_num]->GetSize();
    if (thread_load == 0) {
      selected_thread = thread_num;
      break;
    }

    if (thread_load < min_thread_load) {
      min_thread_load = thread_load;
      selected_thread = thread_num;
    }
  }

  hash_writers[selected_thread]->AddData(data);
}
