#include "InverseSolver.h"
#include "LimitedModel.h"
#include "IDirectSolver.h"
#include "TargetFunction.h"
#include "min/argmin.h"
#include <set>
#include <algorithm>

namespace {
	/**
	 * Сохраняет данные в порядке, удобным для прямой задачи
	 * \param[in] info дополнительная информация
	 * \param[in] nprobes количество зондов 
	 * \param[in] num_probe зонды
	 * \param[in] point_number количество точек измерения
	 * \param[in] signals сигналы в порядке пользователя
	 * \param[in] direct_solver прямая задача
	 * \param[out] out сигналы в порядке, удобным для прямой задачи
	 */
	void UserToDirectSolverRecord(
		const InversionInfo& info,
		const uint16_t nprobes, const void* num_probe, const size_t point_number,
		const double signals[],
		const IDirectSolver& direct_solver,
		double out[]
	) {
		size_t direct_solver_size = direct_solver.SignalSize();
		size_t out_size = point_number * direct_solver_size;
		for (size_t i = 0; i < out_size; ++i) {
			out[i] = NAN;
		}
		for (size_t k = 0; k < point_number; ++k) {
			for (size_t i = 0; i < nprobes; i++) {
				if (!isnan(signals[k * nprobes + i])) {
					int32_t index = direct_solver.PublicIndexToDirectSolverIndex(num_probe, i);
					out[index + k * direct_solver_size] = signals[i + k * nprobes];
				}
			}
		}
	}

	/**
	 * Сохраняет данные в порядке, удобным для пользователя
	 * \param[in] info дополнительная информация
	 * \param[in] nprobes количество зондов 
	 * \param[in] num_probe зонды
	 * \param[in] point_number количество точек измерения
	 * \param[in] record сигналы в порядке палетки
	 * \param[in] direct_solver прямая задача
	 * \param[out] out сигналы в порядке, удобным для пользователя
	 */
	void DirectSolverRecordToUser(const InversionInfo& info, const uint16_t nprobes, const void* num_probe, const size_t point_number,
		const double record[], const IDirectSolver& direct_solver, double out[]
	) {
		size_t signal_size = direct_solver.SignalSize();
		size_t out_size = point_number * nprobes;
		for (size_t i = 0; i < out_size; ++i) {
			out[i] = NAN;
		}
		for (size_t k = 0; k < point_number; ++k) {
			for (size_t i = 0; i < nprobes; i++) {
				int32_t index = direct_solver.PublicIndexToDirectSolverIndex(num_probe, i);
				if (isnan(record[index + k * signal_size])) {
					continue;;
				}
				out[i + k * nprobes] = record[index + k * signal_size];
			}
		}
	}

	struct inversion_solver_struct {
		LimitedModel& model;
		const InversionInfo& info;
		const std::vector <double>& etalon;
		const IDirectSolver& direct_solver;
		const DirectSolverErrorCalculator& err_calc;
	};

	double Inversion_solver_fn(const double x[], const void* pdata) {
		inversion_solver_struct* data = (inversion_solver_struct*)pdata;
		data->model.UpdateInversion(x, data->info);
		double res = TargetFunction::calc(data->direct_solver, *((data->model).model), data->info, data->etalon, data->err_calc);
		return res;
	}

	void InversionFunction(
		const InversionInfo& info,
		const std::vector<double>& etalon, const size_t point_number,
		LimitedModel& model,
		const IDirectSolver& direct_solver,
		const DirectSolverErrorCalculator& err_calc,
		double& misfit
	) {
		size_t count_x = model.model->Nparams(info);

		model.min_model->LimitFromBelow(direct_solver.MinModel());
		model.max_model->LimitFromAbove(direct_solver.MaxModel());

		std::vector <double> start(count_x);
		std::vector <double> step(count_x);
		std::vector <double> xmin(count_x);
		std::vector <double> min_b(count_x);
		std::vector <double> max_b(count_x);
		std::vector <int> fix(count_x);
		model.ToDoubleInversion(start.data(), info);

		for (size_t i = 0; i < count_x; ++i) {
			step[i] = 0.1;
		}

		for (size_t i = 0; i < count_x; ++i) {
			xmin[i] = 0;
		}

		double reqmin = 1e-12;		    
		int kcount = info.GetIterationNumber();								 

		model.ToMinDouble(min_b.data());
		model.ToMaxDouble(max_b.data());

		model.ToFixArray(fix.data());

		const inversion_solver_struct mes_struct = {
			model,
			info,
			etalon,
			direct_solver,
			err_calc
		};

		argmin(&Inversion_solver_fn, (int)count_x, start.data(), min_b.data(), max_b.data(),
			fix.data(), xmin.data(), &misfit, reqmin, step.data(), kcount, &mes_struct);
		model.UpdateInversion(xmin.data(), info);
		model.LimitByBound();
	}
}

void InverseSolver::SignalsByModel(
	const InversionInfo& info,
	const uint16_t nprobes, const void* num_probe, const size_t point_number,
	const Model& model, double record[],
	const IDirectSolver& direct_solver,
	double synt_signals[]
) {
	direct_solver.DirectProblem(model, info, record);
	DirectSolverRecordToUser(info, nprobes, num_probe, point_number, record, direct_solver, synt_signals);
}

void InverseSolver::Inversion(
	const InversionInfo& info,
	const uint16_t nprobes, const void* num_probe, const size_t point_number,
	const double etalon_signals[],
	LimitedModel& model,
	const IDirectSolver& direct_solver,
	const DirectSolverErrorCalculator& err_calc,
	double synt_signals[], double& misfit
) {
	static std::vector <double> record;
	size_t record_point_number_size = point_number * direct_solver.SignalSize();
	if (record.size() < record_point_number_size) {
		record.resize(record_point_number_size);
	}
	UserToDirectSolverRecord(info, nprobes, num_probe, point_number, etalon_signals, direct_solver, record.data());
	InversionFunction(info, record, point_number, model, direct_solver, err_calc, misfit);
	if (synt_signals != NULL) {
		SignalsByModel(info, nprobes, num_probe, point_number, *(model.model), record.data(), direct_solver, synt_signals);
	}
}