#include "TargetFunction.h"

double TargetFunction::calc(const IDirectSolver& directSolver, const Model& model, const InversionInfo& info, const double* etalon, const size_t size, const DirectSolverErrorCalculator& err_calc) {
		std::vector <double> record;
		directSolver.DirectProblem(model, info, record.data());
		double res = 0;
		size_t size_signal = directSolver.SignalSize();
		size_t npoints = size / size_signal;
		for (int i = 0; i < npoints; ++i) {
			for (int j = 0; j < size_signal; ++j) {
				const auto cur_etalon = etalon[i * size_signal + j];
				const auto cur_value = record[i * size_signal + j];
				if (!std::isnan(cur_etalon)) {
					double cur_res = err_calc.Calc(cur_value, cur_etalon, j);
					res += cur_res;
				}
			}
		}
		return err_calc.End(res / npoints);
	}

	double TargetFunction::calc(const IDirectSolver& directSolver, Model& model, const InversionInfo& info, const std::vector <double>& etalon, const DirectSolverErrorCalculator& err_calc) {
		return calc(directSolver, model, info, etalon.data(), etalon.size(), err_calc);
	}