#pragma once

#include "allocators.hpp"
#include "stack_pool.hpp"

template <typename T>
using StackAllocator = PoolAllocator<T, StackPool>;

template <typename T>
class DefaultMemory {
 public:
  DefaultMemory(const DefaultMemory& memory) = default;
  DefaultMemory(DefaultMemory&& memory) = default;

  DefaultMemory& operator=(const DefaultMemory& memory) = default;
  DefaultMemory& operator=(DefaultMemory&& memory) = default;

  DefaultMemory(const uint64_t initial_size);
  ~DefaultMemory();

  void Realloc(const uint64_t, const uint64_t new_capacity);

  T* data();
  const T* data() const;

 private:
  char* data_;
};

template <typename T>
class StackMemory {
 public:
  StackMemory(const StackMemory& memory) = default;
  StackMemory(StackMemory&& memory) = default;

  StackMemory& operator=(const StackMemory& memory) = default;
  StackMemory& operator=(StackMemory&& memory) = default;

  StackMemory(const uint64_t initial_size);
  ~StackMemory();

  void Realloc(const uint64_t, const uint64_t new_capacity);

  T* data();
  const T* data() const;

 private:
  StackAllocator<T> allocator_;
  T* data_;
};

template <typename T>
DefaultMemory<T>::DefaultMemory(const uint64_t initial_size) : data_(nullptr) {
  if (initial_size) {
    data_ = new char[initial_size * sizeof(T)];
  }
}

template <typename T>
DefaultMemory<T>::~DefaultMemory() {
  if (data_) {
    delete[] data_;
    data_ = nullptr;
  }
}

template <typename T>
void DefaultMemory<T>::Realloc(const uint64_t size,
                               const uint64_t new_capacity) {
  char* new_data = new char[new_capacity * sizeof(T)]();
  T* converted_new_data = reinterpret_cast<T*>(new_data);

  MoveConstruct(converted_new_data, 0, size, this->data());
  Destruct(this->data(), 0, size);

  delete[] data_;
  data_ = new_data;
}

template <typename T>
T* DefaultMemory<T>::data() {
  return reinterpret_cast<T*>(data_);
}

template <typename T>
const T* DefaultMemory<T>::data() const {
  return reinterpret_cast<T*>(data_);
}

template <typename T>
StackMemory<T>::StackMemory(const uint64_t initial_size)
    : allocator_(), data_(nullptr) {
  if (initial_size) {
    data_ = allocator_.allocate(initial_size);
  }
}

template <typename T>
StackMemory<T>::~StackMemory() {
  data_ = nullptr;
}

template <typename T>
void StackMemory<T>::Realloc(const uint64_t, const uint64_t new_capacity) {
  if (data_ == nullptr) {
    data_ = allocator_.allocate(new_capacity);

    return;
  }

  data_ = allocator_.reallocate(data(), new_capacity);
}

template <typename T>
T* StackMemory<T>::data() {
  return data_;
}

template <typename T>
const T* StackMemory<T>::data() const {
  return data_;
}
