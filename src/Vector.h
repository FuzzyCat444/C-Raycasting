#ifndef VECTOR_H
#define VECTOR_H

// Vector structure with no operators/functions, just to group x and y coords.
// Function calls have overhead when used in a tight loop, so we inline vector
// operations for a speed boost.
typedef struct Vector {
    double x;
    double y;
} Vector;

#endif