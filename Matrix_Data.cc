#include "Matrix_Data.hpp"


fx::core::detail::Matrix_Data::alloc_type fx::core::detail::Matrix_Data::alloc_{};

fx::core::detail::Matrix_Data::Matrix_Data() : data_buffer_elements_(0)
    { }

/* fx::core::detail::Matrix_Data::Matrix_Data(int64_t elements) : data_buffer_elements_ (elements), data_{ reinterpret_cast<data_type *>(operator new(sizeof(data_type) * elements)), elements, buffer_type::take } {}
*/
fx::core::detail::Matrix_Data::Matrix_Data(int64_t elements) : data_buffer_elements_ (elements), data_{ alloc_.allocate(data_buffer_elements_ * sizeof(data_type)), static_cast<size_t>(data_buffer_elements_), buffer_type::take, &Matrix_Data::deallocate} {}


fx::core::detail::Matrix_Data::Matrix_Data(fx::core::detail::Matrix_Data const& base, fx::core::shallow_copy_)
  : data_buffer_elements_(base.data_buffer_elements_), data_(base.Data(), data_buffer_elements_, buffer_type::reference,
            hold_reference(base.data_))      // keep referenced partition alive
            { }

fx::core::detail::Matrix_Data::Matrix_Data(fx::core::detail::Matrix_Data const& base, fx::core::deep_copy_)
  : fx::core::detail::Matrix_Data::Matrix_Data(base.data_buffer_elements_)
            { 
                std::copy(base.Data(), base.Data() + data_buffer_elements_, Data());
}


    fx::core::detail::Matrix_Data::Matrix_Data(fx::core::detail::Matrix_Data && src) :  data_buffer_elements_ { src.data_buffer_elements_} , data_{ }
    { 
        swap(data_, src.data_);
    }



fx::core::detail::Matrix_Data& fx::core::detail::Matrix_Data::operator=(Matrix_Data const &other){
    fx::core::detail::Matrix_Data m1(other);
    swap(*this, m1);
    return *this;
}

fx::core::detail::Matrix_Data& fx::core::detail::Matrix_Data::operator=(Matrix_Data && src) {
    fx::core::detail::Matrix_Data m1(src);
    swap(*this, m1);
    return *this;
}


bool fx::core::detail::Matrix_Data::operator==(Matrix_Data const &other) const { return this->data_ == other.data_ && this->data_buffer_elements_ == other.data_buffer_elements_; }

bool fx::core::detail::Matrix_Data::operator!=(Matrix_Data const &other) const {
    return !(operator==(other));
}

bool fx::core::detail::Matrix_Data::operator==(Matrix_Data const &other) {
    return static_cast<const Matrix_Data &>(*this).operator==(other);
}

bool fx::core::detail::Matrix_Data::operator!=(Matrix_Data const &other) {
    return static_cast<const Matrix_Data &>(*this).operator!=(other);
}


    void fx::core::detail::swap(Matrix_Data &a, Matrix_Data &b)
    {
        std::swap(a.data_buffer_elements_, b.data_buffer_elements_);
        swap(a.data_, b.data_);
    }
