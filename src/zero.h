#include <math.h>
#include <stdlib.h>

#include "cvector.h"

#ifndef cvector_zero_h
#define cvector_zero_h


#define Zero(Type_)                                                            \
  Vector(Type_) vector_type_t##Type_;                                          \
  Vector(vector_type_t##Type_ *) vector_type_pointer_t##Type_;                 \
  typedef struct {                                                             \
    vector_type_pointer_t##Type_ vector_type_pointer;                          \
    vector_type_t##Type_ *holder;                                              \
    vector_type_t##Type_ **next_holder;                                        \
    size_t gsize;                                                              \
  }

#define zero__blocksize(zero) (vector__size(&((zero)->vector_type_pointer)))
#define zero__gsize(zero) ((zero)->gsize)

#define zero__init(zero)                                                       \
  do {                                                                         \
    ((zero)->gsize) = 0;                                                       \
    vector__init(&((zero)->vector_type_pointer));                              \
  } while (0)

#define zero__add(zero, value)                                                 \
  do {                                                                         \
    size_t gsize = zero__gsize(zero);                                          \
    size_t block_size = zero__blocksize(zero);                                 \
    size_t available_size = (pow(2, block_size) - 1);                          \
                                                                               \
    if (gsize >= available_size) {                                             \
      typeof(((zero)->holder)) pointer =                                       \
          malloc(sizeof(**(((zero)->vector_type_pointer).e)));                 \
      int cap = pow(2, block_size);                                            \
      vector__init_with_cap(pointer, cap);                                     \
      vector__add(&((zero)->vector_type_pointer), pointer);                    \
    }                                                                          \
    typeof(((zero)->next_holder)) v = vector__index(                           \
        &((zero)->vector_type_pointer), zero__blocksize(zero) - 1);            \
    vector__add(*v, value);                                                    \
    ((zero)->gsize)++;                                                         \
  } while (0)

#define zero__size(zero) ((zero)->gsize)
#define zero__floor(expr) floor(expr)
#define zero__int(expr) ((int)(expr))

#define zero__index(zero, index)                                               \
  vector__index(                                                               \
      (*(vector__index(&((zero)->vector_type_pointer),                         \
                       zero__int(zero__floor(log2(index + 1)))))),             \
      index + 1 - zero__int(pow(2, zero__int(zero__floor(log2(index + 1))))))

#define zero__index_cpy(zero, index)                                               \
  vector__index_cpy(                                                               \
      (*(vector__index(&((zero)->vector_type_pointer),                         \
                       zero__int(zero__floor(log2(index + 1)))))),             \
      index + 1 - zero__int(pow(2, zero__int(zero__floor(log2(index + 1))))))

#define zero__first(zero) zero__index(zero, 0)
#define zero__last(zero) zero__index(zero, zero__size(zero) - 1)

#define zero__first_cpy(zero) zero__index_cpy(zero, 0)
#define zero__last_cpy(zero) zero__index_cpy(zero, zero__size(zero) - 1)

#define zero__free(zero)  \
  do { \
    size_t block_size = vector__size(&((zero) -> vector_type_pointer)); \
    for (int i = 0; i < block_size; i++) { \
      typeof(((zero) -> next_holder)) mem = vector__index(&((zero) -> vector_type_pointer), i); \
      vector__free(*(mem)); \
    } \
    vector__free(&((zero) -> vector_type_pointer)); \
  } while(0)

#define Zero_iterator(Type_)                                                 \
  typedef struct {                                                             \
    Type_ *ty;                                                                 \
    size_t current_index;                                                      \
  }

#define zero_iterator__init(it, tv)                                          \
  do {                                                                         \
    ((it)->ty) = (tv);                                                         \
    ((it)->current_index) = 0;                                                 \
  } while (0)

#define zero_iterator__done(it)                                              \
  ((((it)->ty) != NULL) ? ((it)->current_index >= zero__size((it)->ty))      \
                        : true)

#define zero_iterator__current_index(it) ((it)->current_index)

#define zero_iterator__next(it)                                              \
  ((((it) -> current_index)++), (zero__index(((it)->ty), ((it)->current_index)-1)))

#define zero_iterator__next_cpy(it)                                          \
  ((((it) -> current_index)++), (zero__index_cpy(((it)->ty), ((it)->current_index)-1)))

#define zero_iterator__peek(it) (zero__index(((it)->ty), ((it)->current_index)))

#define zero_iterator__peek_cpy(it) (zero__index_cpy(((it)->ty), ((it)->current_index)))

#define zero_iterator__wrapped_iterable(it) ((it)->ty)

#define zero_iterator__reset(it) (((it) -> current_index) = 0)

#endif /* cvector_zero_h */
