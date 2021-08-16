#include "src/cvector.h"
#include "src/zero.h"

#include <assert.h>
#include <math.h>

void test__vector_init() {
  Vector(int) vector_int_t;

  vector_int_t vector_int;
  vector__init(&vector_int);

  assert(vector__size(&vector_int) == 0);
  assert(vector__cap(&vector_int) == 0);
  assert(vector__wrapped_buffer(&vector_int) == NULL);

  vector__free(&vector_int);
}

void test__null_vector() {

  Vector(int) vector_int_t;
  vector_int_t vector_int;

  assert(vector_int.initialized == false);

  vector__init(&vector_int);

  assert(vector_int.initialized == true);
  assert(vector__size(&vector_int) == 0);
  assert(vector__cap(&vector_int) == 0);

  vector__free(&vector_int);
}

void test__vector_add() {
  Vector(char) vector_char_t;
  vector_char_t vector_char;
  vector__init(&vector_char);

  vector__add(&vector_char, 'a');
  vector__add(&vector_char, 'b');
  vector__add(&vector_char, 'c');
  vector__add(&vector_char, 0); // null termination

  assert(vector__size(&vector_char) == 4);
  assert(vector__cap(&vector_char) == 4);

  assert(vector__index_cpy(&vector_char, 0) == 'a');

  char *string = vector__wrapped_buffer(&vector_char);
  assert(strcmp(string, "abc") == 0);

  vector__free(&vector_char);
}

void test__vector_setsize() {

  {
    Vector(int) vector_int_t;
    vector_int_t vector_int;
    vector__init(&vector_int);

    assert(vector__size(&vector_int) == 0);
    assert(vector__setsize(&vector_int, 10) == -1);
    // size won't be set
    assert(vector__size(&vector_int) == 0);

    vector__free(&vector_int);
  }

  {
    Vector(int) vector_int_t;
    vector_int_t vector_int;
    vector__init(&vector_int);

    assert(vector__setsize(&vector_int, 10) == -1);
    assert(vector__size(&vector_int) == 0);
  }
}
void test__vector_init_with_cap() {
  Vector(int) vector_int_t;
  vector_int_t vector_int;
  // Initialize with fixed initial cap
  vector__init_with_cap(&vector_int, 10);

  assert(vector__size(&vector_int) == 0);
  assert(vector__cap(&vector_int) == 10);

  vector__free(&vector_int);
}

void test__vector_setcap() {
  Vector(int) vector_int_t;
  vector_int_t vector_int;

  vector__init(&vector_int);

  assert(vector__size(&vector_int) == 0);
  assert(vector__cap(&vector_int) == 0);

  // can't set cap if wrapped buffer is NULL
  assert(vector__setcap(&vector_int, 10) == -1);
  assert(vector__cap(&vector_int) == 0);

  // can be set if wrapped buffer is not NULL
  vector__add(&vector_int, 10);

  assert(vector__size(&vector_int) == 1);
  assert(vector__cap(&vector_int) == 1);

  assert(vector__setcap(&vector_int, 4));
  assert(vector__cap(&vector_int) == 4);

  vector__free(&vector_int);
}

void test__vector_loop() {
  {
    Vector(int) vector_int_t;
    // declare
    vector_int_t vector_int;
    // constructor
    vector__init(&vector_int);

    for (int i = 0; i < 10000; i++) {
      vector__add(&vector_int, i * i);
    }

    for (int i = 0; i < 10000; i++) {
      int *value = vector__index(&vector_int, i);
      assert(*value == i * i);
    }

    vector__free(&vector_int);
  }

  {
    typedef struct node_t {
      int x;
      int y;
    } node_t;

    Vector(node_t) vector_node_t;

    vector_node_t vector_node;

    // construction
    vector__init(&vector_node);

    for (int i = 0; i < 100000; i++) {
      vector__add(&vector_node, ((node_t){.x = i * i, .y = i * i * i}));
    }

    for (int i = 0; i < 100000; i++) {
      node_t node = vector__index_cpy(&vector_node, i);
      assert(node.x == i * i);
      assert(node.y == i * i * i);
    }

    vector__free(&vector_node);
  }
}

