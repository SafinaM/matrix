//
// Created by mar on 02.10.2022.
//
#include "../include/Matrix.h"
#include "../include/MatrixException.h"
#include "../include/Enums.h"

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <cassert>

#include <iterator>

Matrix::Matrix(std::initializer_list<std::vector<int>> list) : m_vector(list) {
	if (!m_vector.empty()) {
		m_width = m_vector[0].size(); // here we sure that size is not zero
		for (const auto& row: m_vector) {
			if (m_width != row.size())
				throw MatrixException("Width of the rows are not the same!");
		}
		m_height = m_vector.size();
	}
}

uint32_t Matrix::getWidth() const noexcept {
	return m_width;
}

uint32_t Matrix::getHeight() const noexcept {
	return m_height;
}

std::vector<int>& Matrix::operator[](uint32_t i) {
	return m_vector[i];
}

std::vector<int> Matrix::operator[](uint32_t i) const {
	return m_vector[i];
}

void Matrix::clear() noexcept {
	m_vector.clear();
	m_width = 0;
	m_height = 0;
}


bool Matrix::areSizesTheSame(const Matrix& matrix) {
	return getHeight() == matrix.getHeight() && getWidth() == matrix.getWidth();
}

Errors Matrix::sum(const Matrix& matrix) noexcept {
	if (!areSizesTheSame(matrix))
		return Errors::InvalidSizes;

	applyMathOperator(matrix, std::plus<int>());

	return Errors::Ok;
}

Matrix Matrix::operator | (const Matrix& matrix) {

	if (!validateForConcatenation(matrix, DirectionOfConcatenation::Right))
		throw std::runtime_error("Invalid sizes");

	uint32_t newWidth = getWidth() + matrix.getWidth();

	// full allocation for 1 time
	Matrix newMatrix(newWidth, getHeight());

	for (uint32_t i = 0; i < m_height; ++i) {
		assert(newWidth > getWidth());
		copy(m_vector[i].begin(), m_vector[i].end(), newMatrix[i].begin());
		const auto& row = matrix[i];
		std::copy(row.begin(), row.end(), newMatrix[i].begin() + getWidth());
	}

	return newMatrix;
}

Matrix Matrix::operator + (const Matrix& matrix) {

	if (!validateForConcatenation(matrix, DirectionOfConcatenation::Bottom))
		throw MatrixException("Invalid sizes");

	uint32_t newHeight = getHeight() + matrix.getHeight();

	// full allocation for 1 time, more memory, but we avoid some allocations (system calls)
	Matrix newMatrix(getWidth(), newHeight);

	for (uint32_t i = 0; i < m_height; ++i) {
		assert(newHeight > getHeight());
		// copy first matrix by string
		copy(m_vector[i].begin(), m_vector[i].end(), newMatrix[i].begin());
	}

	const uint32_t matrixheight = matrix.getHeight();
	for (uint32_t i = 0; i < matrixheight; ++i) {
		assert(newHeight > getHeight());
		std::copy(matrix.m_vector[i].begin(), matrix.m_vector[i].end(), newMatrix[i + getHeight()].begin());
	}

	return newMatrix;
}

size_t Matrix::fullSize() const {
	size_t ans = 0;
	for (auto &v : m_vector) {
		ans += v.size();
	}
	return ans;
}

Matrix::const_iterator::const_iterator(const ListT* ptr_) :
	ItL(ptr_->cbegin()), ItV(ItL->cbegin()), ptr(ptr_){};

Matrix::const_iterator& Matrix::const_iterator::in_end() {
	ItL = --(ptr->cend());
	ItV = ItL->cend();
	return *this;
}

const int& Matrix::const_iterator::operator*() const { return *ItV; };

Matrix::const_iterator& Matrix::const_iterator::operator++() {
	if (ItV == std::prev((*ItL).cend())) {
		++ItL;
		if (ItL != ptr->cend()) {
			ItV = ItL->cbegin();
		} else {++ItV; --ItL; }
	}
	else
		++ItV;

	return *this;
}

Matrix::const_iterator Matrix::const_iterator::operator++(int) {
	auto tmp = *this;
	++(*this);
	return tmp;
}

Matrix::const_iterator& Matrix::const_iterator::operator--() {
	if (ItV == (*ItL).cbegin()) {
		if (ItL != ptr->cbegin()) {
			--ItL;
			ItV = std::prev((*ItL).cend());
		}
	}
	else
		--ItV;

	return *this;
}

Matrix::const_iterator Matrix::const_iterator::operator--(int) {
	auto tmp = *this;
	--(*this);
	return tmp;
}

bool Matrix::const_iterator::operator== (const const_iterator& right) const {
	return (ItL==right.ItL && ItV==right.ItV);
}

bool Matrix::const_iterator::operator!= (const const_iterator& right) const {
	return !(ItL==right.ItL && ItV==right.ItV);
}

const int* Matrix::const_iterator::operator->() const { return &(*ItV); }


Matrix::const_iterator Matrix::begin() const {
	return !m_vector.empty() ? const_iterator(&m_vector) : const_iterator();
}

Matrix::const_iterator Matrix::end() const {
	if (!m_vector.empty())
		return const_iterator(&m_vector).in_end();

	return const_iterator();
}

// определите методы rbegin / rend
Matrix::const_reverse_iterator Matrix::rbegin() const {
	return !m_vector.empty() ? const_reverse_iterator(end()) : const_reverse_iterator();
}
Matrix::const_reverse_iterator Matrix::rend()   const {
	return !m_vector.empty() ? const_reverse_iterator(begin()) : const_reverse_iterator();
}


void Matrix::applyMathOperator(const Matrix& matrix, std::function<int(int, int)> op) {

	assert(getWidth() == matrix.getWidth());
	assert(getHeight() == matrix.getHeight());

	for (uint32_t i = 0; i < matrix.getHeight(); ++i) {
		for (uint32_t j = 0; j < matrix.getWidth(); ++j) {
			m_vector[i][j] = op(m_vector[i][j], matrix[i][j]);
		}
	}
}

bool Matrix::validateForConcatenation(const Matrix& matrix, DirectionOfConcatenation direction) {

	if (direction == DirectionOfConcatenation::Right) {
		return getHeight() == matrix.getHeight();
	}

	if (direction == DirectionOfConcatenation::Bottom) {
		return getWidth() == matrix.getWidth();
	}
	return false; // yes, by default return false
}
