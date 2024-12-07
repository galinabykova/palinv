#include "Model.h"

#include <cstdio>
#include <vector>
#include <cmath>

void Model::LimitFromBelow(const Model& min) {
	const size_t nparams = Nparams(InversionInfo());
	static std::vector <double> cur;
	static std::vector <double> min_v;
	if (cur.size() < nparams) {
		cur.resize(nparams);
		min_v.resize(nparams);
	}
	min.ToDoubleInversion(min_v.data(), InversionInfo());
	ToDoubleInversion(cur.data(), InversionInfo());
	for (int i = 0; i < nparams; ++i) {
		if (std::isnan(cur[i]) || min_v[i] > cur[i]) {
			cur[i] = min_v[i];
		}
	}
	UpdateInversion(cur.data(), InversionInfo());
}

void Model::LimitFromAbove(const Model& max) {
	const size_t nparams = Nparams(InversionInfo());
	static std::vector <double> cur;
	static std::vector <double> max_v;
	if (cur.size() < nparams) {
		cur.resize(nparams);
		max_v.resize(nparams);
	}
	max.ToDoubleInversion(max_v.data(), InversionInfo());
	ToDoubleInversion(cur.data(), InversionInfo());
	for (int i = 0; i < nparams; ++i) {
		if (std::isnan(cur[i]) || max_v[i] < cur[i]) {
			cur[i] = max_v[i];
		}
	}
	UpdateInversion(cur.data(), InversionInfo());
}
