#include <math.h>
#include <stdlib.h>

#include "cvector.h"

#ifndef cvector_zero_h
#define cvector_zero_h

#define zero__int(expr) ((int)(expr))

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

#define zero__index(zero, index)                                               \
  vector__index(                                                               \
      (*(vector__index(&((zero)->vector_type_pointer),                         \
                       zero__int(zero__floor(log2(index + 1)))))),             \
      index + 1 - zero__int(pow(2, zero__int(zero__floor(log2(index + 1))))))

#endif /* cvector_zero_h */
