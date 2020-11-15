///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#ifndef _HASHWRITER_H_
#define _HASHWRITER_H_
#include "FileWriter.h"

#include <memory>
#include <mqueue.h>
#include <mutex>
#include <queue>
#include <semaphore.h>

class HashWriter {
public:
  HashWriter(const std::shared_ptr<FileWriter> file_writer,
             const size_t block_size);
  void Start();
  void AddData(std::shared_ptr<char> data);
  size_t GetSize();
  void Stop();

private:
  void CalculateAndWriteHash(const std::shared_ptr<char> &buffer);

  std::shared_ptr<FileWriter> file_writer;
  std::queue<std::shared_ptr<char>> buffer_queue;
  std::mutex buffer_queue_mutex;
  sem_t data_ready_semaphore;
  std::uint8_t stop;
  size_t block_size;

  static constexpr std::uint64_t sem_wait_nanos = 1000ULL;
};

#endif //_HASHWRITER_H_
