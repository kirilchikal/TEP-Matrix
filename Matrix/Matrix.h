#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include "MatrixException.h"


template <typename T>
class Matrix
{
public:
	Matrix(int row, int col);
	Matrix(const Matrix& otherM);
	Matrix(Matrix&& otherM);

	~Matrix();

	Matrix newMatrix(int rows, int cols);

	Matrix& operator=(const Matrix& otherM);
	Matrix& operator=(Matrix&& otherM);

	bool setValue(int row, int col, T val);
	T getValue(int row, int col) const;
	T operator()(int row, int col) const;
	T& operator()(int row, int col);

	void fillRandom();
	void fillWithZeros();
	void readFromTheFile(const std::string& fileName);

	Matrix rowVector(int row);
	Matrix columnVector(int col);
	bool isVector();
	T scalarProduct(Matrix& vector);

	Matrix transpose() const;
	Matrix diagonal() const;
	void identity();

	Matrix operator+(const Matrix& otherM)const &;
	Matrix operator+(const Matrix& otherM) && ;

	Matrix operator-(const Matrix& otherM)const &;
	Matrix operator-(const Matrix& otherM) && ;

	Matrix operator*(T multiplier) const &;
	Matrix operator*(T multiplier) && ;
	Matrix operator*(const Matrix& otherM) const;

	void display();

	bool save(const std::string& fileName, const char* separator);

private:
	T* matrix;
	int rows;
	int cols;
};


// Constructor
template <typename T>
Matrix<T>::Matrix(int row, int col)
{
	if (row < 0 || col < 0)
	{
		throw ArgumentException("Invalid argument!");
	}

	rows = row;
	cols = col;

	matrix = new T[rows * cols];
}


// Copy Constructor
template <typename T>
Matrix<T>::Matrix(const Matrix& otherM)
	:rows(otherM.row)
	, cols(otherM.col)
{
	matrix = new T[rows * cols];

	for (int i = 0; i < rows * cols; i++) {
		matrix[i] = otherM.matrix[i];
	}
}


// Move Constructor
template <typename T>
Matrix<T>::Matrix(Matrix&& otherM)
	:rows(otherM.rows)
	, cols(otherM.cols)
	, matrix(otherM.matrix)
{
	otherM.matrix = nullptr;
}


// Destructor
template <typename T>
Matrix<T>::~Matrix()
{
	delete[] matrix;
}



template <typename T>
Matrix<T> Matrix<T>::newMatrix(int rows, int cols)
{
	Matrix<T> newMtx(rows, cols);

	return newMtx;
}


// Copy assignment operator
template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& otherM)
{
	if (rows != otherM.rows && cols != otherM.cols)
	{
		delete[] matrix;

		rows = otherM.rows;
		cols = otherM.cols;

		matrix = new T[rows * cols];
	}

	for (int i = 0; i < rows * cols; i++)
	{
		matrix[i] = otherM.matrix[i];
	}
	
	return *this;
}


// Move assignment operator
template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix&& otherM)
{
	delete[] matrix;

	matrix = otherM.matrix;
	rows = otherM.rows;
	cols = otherM.cols;

	otherM.matrix = nullptr;
}


template <typename T>
bool Matrix<T>::setValue(int row, int col, T val)
{
	if ((row >= rows || row < 0) || (col >= cols || col < 0))
	{
		return false;
	}

	matrix[row * cols + col] = val;

	return true;
}


template <typename T>
T Matrix<T>::getValue(int row, int col) const
{
	if ((row >= rows || row < 0) || (col >= cols || col < 0))
	{
		throw ArgumentException("Argumnent out of range!");
	}

	return matrix[row * cols + col];
}


// Get value
template <typename T>
T Matrix<T>::operator()(int row, int col) const
{
	return getValue(row, col);
}


// Get reference (for modification)
template <typename T>
T& Matrix<T>::operator()(int row, int col)
{
	if ((row >= rows || row < 0) || (col >= cols || col < 0))
	{
		throw ArgumentException("Argumnent out of range!");
	}

	return matrix[row * cols + col];
}


template <typename T>
void Matrix<T>::fillRandom()
{
	for (int i = 0; i < cols * rows; i++)
	{
		matrix[i] = -100 + (T)(rand()) / ((T)(RAND_MAX / 300));
	}
}


template <typename T>
void Matrix<T>::fillWithZeros()
{
	for (int i = 0; i < rows * cols; i++)
	{
		matrix[i] = 0;
	}
}


template <typename T>
void Matrix<T>::readFromTheFile(const std::string& fileName)
{
	int fileRows = 0;
	int fileCols = 0;
	std::string line;

	std::ifstream fin(fileName);
	if (!fin)
	{
		throw FileException("The file could not be opened!");
	}

	// count rows in file
	while (std::getline(fin, line))
	{
		fileRows++;
	}

	// count columns in file
	std::stringstream lineStream(line);
	while (lineStream >> line)
	{
		fileCols++;
	}

	if (fileRows != rows || fileCols != cols)
	{
		fin.close();
		throw FileException("Matrix dimension mismatch in file!");
	}

	fin.clear();
	fin.seekg(0);

	int i;
	for (i = 0; i < rows * cols && !fin.eof(); i++)
	{
		fin >> matrix[i];
	}

	fin.close();

	while (i < rows * cols)
	{
		matrix[i] = 0; i++;
	}
}


