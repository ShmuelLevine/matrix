/*

This file is intended to be included by Matrix.hpp; this struct is a member of fx::core::Matrix.

This declaration is included in this file for the purpose of keeping the Matrix class more clear, and for facilitating the identification of the members of this struct, since it can be difficult to locate it within the declaration of Matrix.

 */



  /*! \brief Iterator class for Matrix
  *
  *
  *  This class defines an iterator type for Matrix.  This iterator
  * presently models a bidirectional iterator, which allows access to the
  * matrix's elements. It should not be difficult
  *  to change this to be a random-access iterator, but at this specific time
  * [2015.05.17] it should not be necessary.
  *
  *  The internal matrix major dimension might be padded transparently, to
  * provide better performance for the matrix.  The contents of those padding
  * elements
  *  is not part of the matrix's data and is ignored.  As such, operator[] will
  * reference only the data elements.
  *
  *  Note: At this time, the following expression evaluates to true:
  * (--matrix.end() == matrix.end()).  I.e. once the iterator passes the end of
  * the matrix, it can no longer be decremented
  *
  *  The element is referenced by a pair<size_t,size_t>.
  */
  class matrix_iterator {

  public:
    // Standard aliases for iterator types
    using T = float;
    using difference_type = std::ptrdiff_t; //< Although this isn't technically
    // just the difference between 2
    // ptrs, the type -- signed long long
    //-- is perfect in this instance
    using value_type = T;
    using reference = T &;
    using pointer = T *;
    using iterator_category =
        std::bidirectional_iterator_tag; // for now.  I'd like to make this into
                                         // a random-access iterator at some
                                         // point

  private:
    std::pair<size_t, size_t> current_element{
        0, 0}; //< index to current element.  This index skips over the padding
    // elements
    Matrix *mat_; //< Matrix reference.  Set to nullptr when iterator does
    // not point to an element.
    std::pair<size_t, size_t> matrix_dims_;
    friend class hpx::serialization::access;
    //  friend class boost::serialization::access;

  public:
    matrix_iterator() : mat_(nullptr) {}
    matrix_iterator(Matrix *matrix)
        : mat_(matrix),
          matrix_dims_{std::make_pair(matrix->specified_dims_.rows - 1,
                                      matrix->specified_dims_.cols - 1)} {}
    matrix_iterator(const Matrix *matrix)
        : mat_(const_cast<Matrix *>(matrix)),
          matrix_dims_{std::make_pair(matrix->specified_dims_.rows - 1,
                                      matrix->specified_dims_.cols - 1)} {}
    matrix_iterator(matrix_iterator const &other) = default;
    matrix_iterator &operator=(matrix_iterator other);
    matrix_iterator(matrix_iterator &&other)
        : current_element(std::move(other.current_element)),
          mat_(std::move(other.mat_)),
          matrix_dims_(std::move(other.matrix_dims_)) {}

    bool operator==(matrix_iterator const &other) const;
    bool operator!=(matrix_iterator const &other) const;

    // [2015.05.17]: this operation only applies to column-major matrices.  It
    // will need to be modified (to use traits for parent matrix to determine
    // which is the major dimension) if the matrix class is made to be more
    // generalized.
    // The iterator class uses the operator()(size_t,size_t) from the referenced
    // matrix.  it is expected: current_element_ <= matrix_dims_. The operator()
    // will find the correct element.
    matrix_iterator &operator++();
    matrix_iterator &operator--();
    matrix_iterator operator++(int);
    matrix_iterator operator--(int);

    reference operator*();
    reference operator*() const;

    difference_type operator-(matrix_iterator const &);
  };
