#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>

#include "memory.hpp"
#include "utilities.hpp"

template <typename T, template <typename> class Memory = DefaultMemory>
class Vector : public Memory<T> {
  class iterator : public std::iterator<std::random_access_iterator_tag, T> {
   public:
    iterator();
    iterator(T* ptr);
    iterator(iterator&& it) = default;
    iterator(const iterator& it) = default;

    ~iterator() = default;

    iterator& operator=(iterator&& it) = default;
    iterator& operator=(const iterator& it) = default;

    bool operator==(const iterator& it) const;
    bool operator!=(const iterator& it) const;
    bool operator<(const iterator& it) const;
    bool operator>(const iterator& it) const;
    bool operator>=(const iterator& it) const;
    bool operator<=(const iterator& it) const;

    T& operator*() const;

    iterator& operator++();
    iterator operator++(int);

    iterator& operator--();
    iterator operator--(int);

    iterator& operator+=(const std::ptrdiff_t diff);
    iterator& operator-=(const std::ptrdiff_t diff);

    iterator operator+(const std::ptrdiff_t diff) const;
    iterator operator-(const std::ptrdiff_t diff) const;

    std::ptrdiff_t operator-(const iterator& it) const;

    T& operator[](const std::ptrdiff_t diff) const;

   private:
    T* ptr_;
  };

  class const_iterator
      : public std::iterator<std::random_access_iterator_tag, T> {
   public:
    const_iterator();
    const_iterator(const T* ptr);
    const_iterator(const_iterator&& it) = default;
    const_iterator(const const_iterator& it) = default;

    ~const_iterator() = default;

    const_iterator& operator=(const_iterator&& it) = default;
    const_iterator& operator=(const const_iterator& it) = default;

    bool operator==(const const_iterator& it) const;
    bool operator!=(const const_iterator& it) const;
    bool operator<(const const_iterator& it) const;
    bool operator>(const const_iterator& it) const;
    bool operator>=(const const_iterator& it) const;
    bool operator<=(const const_iterator& it) const;

    const T& operator*() const;

    const_iterator& operator++();
    const_iterator operator++(int);

    const_iterator& operator--();
    const_iterator operator--(int);

    const_iterator& operator+=(const std::ptrdiff_t diff);
    const_iterator& operator-=(const std::ptrdiff_t diff);

    const_iterator operator+(const std::ptrdiff_t diff) const;
    const_iterator operator-(const std::ptrdiff_t diff) const;

    std::ptrdiff_t operator-(const const_iterator& it) const;

    const T& operator[](const std::ptrdiff_t diff) const;

   private:
    const T* ptr_;
  };

 public:
  Vector();

  explicit Vector(const uint64_t size, T&& elem = T());

  Vector(const Vector<T, Memory>& vector);
  Vector(Vector<T, Memory>&& vector);

  Vector<T, Memory>& operator=(const Vector<T, Memory>& vector);
  Vector<T, Memory>& operator=(Vector<T, Memory>&& vector);

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

  iterator begin();
  iterator end();

  const_iterator cbegin() const;
  const_iterator cend() const;

 private:
  const static uint64_t base_capacity = 8;
  const static uint64_t base_capacity_multiplier_ = 2;

  uint64_t size_;
  uint64_t capacity_;
};

template <>
class Vector<bool> {
 public:
  class BitRef {
   public:
    BitRef();
    BitRef(uint64_t* data, const uint64_t shift);
    BitRef(const BitRef& ref) = default;
    BitRef(BitRef&& ref) = default;

    BitRef& operator=(const BitRef& ref) = default;
    BitRef& operator=(BitRef&& ref) = default;

    bool operator==(const BitRef& it) const;
    bool operator!=(const BitRef& it) const;
    bool operator<(const BitRef& it) const;
    bool operator>(const BitRef& it) const;
    bool operator>=(const BitRef& it) const;
    bool operator<=(const BitRef& it) const;

    BitRef& operator=(const bool element);
    operator bool() const;

   protected:
    uint64_t* data_;
    uint64_t shift_;
  };

  class ConstBitRef {
   public:
    ConstBitRef();
    ConstBitRef(const uint64_t* data, const uint64_t shift);
    ConstBitRef(const ConstBitRef& ref) = default;
    ConstBitRef(ConstBitRef&& ref) = default;

    ConstBitRef& operator=(const ConstBitRef& ref) = default;
    ConstBitRef& operator=(ConstBitRef&& ref) = default;

    bool operator==(const ConstBitRef& it) const;
    bool operator!=(const ConstBitRef& it) const;
    bool operator<(const ConstBitRef& it) const;
    bool operator>(const ConstBitRef& it) const;
    bool operator>=(const ConstBitRef& it) const;
    bool operator<=(const ConstBitRef& it) const;

