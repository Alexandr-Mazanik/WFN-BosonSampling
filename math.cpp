#include "math.h"

void get_comb(int offset, int k, const std::vector<int>& arr,
	std::vector<int>& comb, std::vector<std::vector<int>>& combs) {
	if (k == 0) {
		combs.push_back(comb);
		return;
	}
	for (int i = offset; i <= arr.size() - k; ++i) {
		comb.push_back(arr[i]);
		get_comb(i + 1, k - 1, arr, comb, combs);
		comb.pop_back();
	}
};

std::vector<std::vector<int>> combinations(const std::vector<int>& arr, int k) {
	std::vector<std::vector<int>> combs;
	std::vector<int> comb;

	get_comb(0, k, arr, comb, combs);

	return combs;
}

long factorial(const int n) {
	long f = 1;
	for (int i = 1; i <= n; ++i)
		f *= i;
	return f;
}

std::complex<float> perm(const std::vector<std::vector<std::complex<float>>>& A, int n) {
	std::vector<std::complex<float>> row_comb(n, std::complex<float>(0));
	std::complex<float> total = 0;
	int old_grey = 0, sign = +1;
	std::vector<int> binary_power_dict;
	for (int i = 0; i < n; ++i)
		binary_power_dict.push_back(pow(2, i));
	int num_loops = pow(2, n);
	for (int k = 0; k < num_loops; ++k) {
		int bin_index = (k + 1) % num_loops;
		std::complex<float> reduced = 1;
		for (std::complex<float> elem : row_comb)
			reduced *= elem;
		total += std::complex<float>(sign * reduced.real(), sign * reduced.imag());
		int new_grey = bin_index ^ (bin_index / 2);
		int grey_diff = old_grey ^ new_grey;
		int grey_diff_index = std::find(binary_power_dict.begin(), binary_power_dict.end(), grey_diff) - binary_power_dict.begin();
		std::vector<std::complex<float>> new_vector = A[grey_diff_index];
		int direction = (old_grey > new_grey) - (old_grey < new_grey);
		for (int i = 0; i < n; ++i)
			row_comb[i] += std::complex<float>(direction * new_vector[i].real(), direction * new_vector[i].imag());
		sign = -sign;
		old_grey = new_grey;
	}

	return total;
}

std::vector<std::vector<std::complex<float>>> mat_mul(const std::vector<std::vector<std::complex<float>>>& mat1,
	const std::vector<std::vector<std::complex<float>>>& mat2) {
	
	std::vector<std::vector<std::complex<float>>> mat3;
	mat3.resize(mat1.size());
	for (int i = 0; i < mat3.size(); ++i)
		mat3[i].resize(mat2[0].size());

	for (int i = 0; i < mat1.size(); ++i)
		for (int j = 0; j < mat2[0].size(); ++j) {
			mat3[i][j] = 0;
			for (int k = 0; k < mat2.size(); ++k)
				mat3[i][j] += mat1[i][k] * mat2[k][j];
		}

	return mat3;
}

std::vector<std::vector<std::complex<float>>> transpose(const std::vector<std::vector<std::complex<float>>> data) {

	std::vector<std::vector<std::complex<float>>> result(data[0].size(),
		std::vector<std::complex<float>>(data.size()));
	for (std::vector<std::complex<float>>::size_type i = 0; i < data[0].size(); i++)
		for (std::vector<std::complex<float>>::size_type j = 0; j < data.size(); j++) {
			result[i][j] = data[j][i];
		}
	return result;
}
