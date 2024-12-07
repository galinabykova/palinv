#pragma once
#include "Model.h"
#include <memory>

struct LimitedModel final {
	std::unique_ptr <Model> model;
	std::unique_ptr <Model> min_model;
	std::unique_ptr <Model> max_model;

	/**
	 * Конструктор.
	 * \param model модель
	 * \param min_model минимальные значения параметров модели
	 * \param max_model максимальные значения параметров модели
	 */
	LimitedModel(
		std::unique_ptr <Model> model,
		std::unique_ptr <Model> min_model,
		std::unique_ptr <Model> max_model
	);

	/**
	 * Обновляет модель по массиву в порядке для инверсии. Обратно ToDoubleInversion
	 * \param[in] param массив
	 * \param[in] info дополнительная информация
	 */
	void UpdateInversion(const double param[], const InversionInfo& info);

	/**
	 * Сохраняет в массив параметры в порядке для инверсии. Обратно UpdateInversion
	 * \param[out] param массив
	 * \param[in] info дополнительная информация
	 */
	void ToDoubleInversion(double param[], const InversionInfo& info) const;

	/**
	 * Обновляет модель по массиву в порядке для прямой задачи. Обратно ToDoubleDirect
	 * \param[in] param массив
	 * \param[in] info дополнительная информация
	 */
	void UpdateDirect(const double param[], const InversionInfo& info);

	/**
	 * Сохраняет в массив параметры в порядке для прямой задачи. Обратно UpdateDirect
	 * \param[out] param массив
	 * \param[in] info дополнительная информация
	 */
	void ToDoubleDirect(double param[], const InversionInfo& info) const;

	/**
	 * Сохраняет минимальные значения параметров в массив
	 * \param[in] param массив
	 */
	void ToMinDouble(double param[]) const;

	/**
	 * Сохраняет максимальные значения параметров в массив
	 * \param[in] param массив
	 */
	void ToMaxDouble(double param[]) const;

	/**
	 * Проверяет, находится ли модель в допустимых границах
	 */
	bool IsInBound() const;

	/**
	 * Ограничивает модель
	 */
	void LimitByBound();

	/**
	 * Фиксирует один из параметров
	 */
	void ToFixArray(int fix[]) const;
};