void test__vector_pop() {
  {
    Vector(int) vector_int_t;
    vector_int_t vector_int;
    vector__init(&vector_int);

    for (int i = 0; i < 100; i++) {
      vector__add(&vector_int, i);
    }

    for (int i = 0; i < 100; i++) {
      int value = vector__pop_cpy(&vector_int);
      assert(vector__size(&vector_int) == (100 - i - 1));
      assert(value == (100 - i - 1));
    }

    vector__free(&vector_int);
  }

  {
    Vector(int) vector_int_t;
    vector_int_t vector_int;
    vector__init(&vector_int);

    for (int i = 0; i < 100; i++) {
      vector__add(&vector_int, i);
    }

    for (int i = 0; i < 100; i++) {
      int *value = vector__pop(&vector_int);
      assert(vector__size(&vector_int) == (100 - i - 1));
      assert(*value == (100 - i - 1));
    }

    vector__free(&vector_int);
  }
}

void test__iterator_new() {
  Vector(int) vector_int_t;
  Vector_iterator(vector_int_t) iterator_int_t;

  vector_int_t vector_int;
  iterator_int_t iterator_int;

  // construction
  vector__init(&vector_int);
  vector_iterator__init(&iterator_int, &vector_int);

  // assertions
  assert(vector_iterator__current_index(&iterator_int) == 0);
  assert(vector_iterator__wrapped_iterable(&iterator_int) == &vector_int);
}

void test__iterator_null() {

  Vector(int) vector_int_t;
  Vector_iterator(vector_int_t) iterator_int_t;

  vector_int_t vector_int;
  iterator_int_t iterator_int;

  vector_iterator__init(&iterator_int, NULL);

  assert(vector_iterator__wrapped_iterable(&iterator_int) == NULL);
  assert(vector_iterator__done(&iterator_int) == true);
}

void test__iteration() {
  Vector(int) vector_int_t;
  Vector_iterator(vector_int_t) iterator_int_t;

  vector_int_t vector_int;
  iterator_int_t iterator_int;

  vector__init(&vector_int);
  vector_iterator__init(&iterator_int, &vector_int);

  for (int i = 0; i < 1000; i++) {
    vector__add(&vector_int, i * i);
  }

  assert(vector_iterator__done(&iterator_int) == false);

  int i = 0;

  for (;;) {

    if (vector_iterator__done(&iterator_int)) {
      break;
    }

    int value = vector_iterator__next_cpy(&iterator_int);

    assert(value == i * i);
    i++;
  }
}

void test__vector_with_struct() {
  typedef struct Node_t {
    int value;
    struct Node_t *next_node;
    bool done;
  } Node_t;

  Vector(Node_t) vector_node_t;
  Vector_iterator(vector_node_t) iterator_node_t;

  vector_node_t vector_node;
  iterator_node_t iterator_node;

  vector__init(&vector_node);
  vector_iterator__init(&iterator_node, &vector_node);

  Node_t *current = NULL;

  for (int i = 0; i < 100; i++) {
    Node_t node = ((Node_t){.value = i * i, .next_node = NULL, .done = true});
    vector__add(&vector_node, node);
  }

  int i = 0;
  for (;;) {
    if (vector_iterator__done(&iterator_node)) {
      break;
    }

    Node_t *node = vector_iterator__next(&iterator_node);
    assert(node->done == true);
    assert(node->value == i * i);
    i++;
  }
}

void test__vector_with_list_string() {
  Vector(char *) vector_string_t;
  Vector_iterator(vector_string_t) iterator_string_t;

  vector_string_t vector_string;
  iterator_string_t iterator_string;

  vector__init(&vector_string);
  vector_iterator__init(&iterator_string, &vector_string);

  char *names[3] = {"robus", "james", "jackson"};

  for (int i = 0; i < 3; i++) {
    char *name = names[i];
    vector__add(&vector_string, name);
  }

  {
    char *name = vector_iterator__next_cpy(&iterator_string);
    assert(strcmp(name, "robus") == 0);
  }

  {
    char *name = vector_iterator__next_cpy(&iterator_string);
    assert(strcmp(name, "james") == 0);
  }

  {
    char *name = vector_iterator__next_cpy(&iterator_string);
    assert(strcmp(name, "jackson") == 0);
  }
}

void test__nested_vector() {
  Vector(char) vector_char_t;
  Vector_iterator(vector_char_t) iterator_char_t;

  vector_char_t vector_char;
  iterator_char_t iterator_char;

  vector__init(&vector_char);
  vector_iterator__init(&iterator_char, &vector_char);

  vector__add(&vector_char, 'a');
  vector__add(&vector_char, 'b');
  vector__add(&vector_char, 'c');
  vector__add(&vector_char, 0);

  Vector(vector_char_t) vector_string_t;
  Vector_iterator(vector_string_t) iterator_string_t;

  vector_string_t vector_string;
  iterator_string_t iterator_string;

  vector__init(&vector_string);
  vector_iterator__init(&iterator_string, &vector_string);

  vector__add(&vector_string, vector_char);

  vector_char_t *value = vector_iterator__next(&iterator_string);

  assert(strcmp(vector__wrapped_buffer(value), "abc") == 0);
}

