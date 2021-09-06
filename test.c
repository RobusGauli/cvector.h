#include "src/cvector.h"

#include <assert.h>
#include <math.h>

void test__vector_init() {
  CVector(int) vector_int_t;

  vector_int_t vector_int;
  cvector__init(&vector_int);

  assert(cvector__size(&vector_int) == 0);
  assert(cvector__cap_(&vector_int) == 0);
  assert(cvector__wrapped_buffer(&vector_int) == NULL);

  cvector__free(&vector_int);
}

void test__vector_add() {
  CVector(char) vector_char_t;
  vector_char_t vector_char;
  cvector__init(&vector_char);

  cvector__add(&vector_char, 'a');
  cvector__add(&vector_char, 'b');
  cvector__add(&vector_char, 'c');
  cvector__add(&vector_char, 0); // null termination

  assert(cvector__size(&vector_char) == 4);
  assert(cvector__cap_(&vector_char) == 4);

  assert(cvector__index_cpy(&vector_char, 0) == 'a');

  char *string = cvector__wrapped_buffer(&vector_char);
  assert(strcmp(string, "abc") == 0);

  cvector__free(&vector_char);
}

void test__vector_setsize() {

  {
    CVector(int) vector_int_t;
    vector_int_t vector_int;
    cvector__init(&vector_int);

    assert(cvector__size(&vector_int) == 0);

    cvector__setsize_(&vector_int, 2);

    assert(cvector__size(&vector_int) == 2);
    cvector__free(&vector_int);
  }

  {
    CVector(int) vector_int_t;
    vector_int_t vector_int;
    cvector__init(&vector_int);

    assert(cvector__size(&vector_int) == 0);
    cvector__free(&vector_int);
  }
}
void test__vector_init_with_cap() {
  CVector(int) vector_int_t;
  vector_int_t vector_int;
  // Initialize with fixed initial cap
  cvector__init_with_cap(&vector_int, 10);

  cvector__add(&vector_int, 12);
  cvector__add(&vector_int, 123);
  cvector__add(&vector_int, 123123);

  cvector__free(&vector_int);
}

void test__vector_setcap() {
  CVector(int) vector_int_t;
  vector_int_t vector_int;

  cvector__init(&vector_int);

  assert(cvector__size(&vector_int) == 0);
  assert(cvector__cap_(&vector_int) == 0);

  // can be set if wrapped buffer is not NULL
  cvector__add(&vector_int, 10);

  assert(cvector__size(&vector_int) == 1);
  assert(cvector__cap_(&vector_int) == 1);

  assert(cvector__setcap_(&vector_int, 4));
  assert(cvector__cap_(&vector_int) == 4);

  cvector__free(&vector_int);
}

void test__vector_loop() {
  {
    CVector(size_t) vector_int_t;
    // declare
    vector_int_t vector_int;
    // constructor
    cvector__init(&vector_int);

    for (int i = 0; i < 1000; i++) {
      cvector__add(&vector_int, i * i);
    }

    for (size_t i = 0; i < 1000; i++) {
      size_t *value = cvector__index(&vector_int, i);
      assert(*value == i * i);
    }

    cvector__free(&vector_int);
  }

  {
    typedef struct node_t {
      unsigned long x;
      unsigned long y;
    } node_t;

    CVector(node_t) vector_node_t;

    vector_node_t vector_node;

    // construction
    cvector__init(&vector_node);

    for (size_t i = 0; i < 1000; i++) {
      cvector__add(&vector_node, ((node_t){.x = i * i, .y = i * i * i}));
    }

    for (size_t i = 0; i < 1000; i++) {
      node_t node = cvector__index_cpy(&vector_node, i);
      assert(node.x == i * i);
      assert(node.y == i * i * i);
    }

    cvector__free(&vector_node);
  }
}

void test__vector_pop() {
  {
    CVector(int) vector_int_t;
    vector_int_t vector_int;
    cvector__init(&vector_int);

    for (int i = 0; i < 100; i++) {
      cvector__add(&vector_int, i);
    }

    for (int i = 0; i < 100; i++) {
      int value = cvector__pop_cpy(&vector_int);
      assert(cvector__size(&vector_int) == (100 - i - 1));
      assert(value == (100 - i - 1));
    }

    cvector__free(&vector_int);
  }

  {
    CVector(int) vector_int_t;
    vector_int_t vector_int;
    cvector__init(&vector_int);

    for (int i = 0; i < 500; i++) {
      cvector__add(&vector_int, i);
    }

    for (int i = 0; i < 500; i++) {
      int *value = cvector__pop(&vector_int);
    }

    cvector__free(&vector_int);
  }
}

void test__iterator_new() {
  CVector(int) vector_int_t;
  CVector_iterator(vector_int_t) iterator_int_t;

  vector_int_t vector_int;
  iterator_int_t iterator_int;

  cvector__init(&vector_int);
  cvector_iterator__init(&iterator_int, &vector_int);

  assert(cvector_iterator__current_index_(&iterator_int) == 0);
  assert(cvector_iterator__wrapped_iterable(&iterator_int) == &vector_int);

  cvector__free(&vector_int);
}

