#pragma once

#include <vector>
#include <complex>
#include <numeric>

void get_comb(int offset, int k, const std::vector<int>& arr,
	std::vector<int>& comb, std::vector<std::vector<int>>& combs);

std::vector<std::vector<int>> combinations(const std::vector<int>& arr, int k);

long factorial(const int n);

float get_vec_avg(std::vector<float> vec);

std::complex<float> perm(const std::vector<std::vector<std::complex<float>>>& A, int n);

std::vector<std::vector<std::complex<float>>> mat_mul(const std::vector<std::vector<std::complex<float>>>& mat1,
	const std::vector<std::vector<std::complex<float>>>& mat2);

std::vector<std::vector<std::complex<float>>> abs_mat(const std::vector<std::vector<std::complex<float>>>& mat);

std::vector<std::vector<std::complex<float>>> transpose(const std::vector<std::vector<std::complex<float>>> data);