template <typename T>
Matrix<T> Matrix<T>::rowVector(int row)
{
	if (row < 0 || row >= rows)
	{
		throw ArgumentException("Argumnent out of range!");
	}

	Matrix<T> vector(1, cols);
	for (int i = 0; i < cols; i++)
	{
		vector.setValue(0, i, matrix[row * cols + i]);
	}

	return vector;
}


template <typename T>
Matrix<T> Matrix<T>::columnVector(int col)
{
	if (col < 0 || col >= cols)
	{
		throw ArgumentException("Argumnent out of range!");
	}

	Matrix<T> vector(rows, 1);
	for (int i = 0; i < rows; i++)
	{
		vector.setValue(i, 0, matrix[cols * i + col]);
	}

	return vector;
}


template <typename T>
bool Matrix<T>::isVector()
{
	return (rows == 1 || cols == 1);
}


template <typename T>
T Matrix<T>::scalarProduct(Matrix& vector)
{
	if (!isVector() || !vector.isVector())
	{
		throw MatrixException("Matrix does not match with vector!");
	}

	if (rows != vector.rows || cols != vector.cols)
	{
		throw MatrixException("Vector dimension mismatch!");
	}

	T scalar = 0;
	for (int i = 0; i < rows * cols; i++)
	{
		scalar += matrix[i] * vector.matrix[i];
	}

	return scalar;
}


template <typename T>
Matrix<T> Matrix<T>::transpose() const
{
	Matrix<T> transposeMtx(cols, rows);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			transposeMtx.setValue(j, i, matrix[i * cols + j]);
		}
	}

	return transposeMtx;
}


template <typename T>
Matrix<T> Matrix<T>::diagonal() const
{
	if (rows != cols)
	{
		throw MatrixException("The dimensions mismatch with square matrix!");
	}

	Matrix<T> diagonalMtx(rows, cols);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (i != j)
			{
				diagonalMtx(i, j) = 0;
			}
			else
			{
				diagonalMtx(i, j) = getValue(i, j);
			}
		}
	}

	return diagonalMtx;
}


template <typename T>
void Matrix<T>::identity()
{
	if (rows != cols)
	{
		throw MatrixException("The dimensions mismatch with square matrix!");
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (i == j)
			{
				setValue(i, j, 1);
			}
			else
			{
				setValue(i, j, 0);
			}
		}
	}
}

/*
	+-* OPERATORS
*/


template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix& otherM)const &
{
	if (rows != otherM.rows || cols != otherM.cols)
	{
		throw MatrixException("Matrix dimensions mismatch!");
	}

	Matrix<T> sum(rows, cols);
	for (int i = 0; i < rows * cols; i++)
	{
		sum.matrix[i] = matrix[i] + otherM.matrix[i];
	}

	return sum;
}


template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix& otherM) &&
{
	if (rows != otherM.rows || cols != otherM.cols)
	{
		throw MatrixException("Matrix dimensions mismatch!");
	}

	for (int i = 0; i < rows * cols; i++)
	{
		matrix[i] += otherM.matrix[i];
	}

	return std::move(*this);
}


template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix& otherM)const &
{
	if (rows != otherM.rows || cols != otherM.cols)
	{
		throw MatrixException("Matrix dimensions mismatch!");
	}

	Matrix<T> difference(rows, cols);
	for (int i = 0; i < rows * cols; i++)
	{
		difference.matrix[i] = matrix[i] - otherM.matrix[i];
	}

	return difference;
}


template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix& otherM) &&
{
	if (rows != otherM.rows || cols != otherM.cols)
	{
		throw MatrixException("Matrix dimensions mismatch!");
	}

	for (int i = 0; i < rows * cols; i++)
	{
		matrix[i] -= otherM.matrix[i];
	}

	return std::move(*this);
}


template <typename T>
Matrix<T> Matrix<T>::operator*(T multiplier) const &
{
	Matrix<T> returnMatrix(rows, cols);
	for (int i = 0; i < rows * cols; i++)
	{
		returnMatrix.matrix[i] = matrix[i] * multiplier;
	}

	return returnMatrix;
}


template <typename T>
Matrix<T> Matrix<T>::operator*(T multiplier) &&
{
	for (int i = 0; i < rows * cols; i++)
	{
		matrix[i] *= multiplier;
	}

	return std::move(*this);
}


template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix& otherM) const
{
	if (cols != otherM.rows)
	{
		throw MatrixException("Matrix dimensions mismatch!");
	}

	Matrix<T> multi(rows, otherM.cols);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < otherM.cols; j++)
		{
			multi.setValue(i, j, 0);
			for (int k = 0; k < cols; k++)
			{
				multi(i, j) += matrix[i * cols + k] * otherM(k, j);
			}
		}
	}

	return multi;
}


/*
	--------------------------------------
*/

template <typename T>
void Matrix<T>::display()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			std::cout << matrix[i * cols + j] << " ";
		}
		std::cout << std::endl;
	}
}


template <typename T>
bool Matrix<T>::save(const std::string& fileName, const char* separator)
{
	std::ofstream fout(fileName);

	if (!fout)
	{
		return false;
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			fout << getValue(i, j) << separator;
		}

		fout << std::endl;
	}

	fout.close();

	return true;
}


