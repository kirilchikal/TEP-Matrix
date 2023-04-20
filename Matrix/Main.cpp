#include <iostream>
#include "Matrix.h"
#include <ctime>


void readMatrixTest()
{
	Matrix<int> mtx1(3, 6);
	Matrix<double> mtx2(3, 6);

	try
	{
		mtx1.readFromTheFile("matrix.txt");
		mtx2.readFromTheFile("matrix2.txt");
	}
	catch (const FileException& excpt)
	{
		std::cout << "File exception: " << excpt.what() << std::endl;
	}

	std::cout << "=======================================\n"
		<< "Wczytywanie macierzy z pliku:\n" 
		<< "=======================================\n"
		<< "int value:" << std::endl;
	mtx1.display();
	std::cout << "\ndouble value:" << std::endl;
	mtx2.display();
}


void transposeAndIdentityTest()
{
	std::cout << "=======================================\n"
		<< "Transponowanie macierzy:\n"
		<< "=======================================\n" << std::endl;
	Matrix<int> mtx(3, 5);
	mtx.fillRandom();
	Matrix<int> transpose = mtx.transpose();

	std::cout << "Maicerz:" << std::endl;
	mtx.display();
	std::cout << "\nTransponowana:" << std::endl;
	transpose.display();

	std::cout << "=======================================\n"
		<< "Macierz jednostkowa:\n"
		<< "=======================================\n" << std::endl;
	try
	{
		Matrix<int> ident(4, 4);
		ident.identity();
		std::cout << "Jednostkowa:" << std::endl;
		ident.display();
	}
	catch (const ArgumentException& excpt)
	{
		std::cout << "Argument exception: " << excpt.what() << std::endl;
	}
	catch (const MatrixException& excpt)
	{
		std::cout << "Matrix exception: " << excpt.what() << std::endl;
	}
}


void VectorsTest()
{
	std::cout << "=======================================\n"
		<< "Tworzenie wektorow:\n"
		<< "=======================================\n" << std::endl;

	Matrix<double> mtx(4, 3);
	mtx.fillRandom();
	Matrix<double> rowVector = mtx.rowVector(2);
	Matrix<double> columnVector = mtx.columnVector(2);

	std::cout << "Macierx 4x3:" << std::endl;
	mtx.display();
	std::cout << "\nVector from 3 row:" << std::endl;
	rowVector.display();
	std::cout << "\nVector from 3 column:" << std::endl;
	columnVector.display();

	std::cout << "=======================================\n"
		<< "Iloczyn skalarny:\n"
		<< "=======================================\n" << std::endl;

	try
	{
		Matrix<double> vector(1, 3);
		vector.fillRandom();

		double scalar = rowVector.scalarProduct(vector);

		std::cout << "Iloczyn skalarny vektorow 4x1 : " << scalar << std::endl;
		rowVector.display();
		vector.display();

	}
	catch (const MatrixException& excpt)
	{
		std::cout << "Vector exception: " << excpt.what() << std::endl;
	}
	catch (const std::exception& excpt)
	{
		std::cout << "Exception: " << excpt.what() << std::endl;
	}
}


void OperatorsTest()
{
	std::cout << "=======================================\n"
		<< "Dodawanie i odejmowanie:\n"
		<< "=======================================\n" << std::endl;

	Matrix<int> mtx1(2, 5);
	Matrix<int> mtx2(2, 5);
	mtx1.fillRandom();
	std::cout << "Macierz A " << std::endl;
	mtx1.display();
	std::cout << "\nMacierz B " << std::endl;
	mtx2.fillRandom();
	mtx2.display();

	std::cout << "\nMacierz C = (A + B) * 2 - B * 4" << std::endl;

	try
	{
		Matrix<int> sum = (mtx1 + mtx2) * 2 - mtx1 * 4;
		sum.display();
	}
	catch (const MatrixException& excpt)
	{
		std::cout << "Matrix exception: " << excpt.what() << std::endl;
	}
	catch (const std::exception& excpt)
	{
		std::cout << "Exception: " << excpt.what() << std::endl;
	}


	std::cout << "=======================================\n"
		<< "Mnozenie macierzy:\n"
		<< "=======================================\n" << std::endl;

	std::cout << "Macierz  A * ~B " << std::endl;

	try
	{
		Matrix<int> mult = mtx1 * mtx2.transpose();
		mult.display();
	}
	catch (const MatrixException& excpt)
	{
		std::cout << "Matrix exception: " << excpt.what() << std::endl;
	}
	catch (const std::exception& excpt)
	{
		std::cout << "Exception: " << excpt.what() << std::endl;
	}
}



int main()
{
	std::srand(time(0));

	readMatrixTest();

	transposeAndIdentityTest();

	VectorsTest();

	OperatorsTest();

	Matrix<int> mtx(10, 10);
	mtx.fillRandom();

	std::cout << "\n Save matrix: "<< std::endl;
	mtx.display();

	mtx.save("file.txt", "/:\\");

}