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
  // initialize with fixed initial cap
  vector__init_with_cap(&vector_int, 10);

  assert(vector__size(&vector_int) == 0);
  assert(vector__cap(&vector_int) == 10);

  vector__free(&vector_int);
}

int main() {
  // vector apis
  test__null_vector();
  test__vector_init();
  test__vector_init_with_cap();
  test__vector_add();
  test__vector_setsize();
  /*test__vector_setcap();*/
  /*test__vector_loop();*/
  /*test__vector_truncate();*/

  /*// iterator apis*/
  /*test__iterator_new();*/
  /*test__null_iterator();*/
  /*test__iteration();*/

  /*// with structs*/
  /*test__vector_with_struct();*/

  /*// test with list of string*/
  /*test__vector_with_list_string();*/
  /*test__vector_with_list_string();*/


  /*// test large values*/
  /*test__vector_with_large_buffer();*/

}
