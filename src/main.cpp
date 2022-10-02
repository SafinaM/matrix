#include "../include/Matrix.h"
#include <iostream>
#include <vector>
#include <cassert>

//Let's writa a class of a matrix m * b, where m and n are comparably small number:
// Support:
// moving, copying
// itration in range-based loops,
// concatanation with operator |
// indexing with the help []
// sum

void assertMatrix(const std::vector<std::vector<int>>& refVector, const Matrix& matrix) {
	for (std::size_t i = 0; i < matrix.getHeight(); ++i) {
		for (std::size_t j = 0; j < matrix.getWidth(); ++j) {
			assert(refVector[i][j] == matrix[i][j]);
		}
	}
}

int main() {
	try {
		std::cout << "Test stage...\n" << std::endl;
		std::cout << "Initializer list...\n" << std::endl;
		Matrix matrix1{{10, 15, 15}, {30, 10, 18}};
		Matrix matrix2{{1, 1, 1}, {1, 1, 1}};
		// empty matrix
		Matrix matrix3{};
		// 1 row matrix
		Matrix matrix4{{45, 23, 24, 19, 24, 23, 34}};
		// matrix with empty rows
		Matrix matrix5{{}, {}, {}};

		int testArr1[] = {10, 15, 15, 30, 10, 18};
		uint32_t i = 0;
		std::cout << "Test of a range-based loop...\n" << std::endl;
		for (const auto& elem: matrix1) {
			assert(elem == testArr1[i++]);
		}

		std::cout << "Test of sum...\n" << std::endl;
		std::vector<std::vector<int>> testVector{{11, 16, 16}, {31, 11, 19}};
		auto status = matrix1.sum(matrix2);

		assert(status == Errors::Ok);
		assertMatrix(testVector, matrix1);

		std::cout << "Concatenation test (right)...\n" << std::endl;
		std::vector<std::vector<int>> testVectorForConcatenation1{{11, 16, 16, 1, 1, 1}, {31, 11, 19, 1, 1, 1}};
		Matrix concatenatedMatrix1 = matrix1 | matrix2;
		assertMatrix(testVectorForConcatenation1, concatenatedMatrix1);


		std::cout << "Concatenation test (bottom)...\n" << std::endl;
		std::vector<std::vector<int>> testVectorForConcatenation2{{11, 16, 16}, {31, 11, 19}, {1, 1, 1}, {1, 1, 1}};
		Matrix concatenatedMatrix2 = matrix1 + matrix2;
		assertMatrix(testVectorForConcatenation2, concatenatedMatrix2);

		std::vector<std::vector<int>> testVectorForMovingTest = {{11, 16, 16}, {31, 11, 19}};
		Matrix f = std::move(matrix1);
		assertMatrix(testVectorForMovingTest, f);

		// here exception should be thrown
		Matrix matrix6{{1, 1, 1}, {1, 1, 1, 4}};

	} catch (const std::exception& e) {\
		std::cout << "Processing of test exception!:" << std::endl;
		std::cerr << e.what() << std::endl;
	} catch (...) {
		std::cerr << "Something bad has happened! Possible memory corruption!" << std::endl;
	}
	return 0;
}
