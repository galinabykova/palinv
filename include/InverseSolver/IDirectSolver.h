#pragma once
#include "Model.h"
#include "LimitedModel.h"
#include "DirectSolverErrorCalculator.h"
#include "UniversalApproximationT.h"
#include <vector>
#include <array>
#include <list>
#include <memory>
#include <cmath>

/**
 * Интерфейс для решения прямой задачи
 */
class IDirectSolver {
protected:
	const Model& min_model;
	const Model& max_model;
public:
	/**
	 * Конструктор
	 * \param[in] min_model содержит минимальные значения параметров модели
	 * \param[in] min_model содержит максимальные значения параметров модели
	 */
	IDirectSolver(const Model& min_model, const Model& max_model);

	virtual ~IDirectSolver() = default;

	/**
	 * Возвращает количество зондов, с которыми может работать прямая задача
 	*/
	virtual uint16_t SignalSize() const = 0;

	/**
	 * Проверяет, поддерживается ли зонд прямой задачей
 	*/
	virtual bool IsSupportedProbe(uint32_t num_probe) const = 0;

	/**
	 * Возвращает порядковый номер зонда в оптимальном порядке для прямой задачи
	 * \param[in] num_probe указатель на массив зондов
	 * \param[in] i индекс в массиве, по которому расположен нужный зонд
 	*/
	virtual uint32_t PublicIndexToDirectSolverIndex(const void* num_probe, const size_t i) const = 0;

	/**
	 * Вычисляет сигналы
	 * \param[in] model модель, в которой рассчитываем сигналы
	 * \param[in] info дополнительная информация
	 * \param[out] out сигналы 
	 */
	virtual void DirectProblem(const Model& model, const InversionInfo& info, double out[]) const = 0;

	/**
	 * Возвращает ограничения снизу для прямой задачи
	 */
	virtual const Model& MinModel() const;

	/**
	 * Возвращает ограничения сверху для прямой задачи
	 */
	virtual const Model& MaxModel() const;
};
