#pragma once

#include <stdint.h>
#include <math.h>

template<size_t dimension, typename type_size_dim, size_t size_signal, typename type_signal>
void UniversalApproximationT(const  type_size_dim size_dim[],
	const type_signal params_dim[], const type_signal palette[],
	const type_signal point[], type_size_dim idx[], type_signal weight[], type_signal  signal[]) {
	/*...*/
}

template<typename type_signal>
using TargetFunctionForSelectionT = type_signal(*)(const type_signal signal[], const void* pdata, const type_signal param[]);

template<size_t dimension, typename type_size_dim, size_t size_signal, typename type_signal>
void SelectBestSolutionT(const  type_size_dim size_dim[], const type_signal params_dim[],
    const type_signal palette[], const TargetFunctionForSelectionT<type_signal> fn,
	const void* pdata, type_signal bestParam[]) {
	/*...*/
}