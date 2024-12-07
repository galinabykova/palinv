#pragma once
#include "InversionInfo.h"

#include <cstdio>

struct Model {
	virtual ~Model() = default;

	/**
	 * Возвращает количество параметров в этой модели
	 */
	virtual size_t Nparams(const InversionInfo& info) const = 0;

	/**
	 * Обновляет модель по массиву в порядке для инверсии. Обратно ToDoubleInversion
	 * \param[in] param массив
	 * \param[in] info дополнительная информация
	 */
	virtual void UpdateInversion(const double param[], const InversionInfo& info) = 0;

	/**
	 * Сохраняет в массив параметры в порядке для инверсии. Обратно UpdateInversion
	 * \param[out] param массив
	 * \param[in] info дополнительная информация
	 */
	virtual void ToDoubleInversion(double param[], const InversionInfo& info) const = 0;

	/**
	 * Обновляет модель по массиву в порядке для прямой задачи. Обратно ToDoubleDirect
	 * \param[in] param массив
	 * \param[in] info дополнительная информация
	 */
	virtual void UpdateDirect(const double param[], const InversionInfo& info) = 0;
	
	/**
	 * Сохраняет в массив параметры в порядке для прямой задачи. Обратно UpdateDirect
	 * \param[out] param массив
	 * \param[in] info дополнительная информация
	 */
	virtual void ToDoubleDirect(double param[], const InversionInfo& info) const = 0;

	/**
	 * Ограничивает снизу 
	 * \param[in] min минимальные значения параметров
	 */
	virtual void LimitFromBelow(const Model& min);

	/**
	 * Ограничивает сверху 
	 * \param[in] max максимальные значения параметров
	 */
	virtual void LimitFromAbove(const Model& max);
};
