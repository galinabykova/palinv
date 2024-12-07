#pragma once
#include "IDirectSolver.h"

namespace TargetFunction {
    /**
     * рассчитывает целевую функцию
     * \param directSolver прямая задача
     * \param model модель, для которой рассчитывается прямая задача
     * \param info дополнительная информация
     * \param etalon экспериментальные данные
     * \param size количество измерений
     * \param err_calc способ расчёта целевой функции
     */
    double calc(const IDirectSolver& directSolver, const Model& model, const InversionInfo& info,
        const double* etalon, const size_t size, const DirectSolverErrorCalculator& err_calc);

    /**
     * рассчитывает целевую функцию
     * \param directSolver прямая задача
     * \param model модель, для которой рассчитывается прямая задача
     * \param info дополнительная информация
     * \param etalon экспериментальные данные
     * \param err_calc способ расчёта целевой функции
     */
	double calc(const IDirectSolver& directSolver, Model& model, const InversionInfo& info, 
        const std::vector <double>& etalon, const DirectSolverErrorCalculator& err_calc);
}