#pragma once

#include <vector>
#include <complex>

void get_comb(int offset, int k, const std::vector<int>& arr,
	std::vector<int>& comb, std::vector<std::vector<int>>& combs);

std::vector<std::vector<int>> combinations(const std::vector<int>& arr, int k);

long factorial(const int n);

std::complex<float> perm(const std::vector<std::vector<std::complex<float>>>& A, int n);

std::vector<std::vector<std::complex<float>>> mat_mul(const std::vector<std::vector<std::complex<float>>>& mat1,
	const std::vector<std::vector<std::complex<float>>>& mat2);

std::vector<std::vector<std::complex<float>>> transpose(const std::vector<std::vector<std::complex<float>>> data);
