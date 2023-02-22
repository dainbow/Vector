#include "../include/vector.hpp"

Vector<bool>::BitRef::BitRef(uint64_t* data, const uint8_t shift)
    : data_(data), shift_(shift) {}

Vector<bool>::BitRef& Vector<bool>::BitRef::operator=(const bool elem) {
  if (elem == 0) {
    *data_ &= ~(1 << shift_);
  } else {
    *data_ |= (1 << shift_);
  }

  return *this;
}

Vector<bool>::BitRef::operator bool() {
  return ((*data_) & (1 << shift_)) >> shift_;
}

Vector<bool>::Vector() : size_(0), capacity_(0), data_(nullptr) {}

Vector<bool>::Vector(const uint64_t size, bool elem)
    : size_(size),
      capacity_(base_capacity_multiplier_ * size),
      data_(new uint64_t[GetBitSize(capacity_)]()) {
  uint64_t elem_to_assign = 0;
  if (elem) {
    elem_to_assign = UINT64_MAX;
  }

  for (uint64_t cur_idx = 0; cur_idx < GetBitSize(size_); cur_idx++) {
    data_[cur_idx] = elem_to_assign;
  }
}

Vector<bool>::Vector(const Vector<bool>& vector)
    : size_(vector.size_),
      capacity_(vector.capacity_),
      data_(new uint64_t[GetBitSize(capacity_)]()) {
  for (uint64_t cur_idx = 0; cur_idx < GetBitSize(size_); cur_idx++) {
    data_[cur_idx] = vector.data_[cur_idx];
  }
}

Vector<bool>::Vector(Vector<bool>&& vector)
    : size_(vector.size_), capacity_(vector.capacity_), data_(nullptr) {
  std::swap(data_, vector.data_);
}

Vector<bool>& Vector<bool>::operator=(const Vector<bool>& vector) {
  reserve(vector.size_);
  for (uint64_t cur_idx = 0; cur_idx < GetBitSize(vector.size_); cur_idx++) {
    data_[cur_idx] = vector.data_[cur_idx];
  }

  size_ = vector.size_;
  return *this;
}

Vector<bool>& Vector<bool>::operator=(Vector<bool>&& vector) {
  Vector<bool>::~Vector();
  Vector(std::forward<Vector<bool>>(vector));

  return *this;
}

Vector<bool>::~Vector() {
  if (data_) {
    delete[] data_;
  }

  size_ = 0;
  capacity_ = 0;
  data_ = nullptr;
}

bool Vector<bool>::empty() const { return (size_ == 0); }

uint64_t Vector<bool>::size() const { return size_; }

uint64_t Vector<bool>::capacity() const { return capacity_; }

void Vector<bool>::reserve(uint64_t new_capacity) {
  if (GetBitSize(new_capacity) <= GetBitSize(capacity_)) {
    return;
  }

  new_capacity = std::max(new_capacity, capacity_ * base_capacity_multiplier_);

  uint64_t* new_data = new uint64_t[GetBitSize(new_capacity)]();
  for (uint64_t cur_idx = 0; cur_idx < GetBitSize(size_); cur_idx++) {
    new_data[cur_idx] = data_[cur_idx];
  }

  delete[] data_;

  data_ = new_data;
  capacity_ = new_capacity;
}

void Vector<bool>::resize(const uint64_t new_size, bool elem) {
  reserve(new_size);
  for (uint64_t cur_idx = size_; cur_idx < new_size; cur_idx++) {
    operator[](cur_idx) = elem;
  }

  size_ = new_size;
}

void Vector<bool>::shrink_to_fit() {
  if (GetBitSize(size_) == GetBitSize(capacity_)) {
    return;
  }

  uint64_t* new_data = new uint64_t[GetBitSize(size_)]();
  for (uint64_t cur_idx = 0; cur_idx < GetBitSize(size_); cur_idx++) {
    new_data[cur_idx] = data_[cur_idx];
  }

  delete[] data_;
  data_ = new_data;
  capacity_ = size_;
}

void Vector<bool>::clear() { size_ = 0; }

void Vector<bool>::push_back(bool element) {
  if (capacity_ == 0) {
    reserve(base_capacity);
  } else {
    reserve(size_ + 1);
  }

  operator[](size_++) = element;
}

void Vector<bool>::pop_back() {
  assert(size_);
  size_--;
}

Vector<bool>::BitRef Vector<bool>::at(const uint64_t idx) {
  assert(idx < size_);

  return {data_ + GetBitIdx(idx), static_cast<uint8_t>(idx % bit_divider)};
}

const Vector<bool>::BitRef Vector<bool>::at(const uint64_t idx) const {
  assert(idx < size_);

  return {data_ + GetBitIdx(idx), static_cast<uint8_t>(idx % bit_divider)};
}

Vector<bool>::BitRef Vector<bool>::operator[](const uint64_t idx) {
  return {data_ + GetBitIdx(idx), static_cast<uint8_t>(idx % bit_divider)};
}

const Vector<bool>::BitRef Vector<bool>::operator[](const uint64_t idx) const {
  return {data_ + GetBitIdx(idx), static_cast<uint8_t>(idx % bit_divider)};
}

Vector<bool>::BitRef Vector<bool>::front() {
  assert(size_);

  return {data_, 0};
}

const Vector<bool>::BitRef Vector<bool>::front() const {
  assert(size_);

  return {data_, 0};
}

Vector<bool>::BitRef Vector<bool>::back() {
  assert(size_);

  return {data_ + GetBitIdx(size_ - 1),
          static_cast<uint8_t>((size_ - 1) % bit_divider)};
}

const Vector<bool>::BitRef Vector<bool>::back() const {
  assert(size_);

  return {data_ + GetBitIdx(size_ - 1),
          static_cast<uint8_t>((size_ - 1) % bit_divider)};
}

uint64_t Vector<bool>::GetBitIdx(const uint64_t bit_number) {
  return bit_number / bit_divider;
}

uint64_t Vector<bool>::GetBitSize(const uint64_t bits_amount) {
  if (bits_amount == 0) {
    return 0;
  }

  return ((bits_amount - 1) / bit_divider) + 1;
}
