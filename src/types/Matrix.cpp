/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matrix.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 17:07:55 by qpupier           #+#    #+#             */
/*   Updated: 2026/03/06 18:29:40 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Matrix.hpp"
#include "AST.hpp"

// Utils
static std::vector<std::string>					parse_line(std::string &line, unsigned long *width)
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
	std::vector<std::vector<std::string>>	rows;
	std::string								line;
	std::size_t								pos;

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


// Constructors and destructor
Matrix::Matrix(unsigned long width, unsigned long height): _width(width), _height(height)
{
	this->_matrix = new Rational*[this->_height];
	for (unsigned int i = 0; i < this->_height; i++)
	{
		this->_matrix[i] = new Rational[this->_width];
		for (unsigned int j = 0; j < this->_width; j++)
			this->_matrix[i][j] = Rational();
	}
}

Matrix::Matrix(std::string str, t_data &data): _width(0), _height(0)
{
	std::vector<std::vector<std::string>>	rows;
	AST										*cell;
	Rational								*rational;
	unsigned long int						height;

	rows = parse_matrix(str, &this->_width);
	height = rows.size();
	this->_matrix = new Rational*[height];
	for (unsigned int i = 0; i < height; i++)
	{
		this->_matrix[i] = new Rational[this->_width];
		this->_height++;
		for (unsigned int j = 0; j < this->_width; j++)
		{
			try
			{
				cell = compute_expression(rows[i][j], data);
			}
			catch (const std::exception &e)
			{
				this->error(std::logic_error(std::string("Invalid matrix format: ") + e.what()));
			}
			if (!cell)
				this->error(std::logic_error("Invalid matrix format: invalid element"));
			if (cell->getLeft() || cell->getRight())
			{
				delete cell;
				this->error(std::logic_error("Invalid matrix format: incomplete element"));
			}
			rational = dynamic_cast<Rational*>(cell->getNode()->clone());
			delete cell;
			if (!rational)
				this->error(std::logic_error("Invalid matrix format: non-rational element"));
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
Matrix&		Matrix::operator=(const Matrix &other)
{
	Matrix	new_matrix(other);

	if (this == &other)
		return (*this);
	this->~Matrix();
	*this = new_matrix;
	return (*this);
}

bool		Matrix::operator==(const Matrix &other) const
{
	if (this->_width != other._width || this->_height != other._height)
		return (false);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			if (this->_matrix[i][j] != other._matrix[i][j])
				return (false);
	return (true);
}

bool		Matrix::operator!=(const Matrix &other) const
{
	return (!(*this == other));
}

Rational*	Matrix::operator[](unsigned int index) const
{
	if (index >= this->_height)
		throw ERROR_MATRIX_OUT_OF_RANGE;
	return (this->_matrix[index]);
}

Matrix*		Matrix::operator+(const Matrix &other) const
{
	Matrix	*result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			result->_matrix[i][j] = this->_matrix[i][j] + other._matrix[i][j];
	return (result);
}

Matrix*		Matrix::operator+(const Rational &other) const
{
	Matrix	*result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			result->_matrix[i][j] = this->_matrix[i][j] + other;
	return (result);
}

Matrix*		Matrix::operator+(const Complex &other) const
{
	Rational	rational;

	try
	{
		rational = other;
	}
	catch (const std::logic_error &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (*this + rational);
}

IType*		Matrix::operator+(const IType &other) const
{
	const Matrix	*other_matrix;
	const Rational	*other_rational;
	const Complex	*other_complex;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this + *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this + *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this + *other_complex);
	return (nullptr);
}

Matrix*		Matrix::operator-(const Matrix &other) const
{
	Matrix	*result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			result->_matrix[i][j] = this->_matrix[i][j] - other._matrix[i][j];
	return (result);
}

Matrix*		Matrix::operator-(const Rational &other) const
{
	Matrix	*result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			result->_matrix[i][j] = this->_matrix[i][j] - other;
	return (result);
}

Matrix*		Matrix::operator-(const Complex &other) const
{
	Rational	rational;

	try
	{
		rational = other;
	}
	catch (const std::logic_error &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (*this - rational);
}

IType*		Matrix::operator-(const IType &other) const
{
	const Matrix	*other_matrix;
	const Rational	*other_rational;
	const Complex	*other_complex;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this - *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this - *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this - *other_complex);
	return (nullptr);
}

Matrix*		Matrix::operator*(const Matrix &other) const
{
	Matrix	*result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			result->_matrix[i][j] = this->_matrix[i][j] * other._matrix[i][j];
	return (result);
}

Matrix*		Matrix::operator*(const Rational &other) const
{
	Matrix	*result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			result->_matrix[i][j] = this->_matrix[i][j] * other;
	return (result);
}

Matrix*		Matrix::operator*(const Complex &other) const
{
	Rational	rational;

	try
	{
		rational = other;
	}
	catch (const std::logic_error &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (*this * rational);
}

IType*		Matrix::operator*(const IType &other) const
{
	const Matrix	*other_matrix;
	const Rational	*other_rational;
	const Complex	*other_complex;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this * *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this * *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this * *other_complex);
	return (nullptr);
}

Matrix*		Matrix::operator/(const Matrix &other) const
{
	Matrix	*result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			try
			{
				result->_matrix[i][j] = this->_matrix[i][j] / other._matrix[i][j];
			}
			catch(const std::logic_error& e)
			{
				delete result;
				throw;
			}
	return (result);
}

Matrix*		Matrix::operator/(const Rational &other) const
{
	Matrix	*result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			try
			{
				result->_matrix[i][j] = this->_matrix[i][j] / other;
			}
			catch(const std::logic_error& e)
			{
				delete result;
				throw;
			}
	return (result);
}

Matrix*		Matrix::operator/(const Complex &other) const
{
	Rational	rational;

	try
	{
		rational = other;
	}
	catch (const std::logic_error &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (*this / rational);
}

IType*		Matrix::operator/(const IType &other) const
{
	const Matrix	*other_matrix;
	const Rational	*other_rational;
	const Complex	*other_complex;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this / *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this / *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this / *other_complex);
	return (nullptr);
}

Matrix*		Matrix::operator%(const Matrix &other) const
{
	Matrix	*result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			try
			{
				result->_matrix[i][j] = this->_matrix[i][j] % other._matrix[i][j];
			}
			catch(const std::logic_error& e)
			{
				delete result;
				throw;
			}
	return (result);
}

Matrix*		Matrix::operator%(const Rational &other) const
{
	Matrix	*result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			try
			{
				result->_matrix[i][j] = this->_matrix[i][j] % other;
			}
			catch(const std::logic_error& e)
			{
				delete result;
				throw;
			}
	return (result);
}

Matrix*		Matrix::operator%(const Complex &other) const
{
	Rational	rational;

	try
	{
		rational = other;
	}
	catch (const std::logic_error &e)
	{
		throw ERROR_MODULO_COMPLEX;
	}
	return (*this % rational);
}

IType*		Matrix::operator%(const IType &other) const
{
	const Matrix	*other_matrix;
	const Rational	*other_rational;
	const Complex	*other_complex;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this % *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this % *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this % *other_complex);
	return (nullptr);
}

Matrix*		Matrix::operator^(const Matrix &other) const
{
	Matrix		*result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			try
			{
				result->_matrix[i][j] = this->_matrix[i][j] ^ other._matrix[i][j];
			}
			catch(const std::logic_error& e)
			{
				delete result;
				throw;
			}
	return (result);
}

Matrix*		Matrix::operator^(const Rational &other) const
{
	Matrix	*result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			try
			{
				result->_matrix[i][j] = this->_matrix[i][j] ^ other;
			}
			catch(const std::logic_error& e)
			{
				delete result;
				throw;
			}
	return (result);
}

Matrix*		Matrix::operator^(const Complex &other) const
{
	Rational	rational;

	try
	{
		rational = other;
	}
	catch (const std::logic_error &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (*this ^ rational);
}

IType*		Matrix::operator^(const IType &other) const
{
	const Matrix	*other_matrix;
	const Rational	*other_rational;
	const Complex	*other_complex;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this ^ *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this ^ *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this ^ *other_complex);
	return (nullptr);
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


// Setters
void	Matrix::setValue(unsigned int i, unsigned int j, Rational *value)
{
	if (i >= this->_height || j >= this->_width)
		throw ERROR_MATRIX_OUT_OF_RANGE;
	this->_matrix[i][j] = value;
}


// Methods
Matrix*			Matrix::matrix_operator(const Matrix &other) const
{
	Matrix		*result;
	Rational	*cell;
	Rational	*mul;
	Rational	*tmp;

	if (this->_width != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(other._width, this->_height);
	for (unsigned int i = 0; i < result->_height; i++)
		for (unsigned int j = 0; j < result->_width; j++)
		{
			cell = new Rational();
			for (unsigned int k = 0; k < this->_width; k++)
			{
				mul = this->_matrix[i][k] * other._matrix[k][j];
				tmp = cell;
				cell = *cell + *mul;
				delete tmp;
				delete mul;
			}
			result->_matrix[i][j] = cell;
		}
	return (result);
}

Matrix*			Matrix::matrix_operator(const Rational &other) const
{
	throw ERROR_MATRIX_OPERATOR;
	(void)other;
	return (nullptr);
}

Matrix*			Matrix::matrix_operator(const Complex &other) const
{
	throw ERROR_MATRIX_OPERATOR;
	(void)other;
	return (nullptr);
}

IType*			Matrix::matrix_operator(const IType &other) const
{
	const Matrix	*other_matrix;
	const Rational	*other_rational;
	const Complex	*other_complex;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (this->matrix_operator(*other_matrix));
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (this->matrix_operator(*other_rational));
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (this->matrix_operator(*other_complex));
	return (nullptr);
}

std::ostream&	Matrix::print(std::ostream &os) const
{
	unsigned long	width;
	unsigned long	height;

	width = this->getWidth();
	height = this->getHeight();
	for (unsigned int i = 0; i < height; i++)
	{
		os << "[ ";
		for (unsigned int j = 0; j < width; j++)
		{
			os << this->_matrix[i][j];
			if (j < width - 1)
				os << " , ";
		}
		os << " ]";
		if (i < height - 1)
			os << std::endl;
	}
	return (os);
}

IType*			Matrix::clone(void) const
{
	return (new Matrix(*this));
}

void			Matrix::error(const std::logic_error &e) const
{
	this->~Matrix();
	throw e;
}


// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const Matrix &matrix)
{
	return (matrix.print(os));
}
