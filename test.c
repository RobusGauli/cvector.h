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

void test__vector_with_struct() {
  struct Node {
    int x;
    int y;
    int z;
  };

  struct Node *nodes = NULL;

  vector__add(&nodes, ((struct Node){.x = 1, .y = 2, .z = 3}));
  vector__add(&nodes, ((struct Node){.x = 4, .y = 5, .z = 6}));
  vector__add(&nodes, ((struct Node){.x = 7, .y = 8, .z = 9}));

  assert(vector__size(nodes) == 3);

  vector_iterator_t iterator = vector_iterator__new(nodes);

  assert(vector_iterator__done(&iterator) == false);

  {
    struct Node *node = vector_iterator__next(&iterator);
    assert(node->x == 1);
    assert(node->y == 2);
    assert(node->z == 3);
  }

  {
    struct Node *node = vector_iterator__next(&iterator);
    assert(node->x == 4);
    assert(node->y == 5);
    assert(node->z == 6);
  }

  {
    struct Node *node = vector_iterator__next(&iterator);
    assert(node->x == 7);
    assert(node->y == 8);
    assert(node->z == 9);
  }
}

void test__vector_with_list_string() {
  char **buffers = NULL;

  vector__add(&buffers, "zero");
  vector__add(&buffers, "first");
  vector__add(&buffers, "second");
  vector__add(&buffers, "third");

  assert(vector__size(buffers) == 4);

  char *values[4] = {"zero", "first", "second", "third"};

  vector_iterator_t iterator = vector_iterator__new(buffers);

  assert(vector_iterator__done(&iterator) == false);

  for (;;) {
    if (vector_iterator__done(&iterator)) {
      break;
    }

    int index = iterator.current_index;
    char **value = vector_iterator__next(&iterator);

    assert(strcmp(*value, values[index]) == 0);
  }
}

void test__vector_truncate() {
  int *numbers = NULL;

  vector__add(&numbers, 0);
  vector__add(&numbers, 1);
  vector__add(&numbers, 2);

  assert(vector__size(numbers) == 3);

  vector__truncate(&numbers);

  assert(vector__size(numbers) == 0);
  assert(vector__cap(numbers) == 0);

  vector__add(&numbers, 100);
  vector__add(&numbers, 200);

  assert(vector__size(numbers) == 2);

  assert(numbers[0] == 100);
  assert(numbers[1] == 200);
}

int main() {
  // vector apis
  test__null_vector();
  test__vector_init();
  test__vector_add();
  test__vector_setsize();
  test__vector_setcap();
  test__vector_loop();
  test__vector_truncate();

  // iterator apis
  test__iterator_new();
  test__null_iterator();
  test__iteration();

  // with structs
  test__vector_with_struct();

  // test with list of string
  test__vector_with_list_string();
}
