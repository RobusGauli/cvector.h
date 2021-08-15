#ifndef cvector_h
#define cvector_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Vector(Type_)                                                          \
  struct {                                                                     \
    Type_ *e;                                                                  \
    size_t size;                                                               \
    size_t cap;                                                                \
    bool initialized;                                                          \
  }

#define vector__setsize(tv, value) ((((tv)->e)) ? (((tv)->size) = (value)) : -1)

#define vector__setcap(tv, value) ((((tv) -> e)) ? (((tv) -> cap) = (value)): -1)

#define vector__init(tv)                                                       \
  do {                                                                         \
      ((tv)->cap) = 0;                                                         \
      ((tv)->size) = 0;                                                        \
      ((tv)->e) = NULL;                                                        \
      ((tv)->initialized) = true;                                              \
  } while (0)

#define vector__init_with_cap(tv, ncap)                                        \
  do {                                                                         \
      ((tv)->size) = 0;                                                        \
      ((tv)->e) = NULL;                                                        \
      ((tv)->cap) = 0;                                                         \
      vector__grow(tv, ncap);                                                  \
      ((tv)->initialized) = true;                                              \
  } while (0)

#define vector__grow(tv, ncap)                                                 \
  do {                                                                         \
    size_t cap = ((tv)->cap);                                                  \
    void *mem = malloc(sizeof(*((tv)->e)) * ncap);                             \
    memcpy(mem, ((tv)->e), sizeof(*((tv)->e)) * cap);                          \
    free((tv)->e);                                                             \
    ((tv)->e) = mem;                                                           \
    ((tv)->cap) = ncap;                                                        \
  } while (0);

#define vector__add(tv, value)                                                 \
  do {                                                                         \
    size_t size = ((tv)->size);                                                \
    size_t cap = ((tv)->cap);                                                  \
    if (size >= cap) {                                                         \
      size_t new_cap = (cap == 0) ? 1 : (cap * 2);                             \
      vector__grow(tv, new_cap);                                               \
    }                                                                          \
    (((tv)->e)[size]) = value;                                                 \
    ((tv)->size)++;                                                            \
  } while (0)

#define vector__size(tv) ((tv)->size)

#define vector__index(tv, i) &(((tv)->e)[i])

#define vector__index_cpy(tv, i) (((tv)->e)[i])

#define vector__cap(tv) ((tv)->cap)

#define vector__free(tv) (free((tv)->e))

#define vector__pop(tv)                                                        \
  (vector__setsize((tv), vector__size(tv) - 1),                                \
   vector__index((tv), vector__size(tv)))

#define vector__pop_cpy(tv)                                                    \
  (vector__setsize((tv), vector__size(tv) - 1),                                \
   vector__index_cpy((tv), vector__size(tv)))

#define vector__first(tv) (vector__index(tv, 0))
#define vector__first_cpy(tv) (vector__index_cpy(tv, 0))

#define vector__last(tv) (vector__index(tv, vector__size(tv) - 1))
#define vector__last_cpy(tv) (vector__index_cpy(tv, vector__size(tv) - 1))

#define vector__wrapped_buffer(tv) ((tv) -> e)

#define Vector_iterator(Type_)                                                 \
  struct {                                                                     \
    Type_ *ty;                                                                 \
    size_t current_index;                                                      \
  }

#define vector_iterator__init(it, tv)                                          \
  do {                                                                         \
    ((it)->ty) = (tv);                                                         \
    ((it)->current_index) = 0;                                                 \
  } while (0)

#define vector_iterator__done(it) ((((it) -> ty) != NULL) ? ((it)->current_index >= vector__size((it)->ty)): true)

#define vector_iterator__current_index(it) ((it)->current_index)

#define vector_iterator__next(it)                                              \
  (vector__index(((it)->ty), ((it)->current_index)++))

#define vector_iterator__next_cpy(it)                                          \
  (vector__index_cpy(((it)->ty), ((it)->current_index)++))

#define vector_iterator__peek(it) (vector__last((it)->ty))

#define vector_iterator__peek_cpy(it) (vector__last_cpy((it)->ty))

#define vector_iterator__peek_first(it) (vector__first((it)->ty))

#define vector_iterator__peek_first_cpy(it) (vector__first_cpy((it)->ty))

#define vector_iterator__wrapped_iterable(it) ((it) -> ty)

#endif /* cvector_h */