void test__iterator_null() {

  CVector(int) vector_int_t;
  CVector_iterator(vector_int_t) iterator_int_t;

  iterator_int_t iterator_int;

  cvector_iterator__init(&iterator_int, NULL);

  assert(cvector_iterator__done(&iterator_int) == true);
}

void test__iteration() {
  CVector(int) vector_int_t;
  CVector_iterator(vector_int_t) iterator_int_t;

  vector_int_t vector_int;
  iterator_int_t iterator_int;

  cvector__init(&vector_int);
  cvector_iterator__init(&iterator_int, &vector_int);

  for (int i = 0; i < 1000; i++) {
    cvector__add(&vector_int, i * i);
  }

  assert(cvector_iterator__done(&iterator_int) == false);

  int i = 0;

  for (;;) {

    if (cvector_iterator__done(&iterator_int)) {
      break;
    }

    int value = cvector_iterator__next_cpy(&iterator_int);

    assert(value == i * i);
    i++;
  }

  cvector__free(&vector_int);
}

void test__vector_with_struct() {
  typedef struct Node_t {
    int value;
    struct Node_t *next_node;
    bool done;
  } Node_t;

  CVector(Node_t) vector_node_t;
  CVector_iterator(vector_node_t) iterator_node_t;

  vector_node_t vector_node;
  iterator_node_t iterator_node;

  cvector__init(&vector_node);
  cvector_iterator__init(&iterator_node, &vector_node);

  Node_t *current = NULL;

  for (int i = 0; i < 100; i++) {
    Node_t node = ((Node_t){.value = i * i, .next_node = NULL, .done = true});
    cvector__add(&vector_node, node);
  }

  int i = 0;
  for (;;) {
    if (cvector_iterator__done(&iterator_node)) {
      break;
    }

    Node_t *node = cvector_iterator__next(&iterator_node);
    assert(node->done == true);
    assert(node->value == i * i);
    i++;
  }

  cvector__free(&vector_node);
}

void test__vector_with_list_string() {
  CVector(char *) vector_string_t;
  CVector_iterator(vector_string_t) iterator_string_t;

  vector_string_t vector_string;
  iterator_string_t iterator_string;

  cvector__init(&vector_string);
  cvector_iterator__init(&iterator_string, &vector_string);

  char *names[3] = {"robus", "james", "jackson"};

  for (int i = 0; i < 3; i++) {
    char *name = names[i];
    cvector__add(&vector_string, name);
  }

  {
    char *name = cvector_iterator__next_cpy(&iterator_string);
    assert(strcmp(name, "robus") == 0);
  }

  {
    char *name = cvector_iterator__next_cpy(&iterator_string);
    assert(strcmp(name, "james") == 0);
  }

  {
    char *name = cvector_iterator__next_cpy(&iterator_string);
    assert(strcmp(name, "jackson") == 0);
  }

  cvector__free(&vector_string);
}

void test__nested_vector() {
  CVector(char) vector_char_t;
  CVector_iterator(vector_char_t) iterator_char_t;

  vector_char_t vector_char;
  iterator_char_t iterator_char;

  cvector__init(&vector_char);
  cvector_iterator__init(&iterator_char, &vector_char);

  cvector__add(&vector_char, 'a');
  cvector__add(&vector_char, 'b');
  cvector__add(&vector_char, 'c');
  cvector__add(&vector_char, 0);

  CVector(vector_char_t) vector_string_t;
  CVector_iterator(vector_string_t) iterator_string_t;

  vector_string_t vector_string;
  iterator_string_t iterator_string;

  cvector__init(&vector_string);
  cvector_iterator__init(&iterator_string, &vector_string);

  cvector__add(&vector_string, vector_char);

  vector_char_t *value = cvector_iterator__next(&iterator_string);

  assert(strcmp(cvector__wrapped_buffer(value), "abc") == 0);

  cvector__free(&vector_char);
  cvector__free(&vector_string);
}

void test__vector_set_at_index() {
  CVector(int) vector_int_t;

  vector_int_t vector_int;

  cvector__init_with_cap(&vector_int, 10);

  cvector__add(&vector_int, 44);
  cvector__add(&vector_int, 45);

  assert(*cvector__index(&vector_int, 1) == 45);

  assert(cvector__set_at_index(&vector_int, 1, 100) == 0);
  assert(*cvector__index(&vector_int, 1) == 100);

  // attempt to set beyond bound
  assert(cvector__set_at_index(&vector_int, 12, 100) == -1);

  cvector__free(&vector_int);
}

void test__vector_free() {
  CVector(int) cvector_int_t;
  cvector_int_t cvector_int;

  cvector__init(&cvector_int);

  cvector__resize_(&cvector_int, 2);
  assert(cvector__wrapped_buffer(&cvector_int) != NULL);

  cvector__free(&cvector_int);
  assert(cvector__wrapped_buffer(&cvector_int) == NULL);
}


int main() {
  // vector apis
  test__vector_init();
  test__vector_free();
  test__vector_init_with_cap();
  test__vector_add();
  test__vector_setsize();
  test__vector_setcap();
  test__vector_loop();
  test__vector_pop();
  test__vector_set_at_index();

  // iterator apis
  test__iterator_new();
  test__iterator_null();
  test__iteration();

  // with structs
  test__vector_with_struct();

  // test with list of string
  test__vector_with_list_string();
  test__nested_vector();
}
