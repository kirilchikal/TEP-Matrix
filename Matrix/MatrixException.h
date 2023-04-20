#pragma once
#include <iostream>
#include <exception>
#include <string>


class MatrixException : public std::exception
{

public:

	MatrixException(std::string error)
		:msg(error)
	{}

	const char* what() const noexcept
	{
		return msg.c_str();
	}


protected:
	std::string msg;

};


class ArgumentException final : public MatrixException
{
public:

	ArgumentException(std::string error)
		:MatrixException(error)
	{}

	const char* what() const noexcept
	{
		return msg.c_str();
	}

};


class FileException final : public MatrixException
{
public:

	FileException(std::string error)
		:MatrixException(error)
	{}

	const char* what() const noexcept
	{
		return msg.c_str();
	}

};