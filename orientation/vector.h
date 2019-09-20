
#pragma once

typedef struct {
    double x;
    double y;
    double z;
} vector;

typedef struct {
    double aa;
    double ab;
    double ac;
    double ba;
    double bb;
    double bc;
    double ca;
    double cb;
    double cc;
} matrix;

void vector_copy(const vector* a, vector* dest);
void vector_cross(const vector* a, const vector* b, vector* dest);
void vector_normalize(vector* dest);
void vector_diff(const vector* a, const vector* b, vector* dest);

void matrix_identity(matrix* dest);
void matrix_fill_rows(const vector* row1, const vector* row2, const vector* row3, matrix* dest);
void matrix_multiply(const matrix* a, const vector* b, vector* dest);
