#pragma once

//! для использования алгоритма поиска минимума целевая функция должна иметь такую сигнатуру
typedef double(*min_fn)(const double x[], const void* pdata);

//! константы для фиксации параметров 
#define FIX_PARAM 1
#define UNFIX_PARAM 0
#define INCORRECT_FIX_IND 3
#define STEP_SIZE_NOT_EQUALS 4

//! алгоритм поиска минимума с границами и фиксацией параметров
void argmin(const min_fn fn, const int n, const double start[], const double min_value[],
    const double max_value[], const int fix[], double xmin[],
    double* ynewlo, double reqmin, const double step[], const int kcount,
    const void* pdata);