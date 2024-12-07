#pragma once
/**
 * Определяет, как считать ошибку в целевой функции.
 * Необходимо создать наследника и определить Calc и Avg, чтобы создать DirectSolver
 */
class DirectSolverErrorCalculator {
public:
	virtual ~DirectSolverErrorCalculator() = default;

	/**
	 * Метод для подсчёта ошибки для чиселок одного зонда
	 * \param[in] teor значение, полученное из палетки
	 * \param[in] etalon значение, с которым сравниваем
	 * \param[in] index_in_palette индекс значения teor в одной записи палетке
	 */
	virtual double Calc(double teor, double etalon, int index_in_palette) const = 0;

	/**
	 * Определяет, что делать с суммой Calc (осреднить, взять корень и т.д.)
	 * \param[in] res полученное после суммирования результатов Calc по всем зондам
	 */
	virtual double End(double res) const = 0;
};
