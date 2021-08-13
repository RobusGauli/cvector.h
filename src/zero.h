#include <math.h>
#include <stdlib.h>

#include "cvector.h"

#ifndef cvector_zero_h
#define cvector_zero_h

typedef struct zero_t {
  size_t gsize;
  size_t ***heads;
} zero_t;

#define zero__free(mem)                                                        \
  do {                                                                         \
    if (mem == NULL)                                                           \
      return;                                                                  \
    zero_t *z = (void *)(mem);                                                 \
    size_t len = vector__size(z->heads);                                       \
    for (int i = 0; i < len; i++) {                                            \
      size_t *head = *((z->heads)[i]);                                         \
      vector__free(head);                                                      \
    }                                                                          \
    free(z);                                                                   \
  } while (0);

#define zero__init(n)                                                          \
  do {                                                                         \
    zero_t *z = malloc(sizeof(zero_t));                                        \
    z->gsize = 0;                                                              \
    z->heads = NULL;                                                           \
    (*(zero_t **)(n)) = z;                                                     \
  } while (0);

#define zero__add(ty, mem, number)                                             \
  do {                                                                         \
    if (mem == NULL) {                                                         \
      zero__init(&mem);                                                        \
    }                                                                          \
    zero_t *z = (zero_t *)(mem);                                               \
    size_t current_global_size = z->gsize;                                     \
    size_t block_size = vector__size(z->heads);                                \
                                                                               \
    size_t available_size = (pow(2, block_size) - 1);                          \
                                                                               \
    if (current_global_size >= available_size) {                               \
      size_t **h = malloc(sizeof(size_t *));                                   \
      int cap = pow(2, vector__size(z->heads));                                \
      vector__add(&(z->heads), h);                                             \
      vector__init_with_cap(h, cap);                                           \
    }                                                                          \
    ty m = (ty)(*((z->heads)[vector__size(z->heads) - 1]));                    \
    vector__add(&m, (number));                                                 \
    z->gsize++;                                                                \
                                                                               \
  } while (0);

#define zero__size(x) ((x) ? (size_t)(((zero_t *)(x))->gsize) : 0)

#define zero_int(x) ((int)(x))

#define zero__index(numbers, index)                                            \
  (void *)(&(((char *)(*((((zero_t *)(numbers))->heads)[zero_int(              \
      log2((index) + 1))])))[((index) + 1 -                                    \
                              zero_int(pow(2, zero_int(log2((index) + 1))))) * \
                             sizeof(*(numbers))]))

#endif /* cvector_zero_h */
