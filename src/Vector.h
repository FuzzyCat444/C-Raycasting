#ifndef VECTOR_H
#define VECTOR_H

// Vector structure with no operators/functions, just to group x and y coords.
// Function calls have overhead when used in a tight loop, so we inline allocate
// vector operations.
typedef struct Vector {
    double x;
    double y;
} Vector;

#endif