#pragma once

#include <cassert>
#include <cstdint>
#include <utility>

template <typename T>
class PagePool {
  static constexpr uint64_t PageSize = 0x400;
  static constexpr uint64_t PageAmountMultiplier = 2;
  static constexpr uint64_t BitAmount = 64;
  static constexpr uint64_t BitFieldSize = PageSize / BitAmount;

  struct Page {
    char* begin_;
    char* end_;

    uint64_t bits_[BitFieldSize];
    uint64_t free_amount_;

    Page(const Page& page) = delete;
    Page(Page&& page) = default;

    Page& operator=(const Page& page) = delete;
    Page& operator=(Page&& page) = default;

    Page(const uint64_t amount);
    ~Page();

    T* Allocate(const uint64_t amount);
    void Deallocate(T* ptr, const uint64_t amount);
  };

 public:
  PagePool(const PagePool& pool) = delete;
  PagePool(PagePool&& pool) = default;

  PagePool& operator=(const PagePool& pool) = delete;
  PagePool& operator=(PagePool&& pool) = default;

  PagePool();
  ~PagePool();

  Page* GetFreePoolEntry(const uint64_t size);
  Page* FindEntryByPtr(T* ptr, const uint64_t amount = 0);

 private:
  uint64_t pages_amount_;
  uint64_t pages_allocated_;

  Page** pages_;
};

template <typename T>
PagePool<T>::Page::Page(const uint64_t amount)
    : begin_(new char[amount * sizeof(T)]()),
      end_(begin_ + amount * sizeof(T)),
      bits_(),
      free_amount_(amount) {}

template <typename T>
PagePool<T>::Page::~Page() {
  if (begin_ != nullptr) {
    delete[] begin_;
    begin_ = nullptr;
  }

  end_ = nullptr;
}

template <typename T>
T* PagePool<T>::PagePool::Page::Allocate(const uint64_t amount) {
  assert(amount == 1);

  for (uint64_t bitfield_idx = 0; bitfield_idx < BitFieldSize; bitfield_idx++) {
    int32_t clz = __builtin_clzl(~bits_[bitfield_idx]);

    if (clz != BitAmount) {
      bits_[bitfield_idx] |= (1ull << ((BitAmount - 1) - clz));
      free_amount_--;

      return reinterpret_cast<T*>(
          begin_ +
          ((BitAmount * bitfield_idx + ((BitAmount - 1) - clz))) * sizeof(T));
    }
  }

  assert(nullptr && "Can't find free space");
  return nullptr;
}

template <typename T>
void PagePool<T>::PagePool::Page::Deallocate(T* ptr, const uint64_t amount) {
  assert(amount == 1);

  char* char_ptr = reinterpret_cast<char*>(ptr);

  uint64_t bitfield_idx = (char_ptr - begin_) / BitAmount;
  uint64_t bitfield_shift = (char_ptr - begin_) % BitAmount;

  assert(bits_[bitfield_idx] | (1ull << bitfield_shift));

  free_amount_++;
  bits_[bitfield_idx] &= ~(1ull << bitfield_shift);
}

template <typename T>
PagePool<T>::PagePool()
    : pages_amount_(1), pages_allocated_(1), pages_(new Page*[1]()) {
  pages_[0] = new Page(PageSize);
}

template <typename T>
PagePool<T>::~PagePool() {
  if (pages_ == nullptr) {
    return;
  }

  for (uint64_t page_idx = 0; page_idx < pages_amount_; page_idx++) {
    if (pages_[page_idx] != nullptr) {
      delete pages_[page_idx];
      pages_[page_idx] = nullptr;
    }
  }

  delete[] pages_;
  pages_ = nullptr;
}

template <typename T>
PagePool<T>::Page* PagePool<T>::GetFreePoolEntry(const uint64_t amount) {
  assert(amount <= PageSize);

  for (uint64_t page_idx = 0; page_idx < pages_amount_; page_idx++) {
    if (pages_[page_idx]->free_amount_ != 0) {
      return pages_[page_idx];
    }
  }

  if ((pages_amount_ + 1) > pages_allocated_) {
    Page** new_page_pool = new Page*[pages_amount_ * PageAmountMultiplier]();

    for (uint64_t page_idx = 0; page_idx < pages_amount_; page_idx++) {
      new_page_pool[page_idx] = pages_[page_idx];
    }

    delete[] pages_;
    pages_ = new_page_pool;
  }

  return pages_[++pages_amount_];
}

template <typename T>
PagePool<T>::Page* PagePool<T>::FindEntryByPtr(T* t_ptr, const uint64_t amount) {
  char* ptr = reinterpret_cast<char*>(t_ptr);

  for (uint64_t page_idx = 0; page_idx < pages_amount_; page_idx++) {
    if ((ptr >= pages_[page_idx]->begin_) && ((ptr + amount * sizeof(T)) <= pages_[page_idx]->end_)) {
      return pages_[page_idx];
    }
  }

  assert(nullptr && "INVALID PTR");
  return nullptr;
}
