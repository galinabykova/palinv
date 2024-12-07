#pragma once
#include "IDirectSolver.h"
#include "TargetFunction.h"

#include <vector>

class IPaletteFunction : public IDirectSolver {
	typedef struct {
		const std::vector<double> exp_signals;
		LimitedModel& model;
		const InversionInfo& info;
		const IPaletteFunction& palette;
		const DirectSolverErrorCalculator& err_calc;
	}ParamForBestSolution;

	static double tf_for_start_model(const double signals[], const void* pdata, const double param[]);
	
protected:
	std::vector<size_t> size_dim;
	std::vector<double> params_dim;
	const double* palette_data;

	/**
	 * Конструктор
	 * \param[in] min_model содержит минимальные значения параметров модели
	 * \param[in] min_model содержит максимальные значения параметров модели
	 */
	IPaletteFunction(const Model& min_model, const Model& max_model);

	/**
	 * Конструктор.
	 * \param[in] size_dim размеры сетки
	 * \param[in] params_dim сетка
	 * \param[in] palette_data указатель на палетку
	 * \param[in] min_model содержит минимальные значения параметров модели
	 * \param[in] min_model содержит максимальные значения параметров модели
	 */
	IPaletteFunction(const std::vector<size_t>& size_dim, const std::vector<double>& params_dim, const double* palette_data,
		const Model& min_model, const Model& max_model);

	/**
	 * Служебная функция для сливания размеров сетки в один вектор
	*/
	static std::vector<double> Merge(const std::vector< std::vector<double>>& v);

	/**
	 * * Служебная функция для логарифмирования узлов сетки
	*/
	static std::vector<double> Log10(const std::vector<double>& v);

	/**
	 * * Интерполяция палетки
	 * \param[in] model модель
	 * \param[in] info дополнительная информация
	 * \param[out] out результат
	*/
	template <size_t N_PARAMS, size_t SIZE_SIGNALS>
	const void DirectProblemT(const Model& model, const InversionInfo& info, double out[]) const {
		auto& changed_model = const_cast<Model&>(model);
		changed_model.LimitFromBelow(min_model);
		changed_model.LimitFromAbove(max_model);
		double point[N_PARAMS];
		model.ToDoubleDirect(point, info);
		size_t idx[N_PARAMS];
		double weight[N_PARAMS];
		UniversalApproximationT<N_PARAMS, size_t, SIZE_SIGNALS, double>
			(size_dim.data(), params_dim.data(), palette_data, point, idx, weight, out);
	}

	/**
	 * Поиск наиболее подходящего в палетке
	 * \param[in] etalon экспериментальный сигнал
	 * \param[in, out] model найденная модель с ограничениями
	 * \param[in] info дополнительная информация
	 * \param[out] err_calc способ расчёта невязки 
	 */
	template <size_t N_PARAMS, size_t SIZE_SIGNALS>
	const void StartModelByDirectSolverT(const std::vector <double>& etalon, LimitedModel& model, const InversionInfo& info, const DirectSolverErrorCalculator& err_calc) const {
		const ParamForBestSolution exp_signal = {
			etalon,
			model,
			info,
			*this,
			err_calc
		};
		double res[N_PARAMS];
		SelectBestSolutionT<N_PARAMS, size_t, SIZE_SIGNALS, double>(size_dim.data(), params_dim.data(),
			palette_data, tf_for_start_model, &exp_signal, res);
		model.UpdateDirect(res, info);
		model.LimitByBound();
	}

public:
	/**
	 * Рассчитывает стартовую модель
	 * \param[in] etalon экспериментальный сигнал
	 * \param[in, out] model найденная модель с ограничениями
	 * \param[in] info дополнительная информация
	 * \param[out] err_calc способ расчёта невязки 
	 */
	virtual const void StartModelByDirectSolver(const std::vector <double>& etalon, LimitedModel& model, const InversionInfo& info, const DirectSolverErrorCalculator& err_calc) const = 0;

	~IPaletteFunction() override = default;
};


