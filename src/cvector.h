/*
 * 2021 Sept 07
 *
 *      Hope you give more than you take from this world.
 *
 ************************************************************************************
 *
 * This a main header file that contains private and public APIS for cvector.
 *
 * Implementation is quite straight forward. The most important #define macro
 * is 'CVector(T)' which takes in type T, where T can be any valid C types.
 * For example, if the user of this API wants to manage collection of ints, then
 * he/she should define a new type called 'CVector(int) cvector_int_t;'.
 *
 * Now, creating instance out of 'cvector_int_t' is as simple as 'cvector_int_t vector_a',
 * 'cvector_int_t vector_b', etc.
 *
 * In order to operate on the instance, API looks something like this.
 *
 * Adding an element to vector
 *
 * cvector__add(&vector_a, 1);
 * cvector__add(&vector_a, 2);
 *
 * cvector__add(&vector_b, 3);
 * cvector__add(&vector_b, 4);
 *
 * NOTE: First argument to 'cvector__add' or any other APIs must be address to vector i.e &vector_a.
 *
 * 'CVector(T)' will generate types during preprocessor stage from the macro to provide concrete
 *types holds the value of type 'T'.
 *
 * For example: If user defines CVector(int) cvector_int_t, this will generate following type.
 *
 * typedef struct {
 *   int* cvector__elem_m;
 *   size_t cvecor__size_m;
 *   size_t cvector__cap_m;
 *   bool cvector__initialized_m;
 * } cvector_int_t;
 *
 * If user defines CVector(char) cvector_char_t, this wil generate following type.
 * typedef struct {
 *   char* cvector__elem_m;
 *   size_t cvecor__size_m;
 *   size_t cvector__cap_m;
 *   bool cvector__initialized_m;
 * } cvector_char_t;
 *
 * Here 'cvector__elem_m' [private] points to the actual buffer of data that holds value of type
 *'char'. 'cvector__size_m' [private] keeps track of size and 'cvector__cap_m' keeps track of
 *capacity. 'cvector__initialized_m' [private] is used to determine if the initial constructor is
 *called or not.
 *
 * Initial constructor is called using 'cvector__init(&vector_a)'. This is used to set initial cap
 *and size to 0 and. buffer to 'NULL'.
 */

#ifndef cvector_h
#define cvector_h

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*
 * Macro to create type by wrapping container type i.e cvector__elem_type_.
 * This is root API before operating on vector such as adding, removing, indexing, etc.
 *
 * For example:
 *
 * To create Vector of type 'double'
 * CVector(double) cvector_double_t;
 *
 * Now to create instance of type 'cvector_double_t'
 *
 * int main() {
 *   cvector_double_t cvector_double;
 *
 *   // Initialization
 *   cvector__init(&cvector_double);
 *
 *  // Adding
 *  cvector__add(&cvector_double, 0.2);
 * }
 */
#define CVector(cvector__elem_type_)                                                               \
  typedef struct {                                                                                 \
    /* Pointer to buffer */                                                                        \
    cvector__elem_type_ *cvector__elem_m;                                                          \
    /* Size of Vector */                                                                           \
    size_t cvector__size_m;                                                                        \
    /* Cap of Vector */                                                                            \
    size_t cvector__cap_m;                                                                         \
    /* Flag to check whether vector is initialized or not. */                                      \
    bool cvector__initialized_m;                                                                   \
  }

/* Size should be more than or equal to shrink the vector. We won't bother
 * to shrink size if size is <= CVECTOR__MIN_SHRINK_SIZE.
 */
#ifndef CVECTOR__MIN_SHRINK_SIZE
#define CVECTOR__MIN_SHRINK_SIZE 32
#endif

/* If CVECTOR__LOAD_FACTOR <= 0.25, we shrink the size of the vector.
 * For instance, if Size = 64 & Cap = 256, Load Factor = 0.25, then we shrink the vector implicitly.
 */
#ifndef CVECTOR__LOAD_FACTOR
#define CVECTOR__LOAD_FACTOR 0.25
#endif

/* PRIVATE: macro to access data buffer */
#define cvector__elem_(vec) ((vec)->cvector__elem_m)
/* PRIVATE: macro to set data buffer */
#define cvector__set_elem_(vec, elem) ((cvector__elem_(vec)) = (elem))

/* PRIVATE: macro to access size of buffer's element */
#define cvector__elem_size_(vec) (sizeof((*(cvector__elem_(vec)))))

/* PUBLIC: macro to access size of vector */
#define cvector__size(vec) ((vec)->cvector__size_m)

/* PRIVATE: macro to set size of vector */
#define cvector__setsize_(vec, size) ((cvector__size(vec) = (size)))

/* PRIVATE: macro to access capacity */
#define cvector__cap_(vec) ((vec)->cvector__cap_m)

/* PRIVATE: macro set set capacity to vector */
#define cvector__setcap_(vec, cap) (cvector__cap_(vec) = (cap))

/* PRIVATE: macro to access initialization flag */
#define cvector__initialized_(vec) ((vec)->cvector__initialized_m)
/* PRIVATE: macro to set initialzation flag */
#define cvector__set_initialized_(vec, flag) (cvector__initialized_(vec) = (flag))

