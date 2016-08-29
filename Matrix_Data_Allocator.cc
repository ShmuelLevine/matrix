#include "Matrix_Data_Allocator.hpp"
#include <iostream>


fx::core::detail::Matrix_Allocator::data_type* fx::core::detail::Matrix_Allocator::allocate(int64_t n) {
    boost::lock_guard<mutex_type> l(mtx_);
    if (free_list_[n].empty()) {
        auto addr = static_cast<fx::core::detail::Matrix_Allocator::data_type*>(MKL_malloc(n, fx::core::detail::Matrix_Allocator::alignment));
        allocation_list_[addr] = n;
        return addr;
    }

    auto addr = free_list_[n].top();
    free_list_[n].pop();
    return addr;

}

void fx::core::detail::Matrix_Allocator::deallocate(fx::core::detail::Matrix_Allocator::T* ptr) {
    boost::lock_guard<mutex_type> l(mtx_);

    auto size = allocation_list_[ptr];

    free_list_[size].push(ptr);
}

fx::core::detail::Matrix_Allocator::~Matrix_Allocator() {
    boost::lock_guard<mutex_type> l(mtx_);

    if (allocation_list_.size() > 0)
        for (auto allocation : allocation_list_)
            MKL_free(allocation.first);
}

fx::core::detail::Matrix_Allocator::mutex_type fx::core::detail::Matrix_Allocator::mtx_{};
fx::core::detail::Matrix_Allocator::free_list_type fx::core::detail::Matrix_Allocator::free_list_{};
fx::core::detail::Matrix_Allocator::allocation_list_type fx::core::detail::Matrix_Allocator::allocation_list_{};
