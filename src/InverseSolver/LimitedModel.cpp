#include "Model.h"
#include "LimitedModel.h"
#include "argmin.h"

#include <vector>
#include <cmath>

LimitedModel::LimitedModel(
	std::unique_ptr <Model> model,
	std::unique_ptr <Model> min_model,
	std::unique_ptr <Model> max_model
) : model(std::move(model)), min_model(std::move(min_model)), max_model(std::move(max_model)) {}

void LimitedModel::UpdateInversion(const double param[], const InversionInfo& info) {
	model->UpdateInversion(param, info);
}

void LimitedModel::UpdateDirect(const double param[], const InversionInfo& info) {
	model->UpdateDirect(param, info);
}

void LimitedModel::ToDoubleInversion(double param[], const InversionInfo& info) const {
	model->ToDoubleInversion(param, info);
}

void LimitedModel::ToDoubleDirect(double param[], const InversionInfo& info) const {
	model->ToDoubleDirect(param, InversionInfo());
}

void LimitedModel::ToMinDouble(double param[]) const {
	min_model->ToDoubleInversion(param, InversionInfo());
}

void LimitedModel::ToMaxDouble(double param[]) const {
	max_model->ToDoubleInversion(param, InversionInfo());
}

static bool OneValueIsInBound(double cur_value, double min_value, double max_value) {
	if (!std::isnan(min_value) && cur_value < min_value) {
		return false;
	}
	if (!std::isnan(max_value) && cur_value > max_value) {
		return false;
	}
}

bool LimitedModel::IsInBound() const {
	const size_t nparams = model->Nparams(InversionInfo());
	static std::vector <double> cur;
	static std::vector <double> min_v;
	static std::vector <double> max_v;
	if (cur.size() < nparams) {
		cur.resize(nparams);
		min_v.resize(nparams);
		max_v.resize(nparams);
	}
	model->ToDoubleInversion(cur.data(), InversionInfo());
	min_model->ToDoubleInversion(min_v.data(), InversionInfo());
	max_model->ToDoubleInversion(max_v.data(), InversionInfo());
	for (size_t i = 0; i < nparams; ++i) {
		if (std::isnan(cur[i])) {
			continue;
		}
		if (!std::isnan(min_v[i]) && cur[i] < min_v[i]) {
			return false;
		}
		if (!std::isnan(max_v[i]) && cur[i] > max_v[i]) {
			return false;
		}
	}
	return true;
}

void LimitedModel::LimitByBound() {
	const size_t nparams = model->Nparams(InversionInfo());
	static std::vector <double> cur;
	static std::vector <double> min_v;
	static std::vector <double> max_v;
	if (cur.size() < nparams) {
		cur.resize(nparams);
		min_v.resize(nparams);
		max_v.resize(nparams);
	}
	model->ToDoubleInversion(cur.data(), InversionInfo());
	min_model->ToDoubleInversion(min_v.data(), InversionInfo());
	max_model->ToDoubleInversion(max_v.data(), InversionInfo());
	for (size_t i = 0; i < nparams; ++i) {
		if (std::isnan(cur[i])) {
			continue;
		}
		if (!std::isnan(min_v[i]) && cur[i] < min_v[i]) {
			cur[i] = min_v[i];
		}
		if (!std::isnan(max_v[i]) && cur[i] > max_v[i]) {
			cur[i] = max_v[i];
		}
	}
	model->UpdateInversion(cur.data(), InversionInfo());
}

void LimitedModel::ToFixArray(int fix[]) const {
	const size_t nparams = model->Nparams(InversionInfo());
	static std::vector <double> min_v;
	static std::vector <double> max_v;
	if (min_v.size() < nparams) {
		min_v.resize(nparams);
		max_v.resize(nparams);
	}
	min_model->ToDoubleInversion(min_v.data(), InversionInfo());
	max_model->ToDoubleInversion(max_v.data(), InversionInfo());
	for (size_t i = 0; i < nparams; ++i) {
		if (min_v[i] == max_v[i]) {
			fix[i] = FIX_PARAM;
		}
		else {
			fix[i] = UNFIX_PARAM;
		}
	}
}


