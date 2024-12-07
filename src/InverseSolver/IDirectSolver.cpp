#include "IDirectSolver.h"
#include <vector>

IDirectSolver::IDirectSolver(const Model& min_model, const Model& max_model) : min_model(min_model), max_model(max_model)
{
}

const Model& IDirectSolver::MinModel() const {
	return min_model;
}

const Model& IDirectSolver::MaxModel() const {
	return max_model;
}
