#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>

#include "utilities.hpp"

template <class T>
class Vector {
 public:
  Vector();

  explicit Vector(const uint64_t size, T&& elem = T());

  Vector(const Vector<T>& vector);
  Vector(Vector<T>&& vector);

  Vector<T>& operator=(const Vector<T>& vector);
  Vector<T>& operator=(Vector<T>&& vector);

  ~Vector();

  bool empty() const;
  uint64_t size() const;
  uint64_t capacity() const;

  void reserve(const uint64_t new_capacity);
  void resize(const uint64_t new_size, T&& value);
  void shrink_to_fit();

  void clear();

  void push_back(T&& element);
  void pop_back();

  T& at(const uint64_t idx);
  const T& at(const uint64_t idx) const;

  T& operator[](const uint64_t idx);
  const T& operator[](const uint64_t idx) const;

  T& front();
  const T& front() const;

  T& back();
  const T& back() const;

  T* data();
  const T* data() const;

 private:
  const static uint64_t base_capacity = 8;
  const static uint64_t base_capacity_multiplier_ = 2;

  uint64_t size_;
  uint64_t capacity_;

  char* data_;
};

template <>
class Vector<bool> {
  class BitRef {
   public:
    BitRef(uint64_t* data, const uint8_t shift);

    BitRef& operator=(const bool element);
    operator bool();

   private:
    uint64_t* data_;
    uint8_t shift_;
  };

 public:
  Vector();

  explicit Vector(const uint64_t size, bool elem);

  Vector(const Vector<bool>& vector);
  Vector(Vector<bool>&& vector);

  Vector<bool>& operator=(const Vector<bool>& vector);
  Vector<bool>& operator=(Vector<bool>&& vector);

  ~Vector();

  bool empty() const;
  uint64_t size() const;
  uint64_t capacity() const;

  void reserve(const uint64_t new_capacity);
  void resize(const uint64_t new_size, bool);
  void shrink_to_fit();

  void clear();

  void push_back(bool element);
  void pop_back();

  BitRef at(const uint64_t idx);
  const BitRef at(const uint64_t idx) const;

  BitRef operator[](const uint64_t idx);
  const BitRef operator[](const uint64_t idx) const;

  BitRef front();
  const BitRef front() const;

  BitRef back();
  const BitRef back() const;

 private:
  static uint64_t GetBitIdx(const uint64_t bit_number);
  static uint64_t GetBitSize(const uint64_t bits_amount);

  const static uint8_t bit_divider = 64;
  const static uint64_t base_capacity = 64;
  const static uint64_t base_capacity_multiplier_ = 2;

  uint64_t size_;
  uint64_t capacity_;

  uint64_t* data_;
};

template <class T>
Vector<T>::Vector() : size_(0), capacity_(0), data_(nullptr) {}

template <class T>
Vector<T>::Vector(const uint64_t size, T&& elem)
    : size_(size),
      capacity_(base_capacity_multiplier_ * size),
      data_(new char[capacity_ * sizeof(T)]()) {
  Construct(data(), 0, size_, elem);
}

template <class T>
Vector<T>::Vector(const Vector<T>& vector)
    : size_(vector.size_),
      capacity_(vector.capacity_),
      data_(new char[vector.capacity_ * sizeof(T)]()) {
  Construct(data(), 0, size_, vector.data());
}

template <class T>
Vector<T>::Vector(Vector<T>&& vector)
    : size_(vector.size_), capacity_(vector.capacity_), data_(nullptr) {
  std::swap(data_, vector.data_);
}

template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& vector) {
  if (vector.size_ < size_) {
    Assign(data(), 0, vector.size_, vector.data());
    Destruct(data(), vector.size_, size_);
  } else {
    reserve(vector.size_);

    Assign(data(), 0, size_, vector.data());
    Construct(data(), size_, vector.size_, vector.data());
  }

  size_ = vector.size_;
}

template <class T>
Vector<T>& Vector<T>::operator=(Vector<T>&& vector) {
  ~Vector();
  Vector(std::forward<Vector<T>>(vector));
}

template <class T>
Vector<T>::~Vector() {
  if (data_) {
    Destruct(data(), 0, size_);
    delete[] data_;
  }

  size_ = 0;
  capacity_ = 0;
  data_ = nullptr;
}

template <class T>
bool Vector<T>::empty() const {
  return (size_ == 0);
}

template <class T>
uint64_t Vector<T>::size() const {
  return size_;
}

template <class T>
uint64_t Vector<T>::capacity() const {
  return capacity_;
}

template <class T>
void Vector<T>::reserve(uint64_t new_capacity) {
  if (new_capacity <= capacity_) {
    return;
  }

  new_capacity = std::max(new_capacity, capacity_ * base_capacity_multiplier_);

  data_ = Realloc(data(), size_, new_capacity);
  capacity_ = new_capacity;
}

template <class T>
void Vector<T>::resize(const uint64_t new_size, T&& elem) {
  if (new_size < size_) {
    Destruct(data(), new_size, size_);
  } else {
    reserve(new_size);

    Construct(data(), size_, new_size, elem);
  }

  size_ = new_size;
}

template <class T>
void Vector<T>::shrink_to_fit() {
  if (size_ == capacity_) {
    return;
  }

  data_ = Realloc(data(), size_, size_);
  capacity_ = size_;
}

template <class T>
void Vector<T>::clear() {
  Destruct(data(), 0, size_);
  size_ = 0;
}

template <class T>
void Vector<T>::push_back(T&& element) {
  if (capacity_ == 0) {
    reserve(base_capacity);
  } else {
    reserve(size_ + 1);
  }

  data()[size_++] = std::forward<T>(element);
}

template <class T>
void Vector<T>::pop_back() {
  assert(size_);

  data()[--size_].~T();
}

template <class T>
T& Vector<T>::at(const uint64_t idx) {
  assert(idx < size_);

  return data()[idx];
}

template <class T>
const T& Vector<T>::at(const uint64_t idx) const {
  assert(idx < size_);

  return data()[idx];
}

template <class T>
T& Vector<T>::operator[](const uint64_t idx) {
  return data()[idx];
}

template <class T>
const T& Vector<T>::operator[](const uint64_t idx) const {
  return data()[idx];
}

template <class T>
T& Vector<T>::front() {
  assert(size_);

  return data()[0];
}

template <class T>
const T& Vector<T>::front() const {
  assert(size_);

  return data()[0];
}

template <class T>
T& Vector<T>::back() {
  assert(size_);

  return data()[size_ - 1];
}

template <class T>
const T& Vector<T>::back() const {
  assert(size_);

  return data()[size_ - 1];
}

template <class T>
T* Vector<T>::data() {
  return reinterpret_cast<T*>(data_);
}

template <class T>
const T* Vector<T>::data() const {
  return reinterpret_cast<const T*>(data_);
}