/* PUBLIC: macro function to initialized vector.
 *   - It sets cap to 0.
 *   - It sets size to 0.
 *   - It sets buffer to NULL.
 *   - Finally, it set initialization to `true`
 *
 *   USAGE:
 *
 *   CVector(int) cvector_int_t;
 *
 *   int main() {
 *     cvector_int_t cvector_int;
 *     cvector__init(&cvector_int);
 *   }
 */
#define cvector__init(vec)                                                                         \
  do {                                                                                             \
    cvector__setcap_((vec), 0);                                                                    \
    cvector__setsize_((vec), 0);                                                                   \
    cvector__set_elem_((vec), (NULL));                                                             \
    cvector__set_initialized_((vec), true);                                                        \
  } while (0)

/* PUBLIC: macro function to initialized vector with user defined capacity.
 *   - It sets cap to value passed by the user.
 *   - It sets size to 0.
 *   - It sets buffer to poniter to memory in heap.
 *   - Finally, it set initialization to `true`
 *
 *   USAGE:
 *
 *   CVector(int) cvector_int_t;
 *
 *   int main() {
 *     cvector_int_t cvector_int;
 *     cvector__init_with_cap(&cvector_int);
 *
 *     cvector__free(&cvector_int);
 *   }
 *
 *   NOTE: Need to call free, because heap allocation was made due to non-zero cap.
 *
 */
#define cvector__init_with_cap(vec, cap)                                                           \
  do {                                                                                             \
    cvector__setsize_((vec), 0);                                                                   \
    cvector__setcap_((vec), 0);                                                                    \
    cvector__set_elem_((vec), (NULL));                                                             \
    cvector__resize_((vec), (cap));                                                                \
    cvector__set_initialized_((vec), true);                                                        \
  } while (0)

/* PRIVATE: Resizes vector with new cap.
 * INVARIANTS:
 *  Cap must be more than equal to current vector's size.
 */
#define cvector__resize_(vec, cap)                                                                 \
  do {                                                                                             \
    void *cvector__mem_m = malloc((cvector__elem_size_(vec)) * (cap));                             \
    memcpy((cvector__mem_m), (cvector__elem_(vec)),                                                \
           ((cvector__elem_size_(vec)) * (cvector__size(vec))));                                   \
    free(cvector__elem_(vec));                                                                     \
    cvector__set_elem_((vec), (cvector__mem_m));                                                   \
    cvector__setcap_((vec), (cap));                                                                \
  } while (0)

/* PUBLIC: Add element to vector.
 * Resizes vector if current size is more than equal to current's cap.
 * Cap grows by powers of 2.
 */
#define cvector__add(vec, val)                                                                     \
  do {                                                                                             \
    if (cvector__size(vec) >= cvector__cap_(vec)) {                                                \
      cvector__resize_((vec), ((cvector__cap_(vec) == 0) ? 1 : ((cvector__cap_(vec)) * 2)));       \
    }                                                                                              \
    (((cvector__elem_(vec))[cvector__size(vec)]) = (val));                                         \
    cvector__setsize_((vec), (cvector__size(vec) + 1));                                            \
  } while (0)

/* PUBLIC: Set element to vector at index.
 * Returns -1 (error) if element is not set because index is out of bound.
 */
#define cvector__set_at_index(vec, index, val)                                                     \
  (((index) < cvector__size(vec)) ? (((cvector__elem_(vec))[(index)]) = (val), 0) : -1)

/* PUBLIC: Returns element at given index. */
#define cvector__index(vec, index) (((cvector__elem_(vec))[(index)]))

/* PUBLIC: Returns ref to element at given index. */
#define cvector__index_ref(vec, index) (&((cvector__elem_(vec))[(index)]))

/* PUBLIC: Frees the vector and sets buffer to NULL. */
#define cvector__free(vec)                                                                         \
  do {                                                                                             \
    free(cvector__elem_(vec));                                                                     \
    cvector__set_elem_((vec), NULL);                                                               \
  } while (0)

/* PRIVATE: Shrinks the vector is Load Factor is less than CVECTOR__LOAD_FACTOR.
 * Also, resize happens only when Size is >= CVECTOR__MIN_SHRINK_SIZE.
 */
#define cvector__shrink_(vec)                                                                      \
  do {                                                                                             \
    double cvector__current_load_factor =                                                          \
        (((double)cvector__size(vec)) / ((double)cvector__cap_(vec)));                             \
    if ((cvector__current_load_factor <= CVECTOR__LOAD_FACTOR) &&                                  \
        (cvector__size(vec) >= CVECTOR__MIN_SHRINK_SIZE)) {                                        \
      cvector__resize_((vec), (cvector__size(vec) * 2));                                           \
    }                                                                                              \
  } while (0)

/* PUBLIC: Pops the element from the last index in the vector. */
#define cvector__pop(vec)                                                                          \
  ({                                                                                               \
    cvector__shrink_(vec);                                                                         \
    cvector__setsize_((vec), (cvector__size(vec) - 1));                                            \
    (cvector__index((vec), cvector__size(vec)));                                                   \
  })

