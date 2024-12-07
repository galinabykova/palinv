#pragma once
#include "IDirectSolver.h"
#include "InversionInfo.h"
#include <map>

namespace InverseSolver {
	/**
	 * моделирует сигналы по модели
	 * \param[in] info дополнительная информация
	 * \param[in] nprobes количество зондов
	 * \param[in] num_probe зонды
	 * \param[in] point_number количество точек измерения
	 * \param[in] model модель
	 * \param[in, out] record место для сохранения данных в порядке палетки. Выделяется вне функции для оптимизации выделений памяти
	 * \param[in] direct_solver прямая задача
	 * \param[out] synt_signals сигналы в порядке пользователя
	 */
	void SignalsByModel(
		const InversionInfo& info,
		const uint16_t nprobes, const void* num_probe, const size_t point_number,
		const Model& model, double record[],
		const IDirectSolver& direct_solver,
		double synt_signals[]
	);

	/**
	 * обратная задача
	 * \param[in] info дополнительная информация
	 * \param[in] nprobes количество зондов
	 * \param[in] num_probe зонды
	 * \param[in] point_number количество точек измерения
	 * \param[in] etalon_signals экспериментальные сигналы
	 * \param[out] model найденная модель
	 * \param[in] direct_solver прямая задача
	 * \param[in] err_calc целевая функция
	 * \param[out] synt_signals сигналы модели
	 * \param[out] misfit невязка
	 */	
	void Inversion(
		const InversionInfo& info,
		const uint16_t nprobes, const void* num_probe, const size_t point_number,
		const double etalon_signals[],
		LimitedModel& model,
		const IDirectSolver& direct_solver,
		const DirectSolverErrorCalculator& err_calc,
		double synt_signals[], double& misfit
	);
};


