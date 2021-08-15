#include "src/cvector.h"
#include "src/zero.h"

#include <assert.h>


void test__vector_init() {
  typedef Vector(int) vector_int_t;

  vector_int_t vector_int;
  vector__init(&vector_int);

  assert(vector__size(&vector_int) == 0);
  assert(vector__cap(&vector_int) == 0);
  assert(vector__wrapped_buffer(&vector_int) == NULL);

  vector__free(&vector_int);
}

void test__null_vector() {

  typedef Vector(int) vector_int_t;
  vector_int_t vector_int;

  assert(vector_int.initialized == false);

  vector__init(&vector_int);

  assert(vector_int.initialized == true);
  assert(vector__size(&vector_int) == 0);
  assert(vector__cap(&vector_int) == 0);

  vector__free(&vector_int);
}

void test__vector_add() {
  typedef Vector(char) vector_char_t;
  vector_char_t vector_char;
  vector__init(&vector_char);

  vector__add(&vector_char, 'a');
  vector__add(&vector_char, 'b');
  vector__add(&vector_char, 'c');
  vector__add(&vector_char, 0); // null termination

  printf("the size is: %ld\n", vector__size(&vector_char));
  assert(vector__size(&vector_char) == 4);
  assert(vector__cap(&vector_char) == 4);

  assert(vector__index_cpy(&vector_char, 0) == 'a');

  char* string = vector__wrapped_buffer(&vector_char);
  assert(strcmp(string, "abc") == 0);

  vector__free(&vector_char);
}


void test__vector_setsize() {

  {
    typedef Vector(int) vector_int_t;
    vector_int_t vector_int;
    vector__init(&vector_int);

    assert(vector__size(&vector_int) == 0);
    assert(vector__setsize(&vector_int, 10) == -1);
    // size won't be set
    assert(vector__size(&vector_int) == 0);

    vector__free(&vector_int);
  }

  {
    typedef Vector(int) vector_int_t;
    vector_int_t vector_int;
    vector__init(&vector_int);

    assert(vector__setsize(&vector_int, 10) == -1);
    assert(vector__size(&vector_int) == 0);
  }

}
void test__vector_init_with_cap() {
  typedef Vector(int) vector_int_t;
  vector_int_t vector_int;
  // Initialize with fixed initial cap
  vector__init_with_cap(&vector_int, 10);

  assert(vector__size(&vector_int) == 0);
  assert(vector__cap(&vector_int) == 10);

  vector__free(&vector_int);
}

void test__vector_setcap() {
  typedef Vector(int) vector_int_t;
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
    typedef Vector(int) vector_int_t;
    // declare
    vector_int_t vector_int;
    // constructor
    vector__init(&vector_int);

    for (int i = 0; i < 10000; i++) {
      vector__add(&vector_int, i*i);
    }

    for (int i = 0; i < 10000; i++) {
      int* value = vector__index(&vector_int, i);
      assert(*value == i*i);
    }

    vector__free(&vector_int);
  }

  {
    typedef struct node_t {
      int x;
      int y;
    } node_t;

    typedef Vector(node_t) vector_node_t;

    vector_node_t vector_node;

    // construction
    vector__init(&vector_node);

    for (int i = 0; i < 100000; i++) {
      vector__add(&vector_node, ((node_t){.x=i*i, .y=i*i*i}));
    }

    for (int i = 0; i < 100000; i++) {
      node_t node = vector__index_cpy(&vector_node, i);
      assert(node.x == i*i);
      assert(node.y == i*i*i);
    }

    vector__free(&vector_node);
  }
}

void test__vector_pop() {
  {
    typedef Vector(int) vector_int_t;
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
    typedef Vector(int) vector_int_t;
    vector_int_t vector_int;
    vector__init(&vector_int);

    for (int i = 0; i < 100; i++) {
      vector__add(&vector_int, i);
    }

    for (int i = 0; i < 100; i++) {
      int* value = vector__pop(&vector_int);
      assert(vector__size(&vector_int) == (100 - i - 1));
      assert(*value == (100 - i - 1));
    }

    vector__free(&vector_int);
  }

}

void test__iterator_new() {
  typedef Vector(int) vector_int_t;
  typedef Vector_iterator(vector_int_t) iterator_int_t;

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

  typedef Vector(int) vector_int_t;
  typedef Vector_iterator(vector_int_t) iterator_int_t;

  vector_int_t vector_int;
  iterator_int_t iterator_int;

  vector_iterator__init(&iterator_int, NULL);

  assert(vector_iterator__wrapped_iterable(&iterator_int) == NULL);
  assert(vector_iterator__done(&iterator_int) == true);
}

void test__iteration() {
  typedef Vector(int) vector_int_t;
  typedef Vector_iterator(vector_int_t) iterator_int_t;

  vector_int_t vector_int;
  iterator_int_t iterator_int;

  vector__init(&vector_int);
  vector_iterator__init(&iterator_int, &vector_int);

  for (int i = 0; i < 1000; i++) {
    vector__add(&vector_int, i*i);
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

  /*// with structs*/
  /*test__vector_with_struct();*/

  /*// test with list of string*/
  /*test__vector_with_list_string();*/
  /*test__vector_with_list_string();*/


  /*// test large values*/
  /*test__vector_with_large_buffer();*/

}
