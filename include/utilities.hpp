#pragma once

#include <cstdint>
#include <utility>

template <class T>
void Destruct(T* data, const uint64_t from, const uint64_t to) {
  for (uint64_t cur_idx = from; cur_idx < to; cur_idx++) {
    data[cur_idx].~T();
  }
}

template <class T>
void Construct(T* data, const uint64_t from, const uint64_t to,
               T&& elem = T()) {
  for (uint64_t cur_idx = from; cur_idx < to; cur_idx++) {
    new (data + cur_idx) T(elem);
  }
}

template <class T>
void Construct(T* data, const uint64_t from, const uint64_t to,
               const T* elements) {
  for (uint64_t cur_idx = from; cur_idx < to; cur_idx++) {
    new (data + cur_idx) T(elements[cur_idx]);
  }
}

template <class T>
void MoveConstruct(T* data, const uint64_t from, const uint64_t to,
                   T* elements) {
  for (uint64_t cur_idx = from; cur_idx < to; cur_idx++) {
    new (data + cur_idx) T(std::move(elements[cur_idx]));
  }
}

template <class T>
void Assign(T* data, const uint64_t from, const uint64_t to, T&& elem = T()) {
  for (uint64_t cur_idx = from; cur_idx < to; cur_idx++) {
    data[cur_idx] = elem;
  }
}

template <class T>
void Assign(T* data, const uint64_t from, const uint64_t to,
            const T* elements) {
  for (uint64_t cur_idx = from; cur_idx < to; cur_idx++) {
    data[cur_idx] = elements[cur_idx];
  }
}

template <class T>
void MoveAssign(T* data, const uint64_t from, const uint64_t to, T* elements) {
  for (uint64_t cur_idx = from; cur_idx < to; cur_idx++) {
    data[cur_idx] = std::move(elements[cur_idx]);
  }
}

// template <class T>
// char* Realloc(T* old_data, const uint64_t size, const uint64_t new_capacity) {
//   char* new_data = new char[new_capacity * sizeof(T)]();
//   T* converted_new_data = reinterpret_cast<T*>(new_data);

//   MoveConstruct(converted_new_data, 0, size, old_data);
//   Destruct(old_data, 0, size);

//   delete[] reinterpret_cast<char*>(old_data);

//   return new_data;
// }
