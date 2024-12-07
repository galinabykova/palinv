#include "IPaletteFunction.h"

IPaletteFunction::IPaletteFunction(const Model& min_model, const Model& max_model) :
	IDirectSolver(min_model, max_model),
	palette_data(NULL) {
}

IPaletteFunction::IPaletteFunction(const std::vector<size_t>& size_dim, const std::vector<double>& params_dim,
	const double* palette_data, const Model& min_model, const Model& max_model) :
	IDirectSolver(min_model, max_model),
	size_dim(size_dim), params_dim(params_dim), palette_data(palette_data) {
}

double IPaletteFunction::tf_for_start_model(const double signals[], const void* pdata, const double param[]) {
	auto data = (ParamForBestSolution*)pdata;
	int big_int = 10000000;
	(data->model).UpdateDirect(param, data->info);
	if (!(data->model).IsInBound()) {
		return big_int;
	}
	auto res = TargetFunction::calc(data->palette, *(data->model).model, data->info, data->exp_signals, data->err_calc);
	return res;
}

std::vector<double> IPaletteFunction::Merge(const std::vector< std::vector<double>>& v) {
	size_t size = 0;
	for (size_t i = 0; i < v.size(); ++i) {
		size += v[i].size();
	}
	std::vector <double> rez(size);
	size_t index = 0;
	for (size_t i = 0; i < v.size(); ++i) {
		for (size_t j = 0; j < v[i].size(); ++j) {
			rez[index++] = v[i][j];
		}
	}
	return rez;
}

std::vector<double> IPaletteFunction::Log10(const std::vector<double>& v) {
	std::vector <double> res(v.size());
	for (size_t i = 0; i < v.size(); i++) {
		res[i] = log10(v[i]);
	}
	return res;
}