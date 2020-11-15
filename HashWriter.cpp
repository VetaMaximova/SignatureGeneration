///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include "HashWriter.h"

#include <boost/crc.hpp>
#include <iostream>

HashWriter::HashWriter(const std::shared_ptr<FileWriter> file_writer,
                       const size_t block_size)
    : file_writer(file_writer), block_size(block_size) {
  int sem_init_result = sem_init(&data_ready_semaphore, 0, 0);
  if (sem_init_result) {
    std::cerr << "Problems with creating a semaphore" << std::endl;
    throw std::runtime_error("Problems with creating a semaphore");
  }
}

void HashWriter::AddData(std::shared_ptr<char> data) {
  {
    std::lock_guard<std::mutex> lock(buffer_queue_mutex);
    buffer_queue.push(data);
  }
  int sem_post_result = sem_post(&data_ready_semaphore);
  if (sem_post_result) {
    std::cerr << "Problems with creating a semaphore" << std::endl;
    throw std::runtime_error("Problems with creating a semaphore");
  }
}

size_t HashWriter::GetSize() {
  std::lock_guard<std::mutex> lock(buffer_queue_mutex);
  return buffer_queue.size();
}

void HashWriter::CalculateAndWriteHash(const std::shared_ptr<char> &buffer) {
  boost::crc_32_type result;
  result.process_bytes(buffer.get(), block_size);
  file_writer->WriteResult(result.checksum());
}

void HashWriter::Start() {
  std::shared_ptr<char> current_data;
  int sem_wait_result;
  struct timespec ts;
  ts.tv_nsec += sem_wait_nanos;

  while (!stop || GetSize() > 0) {
    while (GetSize() > 0) {
      {
        std::lock_guard<std::mutex> lock(buffer_queue_mutex);
        current_data = buffer_queue.front();
        buffer_queue.pop();
      }
      CalculateAndWriteHash(current_data);
    }

    while ((sem_wait_result = sem_timedwait(&data_ready_semaphore, &ts)) ==
               -1 &&
           errno == EINTR)
      continue;
  }
}

void HashWriter::Stop() { stop = 1; }
