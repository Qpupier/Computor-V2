/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matrix.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 17:07:55 by qpupier           #+#    #+#             */
/*   Updated: 2026/07/21 19:28:47 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Matrix.hpp"
#include "AST.hpp"

// Utils

static std::vector<std::string>					parse_line(					\
		std::string &line, unsigned long int * width)
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
		throw LogicError("Invalid matrix format: empty row");
	if (!*width)
		*width = row.size();
	else if (row.size() != *width)
		throw LogicError("Invalid matrix format: inconsistent row sizes");
	return (row);
}

static std::vector<std::vector<std::string>>	parse_matrix(				\
		std::string &matrix, unsigned long int * width)
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
		throw LogicError("Invalid matrix format: empty matrix");
	return (rows);
}

static Rational*								value_to_rational(			\
		std::string value, t_data &data, Matrix *matrix)
{
	AST*		cell;
	Rational*	rational;

	try
	{
		cell = compute_expression(value, data, true);
	}
	catch (const std::exception &e)
	{
		matrix->error(	\
				LogicError(std::string("Invalid matrix format: ") + e.what()));
	}
	if (!cell)
		matrix->error(LogicError("Invalid matrix format: invalid element"));
	if (cell->getLeft() || cell->getRight())
	{
		delete cell;
		matrix->error(LogicError("Invalid matrix format: incomplete element"));
	}
	rational = dynamic_cast<Rational*>(cell->getNode()->clone());
	delete cell;
	if (!rational)
		matrix->error(	\
				LogicError("Invalid matrix format: non-rational element"));
	return (rational);
}

static void										print_rounded_matrix(		\
		const Matrix *matrix)
{
	unsigned long	width;
	unsigned long	height;

	width = matrix->getWidth();
	height = matrix->getHeight();
	for (unsigned int j = 0; j < height; j++)
	{
		std::cout << "[ ";
		for (unsigned int i = 0; i < width; i++)
		{
			std::cout << matrix->getRoundedValue(i, j);
			if (i < width - 1)
				std::cout << " , ";
		}
		std::cout << " ]";
		if (j < height - 1)
			std::cout << std::endl;
	}
}

unsigned long int								line_pivot(					\
		const Matrix& matrix, unsigned long int pivot)
{
	for (unsigned long int i(pivot); i < matrix.getHeight(); i++)
		if (matrix.getValue(pivot, i))
			return (i);
	throw ERROR_MATRIX_INVERSION_PIVOT;
	return (pivot);
}

static void										elimination(				\
		Matrix& matrix, unsigned long int pivot, unsigned long int j)
{
	IType*	coeff;
	IType*	tmp;

	coeff = matrix[j][pivot]->clone();
	if (*coeff)
		for (unsigned long int i(0); i < matrix.getWidth(); i++)
		{
			tmp = *coeff * *matrix[pivot][i];
			matrix.setValue(i, j, *matrix[j][i] - *tmp);
			delete tmp;
		}
	delete coeff;
}

static void										gauss_elimination(			\
		Matrix& matrix, unsigned long int pivot)
{
	for (unsigned long int j(pivot + 1); j < matrix.getHeight(); j++)
		elimination(matrix, pivot, j);
}

static void										jordan_elimination(			\
		Matrix& matrix, unsigned long int pivot)
{
	for (unsigned long int j(0); j < pivot; j++)
		elimination(matrix, pivot, j);
}

static void										normalize_pivot(			\
		Matrix& matrix, unsigned long int pivot)
{
	IType*	coeff;

	coeff = matrix[pivot][pivot]->clone();
	if (*coeff != 1 && *coeff)
		for (unsigned long int i = 0; i < matrix.getWidth(); i++)
			matrix.setValue(i, pivot, *matrix[pivot][i] / *coeff);
	delete coeff;
}

static void										gauss_jordan_elimination(	\
		Matrix& matrix)
{
	for (unsigned long int pivot(0); pivot < matrix.getHeight(); pivot++)
	{
		unsigned long int pivot_line(line_pivot(matrix, pivot));

		if (pivot_line != pivot)
			matrix.swap_lines(pivot, pivot_line);
		normalize_pivot(matrix, pivot);
		gauss_elimination(matrix, pivot);
		jordan_elimination(matrix, pivot);
	}
}

static IType*									sqrt_final_value(			\
		Matrix* sqrt_value, const Matrix& value)
{
	Matrix*	copy;
	Matrix				test_exact_value;
	std::vector<unsigned char>	decimal_part;

	copy = new Matrix(*sqrt_value);
	for (unsigned long int j(0); j < copy->getHeight(); j++)
		for (unsigned long int i(0); i < copy->getWidth(); i++)
		{
			IType*		cell = copy->getValue(i, j);
			InfiniteFloat	cell_float;

			if (dynamic_cast<Rational*>(cell))
				cell_float = dynamic_cast<Rational*>(cell)->getValue();
			else
				cell_float = dynamic_cast<Real*>(cell)->getValue();
			decimal_part = cell_float.getDecimalPart().getDigits();
			for (std::size_t k(InfiniteFloat::PRINT_PRECISION); 	\
					k < decimal_part.size() && k < InfiniteFloat::MAX_PRECISION; k++)// [ ] Ajouter la meme limite pour les rationals
				decimal_part[k] = 0;
			if (!InfiniteInt(decimal_part))
				copy->setValue(i, j, new Rational(cell_float.getIntegerPart()));
			else
				copy->setValue(i, j, new Real(InfiniteFloat(cell_float.getIntegerPart(), InfiniteInt(decimal_part))));
		}
	Matrix* tmp = *copy ^ 2;
	if (*tmp == value)
	{
		delete tmp;
		return (copy);
	}
	delete tmp;
	delete copy;
	for (unsigned long int j(0); j < sqrt_value->getHeight(); j++)
		for (unsigned long int i(0); i < sqrt_value->getWidth(); i++)
		{
			IType*		cell = sqrt_value->getValue(i, j);

			if (cell->in_Q())
				sqrt_value->setValue(i, j, new Rational(*cell));
			else
				sqrt_value->setValue(i, j, new Real(*cell));
		}
	return (sqrt_value);
}


// Constructors and destructor

Matrix::Matrix(unsigned long int width, 	\
		unsigned long int height): _width(width), _height(height)
{
	if (!width || !height)
	{
		this->_width = 0;
		this->_height = 0;
		this->_matrix = nullptr;
		return ;
	}
	this->_matrix = new IType**[this->_height];
	for (unsigned long int i = 0; i < this->_height; i++)
	{
		this->_matrix[i] = new IType*[this->_width];
		for (unsigned long int j = 0; j < this->_width; j++)
			(*this)[i][j] = new Rational(i == j);
	}
}

Matrix::Matrix(std::string str, t_data &data): _width(0), _height(0)
{
	std::vector<std::vector<std::string>>	rows;
	unsigned long int						height;

	rows = parse_matrix(str, &this->_width);
	height = rows.size();
	this->_matrix = new IType**[height];
	for (unsigned long int j(0); j < height; j++)
	{
		this->_matrix[j] = new IType*[this->_width];
		this->_height++;
		for (unsigned long int i = 0; i < this->_width; i++)
			(*this)[j][i] = new Rational();
	}
	for (unsigned long int j(0); j < height; j++)
		for (unsigned long int i = 0; i < this->_width; i++)
			this->setValue(i, j, value_to_rational(rows[j][i], data, this));
}

Matrix::Matrix(const Matrix &other): 			\
		_width(other._width), _height(other._height)
{
	this->_matrix = new IType**[this->_height];
	for (unsigned long int i = 0; i < this->_height; i++)
	{
		this->_matrix[i] = new IType*[this->_width];
		for (unsigned long int j = 0; j < this->_width; j++)
			(*this)[i][j] = other._matrix[i][j]->clone();
	}
}

Matrix::Matrix(const IType &other): Matrix()
{
	const Matrix*		other_matrix;
	const Vector*		other_vector;
	const Polynomial*	other_polynomial;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	other_vector = dynamic_cast<const Vector*>(&other);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_matrix)
		*this = *other_matrix;
	else if (other_vector)
		*this = Matrix(*other_vector);
	else if (other_polynomial)
		*this = Matrix(*other_polynomial);
	else
		throw ERROR_UNEXPECTED;
}

Matrix::Matrix(const Vector &vector): Matrix(vector.size(), 1)
{
	for (unsigned long int i = 0; i < vector.size(); i++)
		this->setValue(i, 0, vector[i]->clone());
}

Matrix::Matrix(const Polynomial &polynomial)
{
	if (polynomial.getDividers().size() != 1 					\
			|| *polynomial.getDividers()[0].coefficient != 1 	\
			|| polynomial.getDividers()[0].power 				\
			|| polynomial.getTerms().size() != 1 				\
			|| polynomial.getTerms()[0].power)
		throw ERROR_UNEXPECTED;
	*this = Matrix(*polynomial.getTerms()[0].coefficient);
}

Matrix::~Matrix(void)
{
	this->free();
}


// Operator overloads

Matrix::operator bool() const
{
	return (this->_width && this->_height);
}

Matrix&		Matrix::operator=(const Matrix &other)
{
	if (this == &other)
		return (*this);
	this->free();
	this->_width = other._width;
	this->_height = other._height;
	this->_matrix = new IType**[other._height];
	for (unsigned int j = 0; j < other._height; j++)
	{
		this->_matrix[j] = new IType*[other._width];
		for (unsigned int i = 0; i < other._width; i++)
			(*this)[j][i] = other[j][i]->clone();
	}
	return (*this);
}

Matrix		Matrix::operator=(const IType &other)
{
	*this = Matrix(other);
	return (*this);
}

bool		Matrix::operator==(const IType &other) const
{
	Matrix	other_matrix;

	try
	{
		other_matrix = Matrix(other);
	}
	catch(const UnexpectedError &e)
	{
		return (false);
	}
	if (this->_width != other_matrix._width 	\
			|| this->_height != other_matrix._height)
		return (false);
	for (unsigned long int j = 0; j < this->_height; j++)
		for (unsigned long int i = 0; i < this->_width; i++)
			if (*this->_matrix[j][i] != *other_matrix._matrix[j][i])
				return (false);
	return (true);
}

bool		Matrix::operator==(const long long int value) const
{
	return (*this == Rational(value));
}

bool		Matrix::operator!=(const IType &other) const
{
	return (!(*this == other));
}

bool		Matrix::operator!=(const long long int value) const
{
	return (*this != Rational(value));
}

bool		Matrix::operator<(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Matrix::operator<(const long long int value) const
{
	return (*this < Rational(value));
}

bool		Matrix::operator<=(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Matrix::operator<=(const long long int value) const
{
	return (*this <= Rational(value));
}

bool		Matrix::operator>(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Matrix::operator>(const long long int value) const
{
	return (*this > Rational(value));
}

bool		Matrix::operator>=(const IType &other) const
{
	(void)other;
	return (false);
}

bool		Matrix::operator>=(const long long int value) const
{
	return (*this >= Rational(value));
}

IType**		Matrix::operator[](unsigned long int index) const
{
	if (index >= this->_height)
		throw ERROR_MATRIX_OUT_OF_RANGE;
	return (this->_matrix[index]);
}

IType**		Matrix::operator[](unsigned long int index)// [ ]: Utile ?
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
	for (unsigned long int i = 0; i < this->_height; i++)
		for (unsigned long int j = 0; j < this->_width; j++)
			result->setValue(j, i, *this->_matrix[i][j] + *other._matrix[i][j]);
	return (result);
}

Matrix*		Matrix::operator+(const Rational &other) const
{
	Matrix	*result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned long int i = 0; i < this->_height; i++)
		for (unsigned long int j = 0; j < this->_width; j++)
			result->setValue(j, i, *this->_matrix[i][j] + other);
	return (result);
}

Matrix*		Matrix::operator+(const Complex &other) const
{
	Rational	rational;

	try
	{
		rational = other;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (*this + rational);
}

IType*		Matrix::operator+(const Polynomial &other) const
{
	return (other + *this);
}

Matrix*		Matrix::operator+(const Vector &other) const
{
	return (*this + Matrix(other));
}

Matrix*		Matrix::operator+(const Real &other) const
{
	Matrix*	result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned long int i = 0; i < this->_height; i++)
		for (unsigned long int j = 0; j < this->_width; j++)
			result->setValue(j, i, *this->_matrix[i][j] + other);
	return (result);
}

IType*		Matrix::operator+(const IType &other) const
{
	const Matrix*		other_matrix;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this + *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this + *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this + *other_complex);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this + *other_polynomial);
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this + *other_vector);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this + *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Matrix*		Matrix::operator+(const long long int value) const
{
	return (*this + Rational(value));
}

Matrix*		Matrix::operator-(void) const
{
	return (*this * (-1));
}

Matrix*		Matrix::operator-(const Matrix &other) const
{
	Matrix*	result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned long int i = 0; i < this->_height; i++)
		for (unsigned long int j = 0; j < this->_width; j++)
			result->setValue(j, i, *this->_matrix[i][j] - *other._matrix[i][j]);
	return (result);
}

Matrix*		Matrix::operator-(const Rational &other) const
{
	Matrix*	result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned long int i = 0; i < this->_height; i++)
		for (unsigned long int j = 0; j < this->_width; j++)
			result->setValue(j, i, *this->_matrix[i][j] - other);
	return (result);
}

Matrix*		Matrix::operator-(const Complex &other) const
{
	Rational	rational;

	try
	{
		rational = other;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (*this - rational);
}

IType*		Matrix::operator-(const Polynomial &other) const
{
	IType*	tmp;
	IType*	result;

	tmp = other - *this;
	result = -*tmp;
	delete tmp;
	return (result);
}

Matrix*		Matrix::operator-(const Vector &other) const
{
	return (*this - Matrix(other));
}

Matrix*		Matrix::operator-(const Real &other) const
{
	Matrix*	result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned long int i = 0; i < this->_height; i++)
		for (unsigned long int j = 0; j < this->_width; j++)
			result->setValue(j, i, *this->_matrix[i][j] - other);
	return (result);
}

IType*		Matrix::operator-(const IType &other) const
{
	const Matrix*		other_matrix;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this - *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this - *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this - *other_complex);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this - *other_polynomial);
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this - *other_vector);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this - *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Matrix*		Matrix::operator-(const long long int value) const
{
	return (*this - Rational(value));
}

Matrix*		Matrix::operator*(const Matrix &other) const
{
	Matrix	*result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned long int i = 0; i < this->_height; i++)
		for (unsigned long int j = 0; j < this->_width; j++)
			result->setValue(j, i, *this->_matrix[i][j] * *other._matrix[i][j]);
	return (result);
}

Matrix*		Matrix::operator*(const Rational &other) const
{
	Matrix	*result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned long int i = 0; i < this->_height; i++)
		for (unsigned long int j = 0; j < this->_width; j++)
			result->setValue(j, i, *this->_matrix[i][j] * other);
	return (result);
}

Matrix*		Matrix::operator*(const Complex &other) const
{
	Rational	rational;

	try
	{
		rational = other;
	}
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (*this * rational);
}

IType*		Matrix::operator*(const Polynomial &other) const
{
	return (other * *this);
}

Matrix*		Matrix::operator*(const Vector &other) const
{
	return (*this * Matrix(other));
}

Matrix*		Matrix::operator*(const Real &other) const
{
	Matrix*	result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned long int i = 0; i < this->_height; i++)
		for (unsigned long int j = 0; j < this->_width; j++)
			result->setValue(j, i, *this->_matrix[i][j] * other);
	return (result);
}

IType*		Matrix::operator*(const IType &other) const
{
	const Matrix*		other_matrix;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this * *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this * *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this * *other_complex);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this * *other_polynomial);
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this * *other_vector);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this * *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Matrix*		Matrix::operator*(const long long int value) const
{
	return (*this * Rational(value));
}

Matrix*		Matrix::operator/(const Matrix &other) const
{
	Matrix*	result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned long int i = 0; i < this->_height; i++)
		for (unsigned long int j = 0; j < this->_width; j++)
			try
			{
				result->setValue(j, i, *this->_matrix[i][j] / *other._matrix[i][j]);
			}
			catch(const LogicError& e)
			{
				delete result;
				throw;
			}
	return (result);
}

Matrix*		Matrix::operator/(const Rational &other) const
{
	Matrix*	result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned long int i = 0; i < this->_height; i++)
		for (unsigned long int j = 0; j < this->_width; j++)
			try
			{
				result->setValue(j, i, *this->_matrix[i][j] / other);
			}
			catch(const std::exception &e)
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
	catch (const LogicError &e)
	{
		throw ERROR_OPERATION_MATRIX_COMPLEX;
	}
	return (*this / rational);
}

Polynomial*	Matrix::operator/(const Polynomial &other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = new Polynomial(other.getName(), 	\
			(Polynomial::t_term){this->clone(), 0});
	result = *tmp / other;
	delete tmp;
	return (result);
}

Matrix*		Matrix::operator/(const Vector &other) const
{
	return (*this / Matrix(other));
}

Matrix*		Matrix::operator/(const Real &other) const
{
	Matrix*	result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned long int i = 0; i < this->_height; i++)
		for (unsigned long int j = 0; j < this->_width; j++)
			try
			{
				result->setValue(j, i, *this->_matrix[i][j] / other);
			}
			catch(const std::exception &e)
			{
				delete result;
				throw;
			}
	return (result);
}

IType*		Matrix::operator/(const IType &other) const
{
	const Matrix*		other_matrix;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this / *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this / *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this / *other_complex);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this / *other_polynomial);
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this / *other_vector);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this / *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Matrix*		Matrix::operator/(const long long int value) const
{
	return (*this / Rational(value));
}

Matrix*		Matrix::operator%(const Matrix &other) const
{
	Matrix	*result;

	if (this->_width != other._width || this->_height != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(this->_width, this->_height);
	for (unsigned long int i = 0; i < this->_height; i++)
		for (unsigned long int j = 0; j < this->_width; j++)
			try
			{
				result->setValue(j, i, *this->_matrix[i][j] % *other._matrix[i][j]);
			}
			catch(const LogicError& e)
			{
				delete result;
				throw;
			}
	return (result);
}

Matrix*		Matrix::operator%(const Rational &other) const
{
	Matrix*	result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned long int i = 0; i < this->_height; i++)
		for (unsigned long int j = 0; j < this->_width; j++)
			try
			{
				result->setValue(j, i, *this->_matrix[i][j] % other);
			}
			catch(const LogicError& e)
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
	catch (const LogicError &e)
	{
		throw ERROR_MODULO_COMPLEX;
	}
	return (*this % rational);
}

Polynomial*	Matrix::operator%(const Polynomial &other) const
{
	Polynomial*	tmp;
	Polynomial*	result;

	tmp = new Polynomial(other.getName(), 	\
			(Polynomial::t_term){this->clone(), 0});
	result = *tmp % other;
	delete tmp;
	return (result);
}

Matrix*		Matrix::operator%(const Vector &other) const
{
	return (*this % Matrix(other));
}

Matrix*		Matrix::operator%(const Real &other) const
{
	Matrix*	result;

	result = new Matrix(this->_width, this->_height);
	for (unsigned long int i = 0; i < this->_height; i++)
		for (unsigned long int j = 0; j < this->_width; j++)
			try
			{
				result->setValue(j, i, *this->_matrix[i][j] % other);
			}
			catch(const LogicError& e)
			{
				delete result;
				throw;
			}
	return (result);
}

IType*		Matrix::operator%(const IType &other) const
{
	const Matrix*		other_matrix;
	const Rational*		other_rational;
	const Complex*		other_complex;
	const Polynomial*	other_polynomial;
	const Vector*		other_vector;
	const Real*			other_real;

	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (*this % *other_matrix);
	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (*this % *other_rational);
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (*this % *other_complex);
	other_polynomial = dynamic_cast<const Polynomial*>(&other);
	if (other_polynomial)
		return (*this % *other_polynomial);
	other_vector = dynamic_cast<const Vector*>(&other);
	if (other_vector)
		return (*this % *other_vector);
	other_real = dynamic_cast<const Real*>(&other);
	if (other_real)
		return (*this % *other_real);
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

Matrix*		Matrix::operator%(const long long int value) const
{
	return (*this % Rational(value));
}

Matrix*		Matrix::operator^(const Rational &other) const
{
	Matrix*	result;
	Matrix*	tmp;

	result = new Matrix(this->_width, this->_height);
	for (InfiniteInt i(0); i < other.getNumerator(); i++)
	{
		tmp = result;
		try
		{
			result = result->matrix_operator(*this);
		}
		catch(const LogicError &e)
		{
			delete result;
			throw;
		}
		delete tmp;
	}
	return (result);
}

IType*		Matrix::operator^(const IType &other) const
{
	Rational	power;

	try
	{
		power = Rational(other);
	}
	catch(const UnexpectedError &e)
	{
		throw UNSUPPORTED_EXPONENT;
	}
	if (!power.in_Z() || power < 0)
		throw UNSUPPORTED_EXPONENT;
	return (*this ^ power);
}

Matrix*		Matrix::operator^(const long long int value) const
{
	return (*this ^ Rational(value));
}


// Getters

IType*				Matrix::getValue(unsigned long int i, 			\
		unsigned long int j) const
{
	if (i >= this->_width || j >= this->_height)
		throw ERROR_MATRIX_OUT_OF_RANGE;
	return (this->_matrix[j][i]);
}

InfiniteFloat		Matrix::getRoundedValue(unsigned long int i, 	\
		unsigned long int j) const
{
	if (i >= this->_width || j >= this->_height)
		throw ERROR_MATRIX_OUT_OF_RANGE;
	return (dynamic_cast<Rational*>(this->_matrix[j][i])->getValue());// [ ] Changer ca
}

unsigned long int	Matrix::getWidth(void) const
{
	return (this->_width);
}

unsigned long int	Matrix::getHeight(void) const
{
	return (this->_height);
}


// Setters

void	Matrix::setValue(unsigned long int x, 	\
		unsigned long int y, IType* value)
{
	if (x >= this->_width || y >= this->_height)
		throw ERROR_MATRIX_OUT_OF_RANGE;
	delete (*this)[y][x];
	(*this)[y][x] = value;
}


// Methods

IType*			Matrix::clone(void) const
{
	return (new Matrix(*this));
}

IType*			Matrix::function_operator(const IType &other) const
{
	throw ERROR_UNEXPECTED;
	(void)other;
	return (nullptr);
}

Matrix*			Matrix::matrix_operator(const Matrix &other) const
{
	Matrix*	result;
	IType*	cell;
	IType*	mul;
	IType*	tmp;

	if (this->_width != other._height)
		throw ERROR_MATRIX_DIMENSIONS;
	result = new Matrix(other._width, this->_height);
	for (unsigned int i = 0; i < result->_height; i++)
		for (unsigned int j = 0; j < result->_width; j++)
		{
			cell = new Rational();
			for (unsigned int k = 0; k < this->_width; k++)
			{
				mul = *this->_matrix[i][k] * *other._matrix[k][j];
				tmp = cell;
				cell = *cell + *mul;
				delete tmp;
				delete mul;
			}
			result->setValue(j, i, cell);
		}
	return (result);
}

IType*			Matrix::matrix_operator(const IType &other) const
{
	Matrix	other_matrix;

	try
	{
		other_matrix = Matrix(other);
	}
	catch (const UnexpectedError &e)
	{
		throw ERROR_MATRIX_OPERATOR;
	}
	return (this->matrix_operator(other_matrix));
}

IType*			Matrix::matrix_inversion(void) const
{
	Matrix	matrix(this->_width * 2, this->_height);
	Matrix*	result;

	if (this->_width != this->_height)
		throw ERROR_MATRIX_INVERSION_SQUARE;
	for (unsigned long int j(0); j < this->_height; j++)
	{
		for (unsigned long int i(0); i < this->_width; i++)
			matrix.setValue(i, j, (*this)[j][i]->clone());
		for (unsigned long int i(0); i < this->_width; i++)
			matrix.setValue(this->_width + i, j, new Rational(i == j));
	}
	gauss_jordan_elimination(matrix);
	result = new Matrix(this->_width, this->_height);
	for (unsigned long int j(0); j < this->_height; j++)
		for (unsigned long int i(0); i < this->_width; i++)
			result->setValue(i, j, matrix[j][this->_width + i]->clone());
	return (result);
}

IType*			Matrix::norm(void) const
{
	IType*	result;

	result = new Rational();
	for (unsigned long int j(0); j < this->_height; j++)
		for (unsigned long int i(0); i < this->_width; i++)
		{
			IType*	square;
			IType*	tmp;

			square = *this->_matrix[j][i] * *this->_matrix[j][i];
			tmp = result;
			result = *result + *square;
			delete square;
			delete tmp;
		}
	return (result->sqrt());// TODO
}

IType*			Matrix::sqrt(void) const
{
	Matrix*			sqrt_value;
	InfiniteFloat	epsilon(1);
	IType*			delta;

	if (this->_width != this->_height)
		throw ERROR_MATRIX_SQRT_SQUARE;
	for (int i(0); i < InfiniteFloat::CALCULATION_PRECISION; i++)
		epsilon /= InfiniteFloat(10);
	sqrt_value = new Matrix(this->_width, this->_height);
	for (int i(0); i < 8; i++)// [ ] Definir un nombre d'iterations max
	{
		Matrix*	tmp = dynamic_cast<Matrix*>(sqrt_value->matrix_inversion());
		Matrix*	tmp2 = this->matrix_operator(*tmp);
		delete tmp;
		Matrix*	tmp3 = *sqrt_value + *tmp2;
		delete tmp2;
		delete sqrt_value;
		sqrt_value = *tmp3 / 2;
		delete tmp3;
		Matrix*	tmp4 = *sqrt_value ^ 2;
		Matrix*	tmp5 = *tmp4 - *this;
		delete tmp4;
		delta = tmp5->norm();
		delete tmp5;
		if (*delta < Real(epsilon))
		{
			delete delta;
			return (sqrt_final_value(sqrt_value, *this));
		}
		delete delta;
	}
	throw ERROR_UNEXPECTED;// [ ] Ameliorer erreur
	return (nullptr);
}

Rational*		Matrix::gcd(const Rational &other) const
{
	return (other.gcd(*this));
}

Rational*		Matrix::gcd(const Complex &other) const
{
	Rational*	first;
	Rational*	second;

	first = this->gcd(*dynamic_cast<const Rational*>(other.getReal()));
	second = first->gcd(*dynamic_cast<const Rational*>(other.getImaginary()));
	delete first;
	return (second);
}

Rational*		Matrix::gcd(const Vector &other) const
{
	return (this->gcd(Matrix(other)));
}

Rational*		Matrix::gcd(const Matrix &other) const
{
	Rational*	gcd;
	Rational*	tmp;

	gcd = this->gcd(*dynamic_cast<const Rational*>(other[0][0]));// [ ] Changer ca
	for (unsigned int i = 0; i < other.getHeight(); i++)
		for (unsigned int j = 0; j < other.getWidth(); j++)
		{
			tmp = gcd;
			gcd = gcd->gcd(*dynamic_cast<const Rational*>(other[i][j]));
			delete tmp;
		}
	return (gcd);
}

Rational*		Matrix::gcd(const IType &other) const
{
	const Rational*	other_rational;
	const Complex*	other_complex;
	const Matrix*	other_matrix;

	other_rational = dynamic_cast<const Rational*>(&other);
	if (other_rational)
		return (this->gcd(*other_rational));
	other_complex = dynamic_cast<const Complex*>(&other);
	if (other_complex)
		return (this->gcd(*other_complex));
	other_matrix = dynamic_cast<const Matrix*>(&other);
	if (other_matrix)
		return (this->gcd(*other_matrix));
	throw ERROR_UNEXPECTED;
	return (nullptr);
}

std::ostream&	Matrix::print(std::ostream &os) const
{
	unsigned long	width;
	unsigned long	height;

	width = this->_width;
	height = this->_height;
	for (unsigned int i = 0; i < height; i++)
	{
		os << "[ ";
		for (unsigned int j = 0; j < width; j++)
		{
			if (!this->_matrix[i][j])
				throw ERROR_UNEXPECTED;
			os << *this->_matrix[i][j];
			if (j < width - 1)
				os << " , ";
		}
		os << " ]";
		if (i < height - 1)
			os << std::endl;
	}
	return (os);
}

std::string		Matrix::to_string(void) const
{
	std::ostringstream	oss;

	this->print(oss);
	return (oss.str());
}

bool			Matrix::in_D(void) const
{
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			if (!this->_matrix[i][j]->in_D())
				return (false);
	return (true);
}

bool			Matrix::in_Q(void) const
{
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			if (!this->_matrix[i][j]->in_Q())
				return (false);
	return (true);
}

bool			Matrix::in_Z(void) const
{
	for (unsigned int i = 0; i < this->_height; i++)
		for (unsigned int j = 0; j < this->_width; j++)
			if (!this->_matrix[i][j]->in_Z())
				return (false);
	return (true);
}

void			Matrix::error(const LogicError &e)
{
	this->free();
	throw e;
}

void			Matrix::free(void)
{
	if (!this->_matrix || !this->_width || !this->_height)
		return ;
	for (unsigned int i = 0; i < this->_height; i++)
	{
		for (unsigned int j = 0; j < this->_width; j++)
			delete this->_matrix[i][j];
		delete[] this->_matrix[i];
	}
	delete[] this->_matrix;
	this->_width = 0;
	this->_height = 0;
	this->_matrix = nullptr;
}

void			Matrix::print_rounded(const std::string var) const
{
	if (this->in_Z())
		return ;
	std::cout << COLOR_DIM;
	if (!var.empty())
	{
		if (this->in_D())
			std::cout << var << " = " << std::endl;
		else
			std::cout << var << " ≈ " << std::endl;
	}
	print_rounded_matrix(this);
	std::cout << COLOR_RESET << std::endl;
}

void			Matrix::swap_lines(unsigned long int line1, 	\
		unsigned long int line2)
{
	IType**	tmp;

	if (line1 >= this->_height || line2 >= this->_height)
		throw ERROR_MATRIX_OUT_OF_RANGE;
	tmp = this->_matrix[line1];
	this->_matrix[line1] = this->_matrix[line2];
	this->_matrix[line2] = tmp;
}


// Output stream operator overload

std::ostream&	operator<<(std::ostream &os, const Matrix &matrix)
{
	return (matrix.print(os));
}


// Functions

bool	is_matrix(const IType& type)
{
	Matrix	matrix;

	try
	{
		matrix = Matrix(type);
	}
	catch (const UnexpectedError &e)
	{
		return (false);
	}
	return (true);
}
