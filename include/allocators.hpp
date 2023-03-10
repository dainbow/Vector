#pragma once

template <typename T, template <typename> class Pool>
class PoolAllocator {
 public:
  using value_type = T;

  template <typename U>
  struct rebind {
    using other = PoolAllocator<U, Pool>;
  };

 public:
  template <typename... Args>
  PoolAllocator(Args&&... args) : pool_(std::forward<Args>(args)...) {}

  T* allocate(const uint64_t amount);
  void deallocate(T* ptr, const uint64_t amount);
  T* reallocate(T* ptr, const uint64_t new_size);

 private:
  Pool<T> pool_;
};



template <typename T, template <typename> class Pool>
T* PoolAllocator<T, Pool>::allocate(const uint64_t amount) {
  return pool_.GetFreePoolEntry(amount)->Allocate(amount);
}

template <typename T, template <typename> class Pool>
void PoolAllocator<T, Pool>::deallocate(T* ptr, const uint64_t amount) {
  pool_.FindEntryByPtr(ptr)->Deallocate(ptr, amount);
}

template <typename T, template <typename> class Pool>
T* PoolAllocator<T, Pool>::reallocate(T* ptr, const uint64_t new_size) {
  return pool_.FindEntryByPtr(ptr)->Reallocate(ptr, new_size);
}
