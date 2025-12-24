#ifndef LIB_H
#define LIB_H

#include <stdbool.h>
#include <stdio.h>

// Функция для вывода матрицы
void change_3_bits(unsigned char *ptr, int substitute);

// Функция для переворота массива
void swap_array(int n, int array[n]);

// Функция которая заполняет верхний треугольник матрицы 0, а нижний вместе с
// центральной диагональю 1
void change_array(int n, int m, int array[n][m]);

// Функция для инициализации матрицы
void initial_array(int n, int array[n][n], int array_res[n][n], int num);

// Функция для проверки в какуб сторону идем дальше
int check(int direction);

// Функция для заполнения массива улиткой
void snail_array(int n, int array[n][n], int array_res[n][n]);

#endif