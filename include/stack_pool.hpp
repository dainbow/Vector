#pragma once

#include <cassert>
#include <cstdint>
#include <utility>

template <typename T>
class StackPool {
  static const uint64_t HeaderSignature = 0x22832997;
  static const uint64_t StackAmountMultiplier = 2;
  static const uint64_t StackSize = 0x400;

  struct Header {
    const uint64_t signature_ = HeaderSignature;
    uint64_t size_;

    Header(const uint64_t size);
  };

  struct Stack {
    char* begin_;
    char* end_;

    char* ptr_;

   public:
    Stack(const Stack& stack) = delete;
    Stack(Stack&& stack) = default;

    Stack& operator=(const Stack& stack) = delete;
    Stack& operator=(Stack&& stack) = default;

    Stack(const uint64_t amount);
    ~Stack();

    T* Allocate(const uint64_t amount);
    void Deallocate(T* ptr);
    T* Reallocate(T* ptr, const uint64_t new_size);

    bool IsFits(const uint64_t amount) const;

   private:
    Header* GetAreaHeader(T* ptr);
  };

 public:
  StackPool(const StackPool& pool) = delete;
  StackPool(StackPool&& pool) = default;

  StackPool& operator=(const StackPool& pool) = delete;
  StackPool& operator=(StackPool&& pool) = default;

  StackPool();
  ~StackPool();

  Stack* GetFreePoolEntry(const uint64_t size);
  Stack* FindEntryByPtr(T* ptr, const uint64_t amount = 0);

 private:
  uint64_t active_stack_;
  uint64_t stacks_amount_;
  Stack** stacks_;
};

template <typename T>
StackPool<T>::Header::Header(const uint64_t size) : size_(size) {}

template <typename T>
StackPool<T>::Stack::Stack(const uint64_t amount)
    : begin_(new char[amount * (sizeof(T) + sizeof(Header))]()),
      end_(begin_ + (amount * (sizeof(T) + sizeof(Header)))),
      ptr_(begin_) {}

template <typename T>
StackPool<T>::Stack::~Stack() {
  if (begin_ != nullptr) {
    delete[] begin_;
    begin_ = nullptr;
  }

  end_ = nullptr;
  ptr_ = nullptr;
}

template <typename T>
T* StackPool<T>::StackPool::Stack::Allocate(const uint64_t amount) {
  assert((ptr_ + sizeof(Header) + sizeof(T) * amount) <= end_);

  char* saved_ptr = ptr_;

  new (saved_ptr) Header(sizeof(T) * amount);
  ptr_ += sizeof(Header) + sizeof(T) * amount;

  return reinterpret_cast<T*>(saved_ptr + sizeof(Header));
}

template <typename T>
void StackPool<T>::StackPool::Stack::Deallocate(T* ptr) {
  char* char_ptr = reinterpret_cast<char*>(ptr);
  Header* header_ptr = GetAreaHeader(ptr);

  assert((char_ptr + header_ptr->size_) == ptr_);

  ptr_ -= (sizeof(Header) + header_ptr->size_);
}

template <typename T>
T* StackPool<T>::StackPool::Stack::Reallocate(T* ptr, uint64_t new_size) {
  if (ptr == nullptr) {
    return Allocate(new_size);
  }

  char* char_ptr = reinterpret_cast<char*>(ptr);

  Header* header_ptr = GetAreaHeader(ptr);
  uint64_t old_size = header_ptr->size_;

  assert((char_ptr + old_size) == ptr_);
  assert((char_ptr + new_size * sizeof(T)) <= end_);

  ptr_ += (new_size * sizeof(T) - old_size);
  header_ptr->size_ = new_size * sizeof(T);

  return ptr;
}

template <typename T>
bool StackPool<T>::StackPool::Stack::IsFits(const uint64_t amount) const {
  return (ptr_ + sizeof(Header) + amount * sizeof(T)) <= end_;
}

template <typename T>
StackPool<T>::Header* StackPool<T>::Stack::GetAreaHeader(T* ptr) {
  char* char_ptr = reinterpret_cast<char*>(ptr);
  Header* header_ptr = reinterpret_cast<Header*>(char_ptr - sizeof(Header));

  assert(header_ptr->signature_ == HeaderSignature);
  assert((header_ptr->size_ % sizeof(T)) == 0);

  return header_ptr;
}

template <typename T>
StackPool<T>::StackPool()
    : active_stack_(0), stacks_amount_(1), stacks_(new Stack*[1]()) {
  stacks_[active_stack_] = new Stack(StackSize);
}

template <typename T>
StackPool<T>::~StackPool() {
  if (stacks_ == nullptr) {
    return;
  }

  for (uint64_t stack_idx = 0; stack_idx < stacks_amount_; stack_idx++) {
    if (stacks_[stack_idx] != nullptr) {
      delete stacks_[stack_idx];
      stacks_[stack_idx] = nullptr;
    }
  }

  delete[] stacks_;
  stacks_ = nullptr;
}

template <typename T>
StackPool<T>::Stack* StackPool<T>::GetFreePoolEntry(const uint64_t amount) {
  assert(amount <= StackSize);

  if (!stacks_[active_stack_]->IsFits(amount)) {
    if ((active_stack_ + 1) > stacks_amount_) {
      Stack** new_stack_pool =
          new Stack*[stacks_amount_ * StackAmountMultiplier]();

      for (uint64_t stack_idx = 0; stack_idx < stacks_amount_; stack_idx++) {
        new_stack_pool[stack_idx] = stacks_[stack_idx];
      }

      delete[] stacks_;
      stacks_ = new_stack_pool;
    }

    stacks_[++active_stack_] = new Stack(StackSize);
  }

  return stacks_[active_stack_];
}

template <typename T>
StackPool<T>::Stack* StackPool<T>::FindEntryByPtr(T* t_ptr, const uint64_t amount) {
  char* ptr = reinterpret_cast<char*>(t_ptr);

  for (uint64_t stack_idx = 0; stack_idx < stacks_amount_; stack_idx++) {
    if ((ptr >= stacks_[stack_idx]->begin_) &&
        ((ptr + amount * sizeof(T)) <= stacks_[stack_idx]->end_)) {
      assert((stack_idx == active_stack_) &&
             "Stack allocator can deallocate only top memory areas");

      return stacks_[stack_idx];
    }
  }

  assert(nullptr && "INVALID PTR");
  return nullptr;
}
