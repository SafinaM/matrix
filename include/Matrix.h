//
// Created by mar on 02.10.2022.
//
#pragma once

#include "Matrix.h"
#include "Enums.h"
#include "MatrixException.h"

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <cassert>
#include <vector>
#include <iterator>

template <typename T>
struct Matrix {
	Matrix(short width, short heigth) :
	m_width(width),
	m_height(heigth),
	m_vector(std::vector(heigth, std::vector<T>(width, T()))) {}

	Matrix(std::initializer_list<std::vector<T>> list);

	Matrix(Matrix&& matrix) = default;
	Matrix(const Matrix& matrix) = default;
	Matrix& operator =(const Matrix& matrix) = default;

	short getWidth() const noexcept ;
	short getHeight() const noexcept;

	std::vector<T>& operator[](short i);

	std::vector<T> operator[](short i) const;

	void clear() noexcept;

	Errors sum(const Matrix& matrix) noexcept;

	Matrix operator | (const Matrix& matrix);

	Matrix operator + (const Matrix& matrix);

	using value_type = T;
	using VectT = std::vector<T>;
	using ListT = std::vector<VectT>;

	size_t fullSize() const;

	struct const_iterator : std::iterator<std::bidirectional_iterator_tag, const T> {
		typename ListT::const_iterator ItL;
		typename VectT::const_iterator ItV;
		const ListT* ptr;

		// code for iterator of 2Dim matrix I wrote as a test task for Stepic platform three years ago
		const_iterator() = default;
		~const_iterator() = default;
		const_iterator(const const_iterator& It) = default;
		const_iterator& operator=(const const_iterator&) = default;
		const_iterator(const ListT* ptr_);

		const_iterator& in_end();

		const T& operator*() const;

		const_iterator& operator++();
		const_iterator operator++(int);

		const_iterator& operator--() ;
		const_iterator operator--(int);

		bool operator== (const const_iterator& right) const;

		bool operator!= (const const_iterator& right) const ;
		const T* operator->() const;

	};

	const_iterator begin() const;
	const_iterator end() const;

	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;

private:

	std::vector<std::vector<T>> m_vector;
	unsigned short m_width = 0;
	unsigned short m_height = 0;

	void applyMathOperator(const Matrix& matrix, std::function<T(T, T)> op);

	bool validateForConcatenation(const Matrix& matrix, DirectionOfConcatenation direction);

	bool areSizesTheSame(const Matrix& matrix);

};


template <typename T>
Matrix<T>::Matrix(std::initializer_list<std::vector<T>> list) : m_vector(list) {
	if (!m_vector.empty()) {
		m_width = m_vector[0].size(); // here we sure that size is not zero
		for (const auto& row: m_vector) {
			if (m_width != row.size())
				throw MatrixException("Width of the rows are not the same!");
		}
		m_height = m_vector.size();
	}
}

template <typename T>
short Matrix<T>::getWidth() const noexcept {
	return m_width;
}

template <typename T>
short Matrix<T>::getHeight() const noexcept {
	return m_height;
}

template <typename T>
std::vector<T>& Matrix<T>::operator[](short i) {
	return m_vector[i];
}

template <typename T>
std::vector<T> Matrix<T>::operator[](short i) const {
	return m_vector[i];
}

template <typename T>
void Matrix<T>::clear() noexcept {
	m_vector.clear();
	m_width = 0;
	m_height = 0;
}

template <typename T>
bool Matrix<T>::areSizesTheSame(const Matrix<T>& matrix) {
	return getHeight() == matrix.getHeight() && getWidth() == matrix.getWidth();
}

template <typename T>
Errors Matrix<T>::sum(const Matrix<T>& matrix) noexcept {
	if (!areSizesTheSame(matrix))
		return Errors::InvalidSizes;

	applyMathOperator(matrix, std::plus<T>());

	return Errors::Ok;
}

template <typename T>
Matrix<T> Matrix<T>::operator | (const Matrix<T>& matrix) {

	if (!validateForConcatenation(matrix, DirectionOfConcatenation::Right))
		throw std::runtime_error("Invalid sizes");

	// let's believe that newWidth is also small number
	unsigned short newWidth = getWidth() + matrix.getWidth();

	Matrix<T> newMatrix(newWidth, getHeight());

	for (unsigned short i = 0; i < m_height; ++i) {
		assert(newWidth > getWidth());
		copy(m_vector[i].begin(), m_vector[i].end(), newMatrix[i].begin());
		const auto& row = matrix[i];
		std::copy(row.begin(), row.end(), newMatrix[i].begin() + getWidth());
	}

	return newMatrix;
}