void test__zero_add() {

  Zero(int) zero_int_t; // declare type
  zero_int_t zero_int;
  zero__init(&zero_int);

  for (int i = 0; i < 100000; i++) {
    zero__add(&zero_int, i);
  }

  for (int i = 0; i < zero__size(&zero_int); i++) {
    int *value = zero__index(&zero_int, i);
    assert(*value == i);
  }

  zero__free(&zero_int);
}

void test__zero_index() {
  Zero(int) zero_int_t;
  zero_int_t zero_int;
  zero__init(&zero_int);

  for (int i = 0; i < 100000; i++) {
    zero__add(&zero_int, i * i);
  }

  for (int i = 0; i < zero__size(&zero_int); i++) {
    assert(zero__index_cpy(&zero_int, i) == i * i);
  }

  zero__free(&zero_int);
}

void test__zero_with_struct() {
  typedef struct Node_t {
    int x;
    int y;
  } Node_t;

  Zero(Node_t) zero_node_t;
  zero_node_t zero_node;
  zero__init(&zero_node);

  for (int i = 0; i < 1000; i++) {
    zero__add(&zero_node, ((Node_t){.x = i, .y = i}));
  }

  for (int i = 0; i < 1000; i++) {
    Node_t *node = zero__index(&zero_node, i);
    assert(node->x == i && node->y == i);
  }

  zero__free(&zero_node);
}

void test__zero_init() {
  Zero(int) zero_int_t;
  zero_int_t zero_int;
  zero__init(&zero_int);

  assert(zero__size(&zero_int) == 0);
}

void test__zero_with_first_last() {
  Zero(int) zero_int_t;
  zero_int_t zero_int;
  zero__init(&zero_int);

  for (int i = 0; i < 1000; i++) {
    zero__add(&zero_int, i);
  }

  assert(*(zero__first(&zero_int)) == 0);
  assert(zero__first_cpy(&zero_int) == 0);

  assert(*(zero__last(&zero_int)) == 999);
  assert(zero__last_cpy(&zero_int) == 999);
}

void test__zero_iteration() {
  Zero(int) zero_int_t;
  zero_int_t zero_int;
  zero__init(&zero_int);

  for (int i = 0; i < 30; i++) {
    zero__add(&zero_int, i);
  }

  Zero_iterator(zero_int_t) int_iterator_t;
  int_iterator_t int_iterator;

  zero_iterator__init(&int_iterator, &zero_int);
  for (int i = 0; i < 30; i++) {
    int *value = zero__index(&zero_int, i);
  }

  int v = 0;
  for (;;) {
    if (zero_iterator__done(&int_iterator)) {
      break;
    }

    int *value = zero_iterator__next(&int_iterator);
    assert(*value == v++);
  }

  zero_iterator__reset(&int_iterator);
  v = 0;

  for (;;) {
    if (zero_iterator__done(&int_iterator)) {
      break;
    }

    int value = zero_iterator__next_cpy(&int_iterator);
    assert(value == v++);
  }
}

void test__zero_peek_first_last() {
  Zero(int) zero_int_t;
  zero_int_t zero_int;
  zero__init(&zero_int);

  for (int i = 0; i < 30; i++) {
    zero__add(&zero_int, i);
  }

  Zero_iterator(zero_int_t) int_iterator_t;
  int_iterator_t int_iterator;

  zero_iterator__init(&int_iterator, &zero_int);

  assert(*zero_iterator__peek(&int_iterator) == 0);

  zero_iterator__next(&int_iterator);

  assert(zero_iterator__peek_cpy(&int_iterator) == 1);
}

// Define zero type of int
Vector(int) vector_int_t;

// Define iterator type for iterator_int_t
Vector_iterator(vector_int_t) iterator_int_t;

int main() {
  // vector apis
  test__null_vector();
  test__vector_init();
  test__vector_init_with_cap();
  test__vector_add();
  test__vector_setsize();
  test__vector_setcap();
  test__vector_loop();
  test__vector_pop();

  // iterator apis
  test__iterator_new();
  test__iterator_null();
  test__iteration();

  // with structs
  test__vector_with_struct();

  // test with list of string
  test__vector_with_list_string();
  test__nested_vector();

  // zero apis
  test__zero_init();
  test__zero_add();
  test__zero_index();
  test__zero_with_struct();
  test__zero_with_first_last();

  // zero iteration
  test__zero_iteration();
  test__zero_peek_first_last();
}