    operator bool() const;

   protected:
    const uint64_t* data_;
    uint64_t shift_;
  };

  class bit_iterator : public BitRef {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::size_t;
    using value_type = bool;
    using reference = BitRef&;
    using pointer = BitRef*;

    bit_iterator();
    bit_iterator(uint64_t* ptr, uint64_t shift);
    bit_iterator(const BitRef& ref);
    bit_iterator(pointer ptr);
    bit_iterator(bit_iterator&& it) = default;
    bit_iterator(const bit_iterator& it) = default;

    ~bit_iterator() = default;

    bit_iterator& operator=(bit_iterator&& it) = default;
    bit_iterator& operator=(const bit_iterator& it) = default;

    bool operator==(const bit_iterator& it) const;
    bool operator!=(const bit_iterator& it) const;
    bool operator<(const bit_iterator& it) const;
    bool operator>(const bit_iterator& it) const;
    bool operator>=(const bit_iterator& it) const;
    bool operator<=(const bit_iterator& it) const;

    reference operator*();

    bit_iterator& operator++();
    bit_iterator operator++(int);

    bit_iterator& operator--();
    bit_iterator operator--(int);

    bit_iterator& operator+=(difference_type diff);
    bit_iterator& operator-=(difference_type diff);

    bit_iterator operator+(const difference_type diff) const;
    bit_iterator operator-(const difference_type diff) const;

    difference_type operator-(const bit_iterator& it) const;

    reference operator[](const difference_type diff) const;
  };

  class const_bit_iterator : public ConstBitRef {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::size_t;
    using value_type = bool;
    using reference = ConstBitRef;
    using pointer = ConstBitRef*;

    const_bit_iterator();
    const_bit_iterator(const uint64_t* ptr, uint64_t shift);
    const_bit_iterator(const ConstBitRef& ref);
    const_bit_iterator(pointer ptr);
    const_bit_iterator(const_bit_iterator&& it) = default;
    const_bit_iterator(const const_bit_iterator& it) = default;

    ~const_bit_iterator() = default;

    const_bit_iterator& operator=(const_bit_iterator&& it) = default;
    const_bit_iterator& operator=(const const_bit_iterator& it) = default;

    bool operator==(const const_bit_iterator& it) const;
    bool operator!=(const const_bit_iterator& it) const;
    bool operator<(const const_bit_iterator& it) const;
    bool operator>(const const_bit_iterator& it) const;
    bool operator>=(const const_bit_iterator& it) const;
    bool operator<=(const const_bit_iterator& it) const;

    reference operator*() const;

    const_bit_iterator& operator++();
    const_bit_iterator operator++(int);

    const_bit_iterator& operator--();
    const_bit_iterator operator--(int);

    const_bit_iterator& operator+=(difference_type diff);
    const_bit_iterator& operator-=(difference_type diff);

    const_bit_iterator operator+(const difference_type diff) const;
    const_bit_iterator operator-(const difference_type diff) const;

    difference_type operator-(const const_bit_iterator& it) const;

    reference operator[](const difference_type diff) const;
  };

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

  bit_iterator begin();
  bit_iterator end();

