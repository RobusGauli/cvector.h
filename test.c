#include "src/cvector.h"

#include <assert.h>

void test__vector_init() {
  char *vector = NULL;
  vector__init(&vector);

  assert(vector != NULL);

  assert(vector__size(vector) == 0);
  assert(vector__cap(vector) == 0);

  vector__free(vector);
}

void test__null_vector() {
  char *vector = NULL;

  assert(vector__size(vector) == 0);
  assert(vector__size(vector) == 0);

  vector__free(vector);
}

void test__vector_add() {
  char *vector = NULL;

  vector__add(&vector, 'a');
  vector__add(&vector, 'b');
  vector__add(&vector, 'c');
  vector__add(&vector, 0); // null termination

  assert(vector__size(vector) == 4);
  assert(vector__cap(vector) == 4);
  // cap would be 2 * 4

  assert(strcmp(vector, "abc") == 0);
  // adding another character should increase the cap to 8
  vector__add(&vector, 'd');
  assert(vector__cap(vector) == 8);

  vector__free(vector);
}

void test__vector_setsize() {
  {
    char *vector = NULL;
    vector__init(&vector);

    assert(vector__size(vector) == 0);
    vector__setsize(vector, 10);
    assert(vector__size(vector) == 10);
    vector__free(vector);
  }
  {
    char *vector = NULL;
    vector__setsize(vector, 10);

    assert(vector__size(vector) == 0);
  }
}

void test__vector_setcap() {
  {
    char *vector = NULL;

    vector__init(&vector);

    vector__setcap(vector, 10);
    assert(vector__cap(vector) == 10);
    vector__free(vector);
  }
  {
    char *vector = NULL;

    vector__setcap(vector, 10);
    // should not set when unitialized
    assert(vector__cap(vector) == 0);
  }
}

void test__vector_loop() {
  int *numbers = NULL;
#define argcount (5)
  int inputs[argcount] = {4, 5, 6, 7, 8};
  for (int i = 0; i < argcount; i++) {
    vector__add(&numbers, inputs[i]);
  }

  for (int i = 0; i < vector__size(numbers); i++) {
    assert(numbers[i] == inputs[i]);
  }
#undef argcount
}

void test__iterator_new() {
  int *numbers = NULL;
  vector__add(&numbers, 4);
  vector_iterator_t iterator = vector_iterator__new(numbers);
  assert(iterator.done == false);
  assert(iterator.head == numbers);
  assert(iterator.size == sizeof(*numbers));
  assert(iterator.current_index == 0);
}

void test__null_iterator() {
  int *numbers = NULL;

  vector_iterator_t iterator = vector_iterator__new(numbers);
  assert(iterator.done == true);
  assert(iterator.current_index == 0);
  assert(iterator.head == NULL);
  assert(iterator.size == sizeof(*numbers));
}

void test__iteration() {
  int inputs[3] = {4, 5, 6};
  int *numbers = NULL;

  for (int i = 0; i < 3; i++) {
    vector__add(&numbers, inputs[i]);
  }

  vector_iterator_t iterator = vector_iterator__new(numbers);
  assert(iterator.done == false);

  assert(*((int *)(vector_iterator__next(&iterator))) == 4);
  assert(*((int *)(vector_iterator__peek(&iterator))) == 5);
  assert(*((int *)(vector_iterator__next(&iterator))) == 5);
  assert(*((int *)(vector_iterator__peek(&iterator))) == 6);
  assert(*((int *)(vector_iterator__next(&iterator))) == 6);

  assert(vector_iterator__done(&iterator) == true);

  vector__free(numbers);
}

int main() {
  test__null_vector();
  test__vector_init();
  test__vector_add();
  test__vector_setsize();
  test__vector_setcap();
  test__vector_loop();
  test__iterator_new();
  test__null_iterator();
  test__iteration();
}
