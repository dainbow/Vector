#pragma once

#include <iostream>

#include "stack_pool.hpp"
#include "page_pool.hpp"
#include "printf.hpp"

#include "allocators.hpp"

#include "vector.hpp"

template <typename T>
using StackAllocator = PoolAllocator<T, StackPool>;

template <typename T>
using PageAllocator = PoolAllocator<T, PagePool>;