/* PUBLIC: Peek first element in the vector. */
#define cvector__first(vec) (cvector__index((vec), 0))
/* PUBLIC: Peek reference to first element in the vector. */
#define cvector__first_ref(vec) (cvector__index_ref((vec), 0))

/* PUBLIC: Peek last element in the vector. */
#define cvector__last(vec) (cvector__index((vec), cvector__size(vec) - 1))
/* PUBLIC: Peek reference to last element in the vector. */
#define cvector__last_ref(vec) (cvector__index_ref((vec), cvector__size(vec) - 1))

/* PUBLIC: Returns internal buffer to user. (HANDLE WITH CARE) */
#define cvector__wrapped_buffer(vec) cvector__elem_(vec)

/*
 * Macro to create iterator type by wrapping vector type i.e cvector__type_.
 * This is root API before operating on Iterator such as next(), done(), etc.
 *
 * For example:
 *
 * #include "stdio.h"
 * #include "cvector.h"
 *
 * Cvector(int) cvector_int_t;
 *
 * To create Iterator type by wrapping vector type.
 * CVector_iterator(cvector_int_t) cvector_iterator_t;
 *
 * int main() {
 *   cvector_int_t cvector_int;
 *   cvector__init(&cvector_int);
 *
 *   cvector__add(&cvector_int, 1);
 *   cvector__add(&cvector_int, 2);
 *
 *   cvector_iterator_t cvector_iterator;
 *
 *   // Initialization
 *   cvector_iterator__init(&cvector_iterator, &cvector_int);
 *
 *   for(;;) {
 *     if (cvector_iterator__done(&cvector_int)) break;
 *
 *     int value = cvector_iterator__next(&cvector_int);
 *     printf("Got value: %d\n", value);
 *   }
 * }
 */
#define CVector_iterator(cvector__type_)                                                           \
  typedef struct {                                                                                 \
    /* Pointer to vector */                                                                        \
    cvector__type_ *cvector_iterator__vec_m;                                                       \
    /* Keep track of current index. */                                                             \
    size_t cvector_iterator__current_index_m;                                                      \
  }

/* PRIVATE: Access pointer to vector. */
#define cvector_iterator__vec_(iterator) ((iterator)->cvector_iterator__vec_m)
/* PRIVATE: Sets vector in iterator struct. */
#define cvector_iterator__set_vec_(iterator, vec) ((cvector_iterator__vec_(iterator)) = (vec))

/* PRIVATE: Access current index from iterator. */
#define cvector_iterator__current_index_(iterator) ((iterator)->cvector_iterator__current_index_m)
/* PRIVATE: Sets current index. */
#define cvector_iterator__set_current_index_(iterator, index)                                      \
  ((cvector_iterator__current_index_(iterator)) = (index))

/* PUBLIC: Macro to initialize iterator. It sets current index to 0 and sets pointer to vector. */
#define cvector_iterator__init(iterator, vec)                                                      \
  do {                                                                                             \
    cvector_iterator__set_vec_((iterator), (vec));                                                 \
    cvector_iterator__set_current_index_((iterator), 0);                                           \
  } while (0)

/* PUBLIC: Macro that returns bool on whether the iterator is done or not. */
#define cvector_iterator__done(iterator)                                                           \
  (((cvector_iterator__vec_(iterator)) != (NULL))                                                  \
       ? ((cvector_iterator__current_index_(iterator)) >=                                          \
          cvector__size(cvector_iterator__vec_(iterator)))                                         \
       : (true))

/* PUBLIC: Macro that returns next element from the iterator. */
#define cvector_iterator__next(iterator)                                                           \
  (cvector__index((cvector_iterator__vec_(iterator)),                                              \
                  (((cvector_iterator__current_index_(iterator))++))))

/* PUBLIC: Macro that returns reference to next element from the iterator. */
#define cvector_iterator__next_ref(iterator)                                                       \
  (cvector__index_ref((cvector_iterator__vec_(iterator)),                                          \
                      (((cvector_iterator__current_index_(iterator))++))))

/* PUBLIC: Macro that returns next element but won't increment the index. (PEEK) */
#define cvector_iterator__peek(iterator)                                                           \
  (cvector__index((cvector_iterator__vec_(iterator)), (cvector_iterator__current_index_(iterator))))

/* PUBLIC: Macro that returns reference to next element but won't increment the index (PEEK) */
#define cvector_iterator__peek_ref(iterator)                                                       \
  (cvector__index_ref((cvector_iterator__vec_(iterator)),                                          \
                      (cvector_iterator__current_index_(iterator))))

/* PUBLIC: Macro that returns the iterator/vector */
#define cvector_iterator__wrapped_iterable(iterator) ((cvector_iterator__vec_(iterator)))

/* PUBLIC: Macro that resets the current index to 0. (seek(0))*/
#define cvector_iterator__reset(iterator) ((cvector_iterator__set_current_index_((iterator), 0)))

#endif /* cvector_h */