  const_bit_iterator cbegin() const;
  const_bit_iterator cend() const;

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

template <typename T, template <typename> class Memory>
Vector<T, Memory>::iterator::iterator() : ptr_(nullptr) {}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::const_iterator::const_iterator() : ptr_(nullptr) {}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::iterator::iterator(T* ptr) : ptr_(ptr) {}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::const_iterator::const_iterator(const T* ptr) : ptr_(ptr) {}

template <typename T, template <typename> class Memory>
bool Vector<T, Memory>::iterator::operator==(const iterator& it) const {
  return ptr_ == it.ptr_;
}

template <typename T, template <typename> class Memory>
bool Vector<T, Memory>::const_iterator::operator==(
    const const_iterator& it) const {
  return ptr_ == it.ptr_;
}

template <typename T, template <typename> class Memory>
bool Vector<T, Memory>::iterator::operator!=(const iterator& it) const {
  return ptr_ != it.ptr_;
}

template <typename T, template <typename> class Memory>
bool Vector<T, Memory>::const_iterator::operator!=(
    const const_iterator& it) const {
  return ptr_ != it.ptr_;
}

template <typename T, template <typename> class Memory>
bool Vector<T, Memory>::iterator::operator<(const iterator& it) const {
  return ptr_ < it.ptr_;
}

template <typename T, template <typename> class Memory>
bool Vector<T, Memory>::const_iterator::operator<(
    const const_iterator& it) const {
  return ptr_ < it.ptr_;
}

template <typename T, template <typename> class Memory>
bool Vector<T, Memory>::iterator::operator>(const iterator& it) const {
  return ptr_ > it.ptr_;
}

template <typename T, template <typename> class Memory>
bool Vector<T, Memory>::const_iterator::operator>(
    const const_iterator& it) const {
  return ptr_ > it.ptr_;
}

template <typename T, template <typename> class Memory>
bool Vector<T, Memory>::iterator::operator>=(const iterator& it) const {
  return ptr_ >= it.ptr_;
}

template <typename T, template <typename> class Memory>
bool Vector<T, Memory>::const_iterator::operator>=(
    const const_iterator& it) const {
  return ptr_ >= it.ptr_;
}

template <typename T, template <typename> class Memory>
bool Vector<T, Memory>::iterator::operator<=(const iterator& it) const {
  return ptr_ <= it.ptr_;
}

template <typename T, template <typename> class Memory>
bool Vector<T, Memory>::const_iterator::operator<=(
    const const_iterator& it) const {
  return ptr_ <= it.ptr_;
}

template <typename T, template <typename> class Memory>
T& Vector<T, Memory>::iterator::operator*() const {
  return *ptr_;
}

template <typename T, template <typename> class Memory>
const T& Vector<T, Memory>::const_iterator::operator*() const {
  return *ptr_;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::iterator& Vector<T, Memory>::iterator::operator++() {
  ++ptr_;

  return *this;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::const_iterator&
Vector<T, Memory>::const_iterator::operator++() {
  ++ptr_;

  return *this;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::iterator Vector<T, Memory>::iterator::operator++(int) {
  ptr_++;

  return *this;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::const_iterator Vector<T, Memory>::const_iterator::operator++(
    int) {
  ptr_++;

  return *this;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::iterator& Vector<T, Memory>::iterator::operator--() {
  --ptr_;

  return *this;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::const_iterator&
Vector<T, Memory>::const_iterator::operator--() {
  --ptr_;

  return *this;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::iterator Vector<T, Memory>::iterator::operator--(int) {
  ptr_--;

  return *this;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::const_iterator Vector<T, Memory>::const_iterator::operator--(
    int) {
  ptr_--;

  return *this;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::iterator& Vector<T, Memory>::iterator::operator+=(
    const std::ptrdiff_t diff) {
  ptr_ += diff;

  return *this;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::const_iterator&
Vector<T, Memory>::const_iterator::operator+=(const std::ptrdiff_t diff) {
  ptr_ += diff;

  return *this;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::iterator& Vector<T, Memory>::iterator::operator-=(
    const std::ptrdiff_t diff) {
  ptr_ -= diff;

  return *this;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::const_iterator&
Vector<T, Memory>::const_iterator::operator-=(const std::ptrdiff_t diff) {
  ptr_ -= diff;

  return *this;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::iterator Vector<T, Memory>::iterator::operator+(
    const std::ptrdiff_t diff) const {
  iterator temp = *this;
  temp += diff;

  return temp;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::const_iterator Vector<T, Memory>::const_iterator::operator+(
    const std::ptrdiff_t diff) const {
  const_iterator temp = *this;
  temp += diff;

  return temp;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::iterator Vector<T, Memory>::iterator::operator-(
    const std::ptrdiff_t diff) const {
  iterator temp = *this;
  temp -= diff;

  return temp;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::const_iterator Vector<T, Memory>::const_iterator::operator-(
    const std::ptrdiff_t diff) const {
  const_iterator temp = *this;
  temp -= diff;

  return temp;
}

template <typename T, template <typename> class Memory>
std::ptrdiff_t Vector<T, Memory>::iterator::operator-(
    const iterator& it) const {
  return ptr_ - it.ptr_;
}

template <typename T, template <typename> class Memory>
std::ptrdiff_t Vector<T, Memory>::const_iterator::operator-(
    const const_iterator& it) const {
  return ptr_ - it.ptr_;
}

template <typename T, template <typename> class Memory>
T& Vector<T, Memory>::iterator::operator[](const std::ptrdiff_t diff) const {
  return ptr_[diff];
}

template <typename T, template <typename> class Memory>
const T& Vector<T, Memory>::const_iterator::operator[](
    const std::ptrdiff_t diff) const {
  return ptr_[diff];
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::Vector() : Memory<T>(0), size_(0), capacity_(0) {}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::Vector(const uint64_t size, T&& elem)
    : Memory<T>(size),
      size_(size),
      capacity_(base_capacity_multiplier_ * size) {
  Construct(this->data(), 0, size_, elem);
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::Vector(const Vector<T, Memory>& vector)
    : Memory<T>(vector.capacity_),
      size_(vector.size_),
      capacity_(vector.capacity_) {
  Construct(this->data(), 0, size_, vector.data());
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::Vector(Vector<T, Memory>&& vector)
    : Memory<T>(std::forward<decltype(vector)>(vector)),
      size_(vector.size_),
      capacity_(vector.capacity_) {}

template <typename T, template <typename> class Memory>
Vector<T, Memory>& Vector<T, Memory>::operator=(
    const Vector<T, Memory>& vector) {
  if (vector.size_ < size_) {
    Assign(this->data(), 0, vector.size_, vector.data());
    Destruct(this->data(), vector.size_, size_);
  } else {
    reserve(vector.size_);

    Assign(this->data(), 0, size_, vector.data());
    Construct(this->data(), size_, vector.size_, vector.data());
  }

  size_ = vector.size_;
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>& Vector<T, Memory>::operator=(Vector<T, Memory>&& vector) {
  ~Vector();
  Vector(std::forward<Vector<T, Memory>>(vector));
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::~Vector() {
  if (this->data()) {
    Destruct(this->data(), 0, size_);
  }

  size_ = 0;
  capacity_ = 0;
}

template <typename T, template <typename> class Memory>
bool Vector<T, Memory>::empty() const {
  return (size_ == 0);
}

template <typename T, template <typename> class Memory>
uint64_t Vector<T, Memory>::size() const {
  return size_;
}

template <typename T, template <typename> class Memory>
uint64_t Vector<T, Memory>::capacity() const {
  return capacity_;
}

template <typename T, template <typename> class Memory>
void Vector<T, Memory>::reserve(uint64_t new_capacity) {
  if (new_capacity <= capacity_) {
    return;
  }

  new_capacity = std::max(new_capacity, capacity_ * base_capacity_multiplier_);

  this->Realloc(size_, new_capacity);
  capacity_ = new_capacity;
}

template <typename T, template <typename> class Memory>
void Vector<T, Memory>::resize(const uint64_t new_size, T&& elem) {
  if (new_size < size_) {
    Destruct(this->data(), new_size, size_);
  } else {
    reserve(new_size);

    Construct(this->data(), size_, new_size, elem);
  }

  size_ = new_size;
}

template <typename T, template <typename> class Memory>
void Vector<T, Memory>::shrink_to_fit() {
  if (size_ == capacity_) {
    return;
  }

  this->Realloc(size_, size_);
  capacity_ = size_;
}

template <typename T, template <typename> class Memory>
void Vector<T, Memory>::clear() {
  Destruct(this->data(), 0, size_);
  size_ = 0;
}

template <typename T, template <typename> class Memory>
void Vector<T, Memory>::push_back(T&& element) {
  if (capacity_ == 0) {
    reserve(base_capacity);
  } else {
    reserve(size_ + 1);
  }

  this->data()[size_++] = std::forward<T>(element);
}

template <typename T, template <typename> class Memory>
void Vector<T, Memory>::pop_back() {
  assert(size_);

  this->data()[--size_].~T();
}

template <typename T, template <typename> class Memory>
T& Vector<T, Memory>::at(const uint64_t idx) {
  assert(idx < size_);

  return this->data()[idx];
}

template <typename T, template <typename> class Memory>
const T& Vector<T, Memory>::at(const uint64_t idx) const {
  assert(idx < size_);

  return this->data()[idx];
}

template <typename T, template <typename> class Memory>
T& Vector<T, Memory>::operator[](const uint64_t idx) {
  return this->data()[idx];
}

template <typename T, template <typename> class Memory>
const T& Vector<T, Memory>::operator[](const uint64_t idx) const {
  return this->data()[idx];
}

template <typename T, template <typename> class Memory>
T& Vector<T, Memory>::front() {
  assert(size_);

  return this->data()[0];
}

template <typename T, template <typename> class Memory>
const T& Vector<T, Memory>::front() const {
  assert(size_);

  return this->data()[0];
}

template <typename T, template <typename> class Memory>
T& Vector<T, Memory>::back() {
  assert(size_);

  return this->data()[size_ - 1];
}

template <typename T, template <typename> class Memory>
const T& Vector<T, Memory>::back() const {
  assert(size_);

  return this->data()[size_ - 1];
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::iterator Vector<T, Memory>::begin() {
  return {this->data()};
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::iterator Vector<T, Memory>::end() {
  return {this->data() + size_};
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::const_iterator Vector<T, Memory>::cbegin() const {
  return {this->data()};
}

template <typename T, template <typename> class Memory>
Vector<T, Memory>::const_iterator Vector<T, Memory>::cend() const {
  return {this->data() + size_};
}
