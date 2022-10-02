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

struct Matrix {
	Matrix(uint32_t width, uint32_t heigth) :
	m_width(width),
	m_height(heigth),
	m_vector(std::vector(heigth, std::vector<int>(width, 0))) {}

	Matrix(std::initializer_list<std::vector<int>> list);

	Matrix(Matrix&& matrix) = default;
	Matrix(const Matrix& matrix) = default;
	Matrix& operator =(const Matrix& matrix) = default;

	uint32_t getWidth() const noexcept ;
	uint32_t getHeight() const noexcept;

	std::vector<int>& operator[](uint32_t i);

	std::vector<int> operator[](uint32_t i) const;

	void clear() noexcept;

	Errors sum(const Matrix& matrix) noexcept;

	Matrix operator | (const Matrix& matrix);

	Matrix operator + (const Matrix& matrix);

	using value_type = int;
	using VectT = std::vector<int>;
	using ListT = std::vector<VectT>;

	size_t fullSize() const;

	struct const_iterator : std::iterator<std::bidirectional_iterator_tag, const int> {
		typename ListT::const_iterator ItL;
		typename VectT::const_iterator ItV;
		const ListT* ptr;

		// code for iterator I wrote as a test task for Stepic platform three years ago
		const_iterator() = default;
		~const_iterator() = default;
		const_iterator(const const_iterator& It) = default;
		const_iterator& operator=(const const_iterator&) = default;
		const_iterator(const ListT* ptr_);

		const_iterator& in_end();

		const int& operator*() const;

		const_iterator& operator++();
		const_iterator operator++(int);

		const_iterator& operator--() ;
		const_iterator operator--(int);

		bool operator== (const const_iterator& right) const;

		bool operator!= (const const_iterator& right) const ;
		const int* operator->() const;

	};

	const_iterator begin() const;
	const_iterator end() const;

	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;

private:

	std::vector<std::vector<int>> m_vector;
	unsigned int m_width = 0;
	unsigned int m_height = 0;

	void applyMathOperator(const Matrix& matrix, std::function<int(int, int)> op);

	bool validateForConcatenation(const Matrix& matrix, DirectionOfConcatenation direction);

	bool areSizesTheSame(const Matrix& matrix);


};
