#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define vector__init(x)                                                        \
  do {                                                                         \
    if (*(x) == NULL) {                                                        \
      size_t *mem = malloc(sizeof(size_t) * 2);                                \
      mem[0] = 0;                                                              \
      mem[1] = 0;                                                              \
      *(x) = (void *)(&(mem[2]));                                              \
    }                                                                          \
  } while (0);

#define vector__size(x) ((x) ? ((size_t *)(x))[-2] : (size_t)(0))

#define vector__cap(x) ((x) ? ((size_t *)(x))[-1] : 0)

#define vector__setsize(x, size) ((x) ? ((size_t *)(x))[-2] = size : 0)

#define vector__setcap(x, cap) ((x) ? ((size_t *)(x))[-1] = cap : 0)

#define vector__free(x) ((x) ? free(&(((size_t *)(x))[-2])) : 0)

#define vector__grow(x)                                                        \
  do {                                                                         \
    size_t cap = vector__cap(*(x));                                            \
    size_t size = vector__size(*(x));                                          \
    if (size >= cap) {                                                         \
      size_t new_cap = cap == 0 ? 1 : cap * 2;                                 \
      size_t *mem = malloc(sizeof(**(x)) * new_cap + sizeof(size_t) * 2);      \
      mem[0] = size;                                                           \
      mem[1] = new_cap;                                                        \
      memcpy(&(mem[2]), *(x), sizeof(**(x)) * cap);                            \
      vector__free(*(x));                                                      \
      *(x) = (void *)(&(mem[2]));                                              \
    }                                                                          \
  } while (0);

#define vector__add(x, val)                                                    \
  do {                                                                         \
    if (*(x) == NULL) {                                                        \
      vector__init(x);                                                         \
    }                                                                          \
    vector__grow(x);                                                           \
    (*(x))[vector__size(*(x))] = val;                                          \
    vector__setsize(*(x), vector__size(*(x)) + 1);                             \
  } while (0);

#define vector__truncate(x)                                                    \
  do {                                                                         \
    vector__free((*(x)));                                                      \
    (*(x)) = NULL;                                                             \
    vector__init(x);                                                           \
  } while (0);

typedef struct vector_iterator_t {
  size_t current_index;
  bool done;
  void *head;
  size_t size;
} vector_iterator_t;

#define vector_iterator__next(iterator)                                        \
  ((((iterator)->head))                                                        \
       ? ((void *)(&(                                                          \
             (((char *)((iterator)->head))[(((iterator)->current_index))++ *   \
                                           (((iterator)->size))]))))           \
       : NULL)

#define vector_iterator__new(v)                                                \
  ((struct vector_iterator_t){.current_index = 0,                              \
                              .done = !(vector__size(v) > 0),                  \
                              .size = sizeof(*v),                              \
                              .head = v})

#define vector_iterator__peek(iterator)                                        \
  ((((iterator)->head))                                                        \
       ? ((void *)(&(                                                          \
             (((char *)((iterator)->head))[((iterator)->current_index) *       \
                                           (((iterator)->size))]))))           \
       : NULL)

#define vector_iterator__done(iterator)                                        \
  (((iterator)->current_index) >= vector__size(((iterator)->head)))
