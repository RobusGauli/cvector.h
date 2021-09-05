# cvector(1)
[code quality](https://www.code-inspector.com/project/27388/score/svg)

  Generic & Typesafe Vector implementation C in ~150 LOC.


## Installation

  It has a single header file file called `cvector.h`. You can simply copy/paste in your source directory to get started. You can also install via clib (package manager for C).

  With [clib](https://github.com/clibs/clib):
  ```sh
  clib install robusgauli/cvector
  ```


## About

Vector/List is fundamental data structure for just about anything. Almost all programming languages as a builtin or packaged into standard library except for C. Also, it includes `Iterator` for free which allows you to do pretty interesting stuff such as peek, next, done, etc.

Go to [cvector.h](https://github.com/RobusGauli/cvector/blob/master/src/cvector.h) for complete list of APIs.

## Usage

### Simple example
```c
#include "cvector.h"

int main() {
  // Define type that holds vector of int
  CVector(int) vector_int_t;

  // Declare variable of type `vector_int_t`
  vector_int_t vector_int;

  // Initialize
  cvector__init(&vector_int);

  // Push number to vector
  for (int i = 0; i < 100; i++) {
    cvector__add(&vector_int, i);
  }

  // Size of the vector
  size_t size = cvector__size(&vector_int);

  // Iterate over elements
  for (int i = 0; i < 100; i++) {
    int* number = cvector__index(&vector_int, i);
    printf("Number is: %d\n", *number);
  }
}
```


### With Iterator

```c
#include "cvector.h"

// Define zero type of int
CVector(int) vector_int_t;


// Define iterator type for iterator_int_t
CVector_iterator(vector_int_t) iterator_int_t;

int main() {

  // Declare variable of type `zero_int_t`
  vector_int_t vector_int;

  // Initialize
  cvector__init(&vector_int);

  // Push elements to vector
  for (int i = 0; i < 10; i++) {
    cvector__add(&vector_int, i*i);
  }

  // Declare varaible of type `iterator_int_t`
  iterator_int_t iterator_int;

  // Initialize iterator with vector of int
  cvector_iterator__init(&iterator_int, &vector_int);

  // Peek the current element
  int* number = cvector_iterator__peek(&iterator_int);
  printf("The peeked number is: %d\n", *number);

  // Iterate using iterator
  for (;;) {
    if (cvector_iterator__done(&iterator_int)) {
      break;
    }

    int* number = cvector_iterator__next(&iterator_int);
    printf("Got number: %d\n", *number);
  }
}
```

### Using struct
```c
#include "cvector.h"

typedef struct Node_t {
  int x;
  int y;
} Node_t;

int main() {
  // Define vector type of `Node_t`
  CVector(Node_t) vector_node_t;

  // Declare variable of type `vector_node_t`
  vector_node_t vector_node;

  // Initialize vector
  cvector__init(&vector_node);

  // Push to vector
  for (int i = 0; i < 100; i++) {
    Node_t node = ((Node_t){.x = i, .y = i*i});
    cvector__add(&vector_node, node);
  }

  // Define iterator of type `vector_node_t`
  CVector_iterator(vector_node_t) iterator_node_t;

  // Declare variable of type `iterator_node_t`
  iterator_node_t iterator_node;

  // Initialize iterator
  cvector_iterator__init(&iterator_node, &vector_node);

  // Iterate
  for(;;) {
    if (cvector_iterator__done(&iterator_node)) {
      break;
    }

    Node_t* node = cvector_iterator__next(&iterator_node);
    printf("Node: x -> %d & y -> %d\n", node -> x, node -> y);
  }
}
```

### More APIs (cvector.h)

```c
#include <cvector.h>

int main() {
  CVector(int) vector_int_t;
  vector_int_t vector_int;

  cvector__init(&vector_int);

  cvector__add(&vector_int, 12);
  cvector__add(&vector_int, 13);
  cvector__add(&vector_int, 14);
  cvector__add(&vector_int, 15);

  // First
  {
    // Get the first element
    int* number = cvector__first(&vector_int);
    printf("First element: %d\n", *number);
  }

  // Last
  {
    // Get the last element by copy
    int number = cvector__last_cpy(&vector_int);
    printf("Last element: %d\n", number);
  }

  // Pop
  {
    size_t size_before = cvector__size(&vector_int);

    printf("Size of vector before pop: %ld\n", size_before);

    // Pop element from the vector
    int* number = cvector__pop(&vector_int);

    printf("Popped element: %d\n", *number);

    size_t size_after = cvector__size(&vector_int);
    printf("Size of vector after pop: %ld\n", size_after);
  }

  // Set value at index
  {
    if(cvector__set_at_index(&vector_int, 0, 56) == -1) {
      fprintf(stderr, "Failed to set the value '56' at index '0'");
    }
  }
}
```

### License

Copyright © 2020-20121 Robus, LLC. This source code is licensed under the MIT license found in
the [LICENSE.txt]
The documentation to the project is licensed under the [CC BY-SA 4.0](http://creativecommons.org/licenses/by-sa/4.0/)
license.


---
Made with ♥ by Robus Gauli ([@robusgauli]
