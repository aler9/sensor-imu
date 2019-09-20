
#include <math.h>

#include "vector.h"

void vector_copy(const vector* a, vector* dest) {
    dest->x = a->x;
    dest->y = a->y;
    dest->z = a->z;
}

void vector_cross(const vector* a, const vector* b, vector* dest) {
    dest->x = a->y*b->z - a->z*b->y;
    dest->y = a->z*b->x - a->x*b->z;
    dest->z = a->x*b->y - a->y*b->x;
}

void vector_normalize(vector* dest) {
    double norm = sqrt(dest->x*dest->x + dest->y*dest->y + dest->z*dest->z);
    dest->x /= norm;
    dest->y /= norm;
    dest->z /= norm;
}

void vector_diff(const vector* a, const vector* b, vector* dest) {
    dest->x = a->x - b->x;
    dest->y = a->y - b->y;
    dest->z = a->z - b->z;
}

void matrix_identity(matrix* dest) {
    dest->aa = 1;
    dest->bb = 1;
    dest->cc = 1;
}

void matrix_fill_rows(const vector* row1, const vector* row2, const vector* row3, matrix* dest) {
    dest->aa = row1->x;
    dest->ab = row1->y;
    dest->ac = row1->z;
    dest->ba = row2->x;
    dest->bb = row2->y;
    dest->bc = row2->z;
    dest->ca = row3->x;
    dest->cb = row3->y;
    dest->cc = row3->z;
}

void matrix_multiply(const matrix* a, const vector* b, vector* dest) {
    dest->x = a->aa*b->x + a->ab*b->y + a->ac*b->z;
    dest->y = a->ba*b->x + a->bb*b->y + a->bc*b->z;
    dest->z = a->ca*b->x + a->cb*b->y + a->cc*b->z;
}
