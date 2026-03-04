/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matrix.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 17:07:55 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/04 19:43:45 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Matrix.hpp"
#include "AST.hpp"

// Constructors and destructor

static std::vector<std::string>	parse_line(std::string &line, unsigned long *width)
{
	std::vector<std::string>	row;
	std::size_t					pos;

	line = line.substr(1, line.size() - 2);
	pos = 0;
	while (pos != std::string::npos)
	{
		pos = line.find(',');
		if (pos != std::string::npos)
		{
			row.push_back(line.substr(0, pos));
			line.erase(0, pos + 1);
		}
		else
			row.push_back(line);
	}
	if (row.empty())
		throw std::logic_error("Invalid matrix format: empty row");
	if (!*width)
		*width = row.size();
	else if (row.size() != *width)
		throw std::logic_error("Invalid matrix format: inconsistent row sizes");
	return (row);
}

static std::vector<std::vector<std::string>>	parse_matrix(std::string &matrix, unsigned long *width)
{
	std::size_t								pos;
	std::vector<std::vector<std::string>>	rows;
	std::string								line;

	matrix = matrix.substr(1, matrix.size() - 2);
	pos = 0;
	while (pos != std::string::npos)
	{
		pos = matrix.find(';');
		if (pos != std::string::npos)
		{
			line = matrix.substr(0, pos);
			matrix.erase(0, pos + 1);
		}
		else
			line = matrix;
		rows.push_back(parse_line(line, width));
	}
	if (rows.empty())
		throw std::logic_error("Invalid matrix format: empty matrix");
	return (rows);
}

static void	free_matrix(Rational **matrix, unsigned long height)
{
	for (unsigned int i = 0; i < height; i++)
		delete[] matrix[i];
	delete[] matrix;
}

Matrix::Matrix(std::string str, t_data &data): _width(0), _height(0)
{
	std::vector<std::vector<std::string>>	rows;
	AST										*cell;
	Rational								*rational;

	rows = parse_matrix(str, &this->_width);
	this->_height = rows.size();
	this->_matrix = new Rational*[this->_height];
	for (unsigned int i = 0; i < this->_height; i++)
	{
		this->_matrix[i] = new Rational[this->_width];
		for (unsigned int j = 0; j < this->_width; j++)
		{
			try
			{
				cell = compute_expression(rows[i][j], data);
			}
			catch (const std::exception &e)
			{
				free_matrix(this->_matrix, this->_height);
				throw std::logic_error(std::string("Invalid matrix format: ") + e.what());
			}
			if (!cell)
			{
				free_matrix(this->_matrix, this->_height);
				throw std::logic_error("Invalid matrix format: invalid element");
			}
			if (cell->getLeft() || cell->getRight())
			{
				delete cell;
				free_matrix(this->_matrix, this->_height);
				throw std::logic_error("Invalid matrix format: incomplete element");
			}
			rational = dynamic_cast<Rational*>(cell->getNode()->clone());
			delete cell;
			if (!rational)
			{
				free_matrix(this->_matrix, this->_height);
				throw std::logic_error("Invalid matrix format: non-rational element");
			}
			this->_matrix[i][j] = *rational;
			delete rational;
		}
	}
}

Matrix::Matrix(const Matrix &other): _width(other._width), _height(other._height)
{
	this->_matrix = new Rational*[this->_height];
	for (unsigned int i = 0; i < this->_height; i++)
	{
		this->_matrix[i] = new Rational[this->_width];
		for (unsigned int j = 0; j < this->_width; j++)
			this->_matrix[i][j] = other._matrix[i][j];
	}
}

Matrix::~Matrix()
{
	for (unsigned int i = 0; i < this->_height; i++)
		delete[] this->_matrix[i];
	delete[] this->_matrix;
}


// Operator overloads
bool	Matrix::operator==(const Matrix &other) const
{
	if (this->_width != other._width || this->_height != other._height)
		return (false);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			if (this->_matrix[i][j] != other._matrix[i][j])
				return (false);
	return (true);
}

bool	Matrix::operator!=(const Matrix &other) const
{
	return (!(*this == other));
}

Matrix&	Matrix::operator=(const Matrix &other)
{
	if (this == &other)
		return (*this);
	for (unsigned int i = 0; i < this->_height; i++)
		delete[] this->_matrix[i];
	delete[] this->_matrix;
	this->_width = other._width;
	this->_height = other._height;
	this->_matrix = new Rational*[this->_height];
	for (unsigned int i = 0; i < this->_height; i++)
	{
		this->_matrix[i] = new Rational[this->_width];
		for (unsigned int j = 0; j < this->_width; j++)
			this->_matrix[i][j] = other._matrix[i][j];
	}
	return (*this);
}

Rational*	Matrix::operator[](unsigned int index) const
{
	if (index >= this->_height)
		throw std::out_of_range("Matrix index out of range");
	return (this->_matrix[index]);
}

IType*	Matrix::operator+(const IType &other) const
{
	return (nullptr);
	(void)other;
}

Matrix*	Matrix::operator+(const Matrix &other) const
{
	return (nullptr);
	(void)other;
}

IType*	Matrix::operator-(const IType &other) const
{
	return (nullptr);
	(void)other;
}

Matrix*	Matrix::operator-(const Matrix &other) const
{
	return (nullptr);
	(void)other;
}

IType*	Matrix::operator*(const IType &other) const
{
	return (nullptr);
	(void)other;
}

Matrix*	Matrix::operator*(const Matrix &other) const
{
	return (nullptr);
	(void)other;
}

IType*	Matrix::operator/(const IType &other) const
{
	return (nullptr);
	(void)other;
}

Matrix*	Matrix::operator/(const Matrix &other) const
{
	return (nullptr);
	(void)other;
}

IType*	Matrix::operator%(const IType &other) const
{
	return (nullptr);
	(void)other;
}

Matrix*	Matrix::operator%(const Matrix &other) const
{
	return (nullptr);
	(void)other;
}

IType*	Matrix::operator^(const IType &other) const
{
	return (nullptr);
	(void)other;
}

Matrix*	Matrix::operator^(const Matrix &other) const
{
	return (nullptr);
	(void)other;
}


// Getters
unsigned long	Matrix::getWidth(void) const
{
	return (this->_width);
}

unsigned long	Matrix::getHeight(void) const
{
	return (this->_height);
}


// Methods
IType*	Matrix::matrix_operator(const IType &other) const
{
	return (nullptr);
	(void)other;
}

std::ostream&	Matrix::print(std::ostream &os) const
{
	for (unsigned int i = 0; i < this->getHeight(); i++)
	{
		os << "[ ";
		for (unsigned int j = 0; j < this->getWidth(); j++)
		{
			os << this->_matrix[i][j];
			if (j < this->getWidth() - 1)
				os << " , ";
		}
		os << " ]";
		if (i < this->getHeight() - 1)
			os << std::endl;
	}
	return (os);
}

IType*	Matrix::clone(void) const
{
	return (new Matrix(*this));
}


// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Matrix &matrix)
{
	return (matrix.print(os));
}