template <typename T>
Matrix<T> Matrix<T>::operator + (const Matrix<T>& matrix) {

	if (!validateForConcatenation(matrix, DirectionOfConcatenation::Bottom))
		throw MatrixException("Invalid sizes");

	unsigned short newHeight = getHeight() + matrix.getHeight();

	Matrix<T> newMatrix(getWidth(), newHeight);

	for (unsigned short i = 0; i < m_height; ++i) {
		assert(newHeight > getHeight());
		copy(m_vector[i].begin(), m_vector[i].end(), newMatrix[i].begin());
	}

	const unsigned short matrixheight = matrix.getHeight();
	for (unsigned short i = 0; i < matrixheight; ++i) {
		assert(newHeight > getHeight());
		std::copy(matrix.m_vector[i].begin(), matrix.m_vector[i].end(), newMatrix[i + getHeight()].begin());
	}

	return newMatrix;
}

template <typename T>
size_t Matrix<T>::fullSize() const {
	size_t ans = 0;
	for (auto &v : m_vector) {
		ans += v.size();
	}
	return ans;
}

template <typename T>
Matrix<T>::const_iterator::const_iterator(const ListT* ptr_) :
	ItL(ptr_->cbegin()), ItV(ItL->cbegin()), ptr(ptr_){};

template <typename T>
typename Matrix<T>::const_iterator Matrix<T>::const_iterator::operator--(int) {
	auto tmp = *this;
	--(*this);
	return tmp;
}

template <typename T>
bool Matrix<T>::const_iterator::operator== (const const_iterator& right) const {
	return (ItL==right.ItL && ItV==right.ItV);
}

template <typename T>
bool Matrix<T>::const_iterator::operator!= (const const_iterator& right) const {
	return !(ItL==right.ItL && ItV==right.ItV);
}

template <typename T>
const T* Matrix<T>::const_iterator::operator->() const { return &(*ItV); }

template <typename T>
typename Matrix<T>::const_iterator Matrix<T>::begin() const {
	return !m_vector.empty() ? const_iterator(&m_vector) : const_iterator();
}

template <typename T>
typename Matrix<T>::const_iterator Matrix<T>::end() const {
	if (!m_vector.empty())
		return const_iterator(&m_vector).in_end();

	return const_iterator();
}

template <typename T>
typename Matrix<T>::const_reverse_iterator Matrix<T>::rbegin() const {
	return !m_vector.empty() ? const_reverse_iterator(end()) : const_reverse_iterator();
}

template <typename T>
typename Matrix<T>::const_reverse_iterator Matrix<T>::rend()   const {
	return !m_vector.empty() ? const_reverse_iterator(begin()) : const_reverse_iterator();
}

template <typename T>
void Matrix<T>::applyMathOperator(const Matrix<T>& matrix, std::function<T(T, T)> op) {

	assert(getWidth() == matrix.getWidth());
	assert(getHeight() == matrix.getHeight());

	for (unsigned short i = 0; i < matrix.getHeight(); ++i) {
		for (unsigned short j = 0; j < matrix.getWidth(); ++j) {
			m_vector[i][j] = op(m_vector[i][j], matrix[i][j]);
		}
	}
}

template <typename T>
bool Matrix<T>::validateForConcatenation(const Matrix<T>& matrix, DirectionOfConcatenation direction) {

	if (direction == DirectionOfConcatenation::Right) {
		return getHeight() == matrix.getHeight();
	}

	if (direction == DirectionOfConcatenation::Bottom) {
		return getWidth() == matrix.getWidth();
	}
	return false; // yes, by default return false
}

template <typename T>
typename Matrix<T>::const_iterator& Matrix<T>::const_iterator::operator++() {
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

template <typename T>
typename Matrix<T>::const_iterator Matrix<T>::const_iterator::operator++(int) {
	auto tmp = *this;
	++(*this);
	return tmp;
}

template <typename T>
typename Matrix<T>::const_iterator& Matrix<T>::const_iterator::in_end() {
	ItL = --(ptr->cend());
	ItV = ItL->cend();
	return *this;
}

template <typename T>
const T& Matrix<T>::const_iterator::operator*() const { return *ItV; };