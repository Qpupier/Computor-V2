/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:18:03 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/23 14:03:38 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "Operator.hpp"
#include "Rational.hpp"
#include "Complex.hpp"
#include "Matrix.hpp"
#include "Polynomial.hpp"
#include "DefinedFunction.hpp"

// Utils

static IType*	defined_functions(std::string function_name)
{
	if (function_name == "sqrt")
		return (new DefinedFunction(DefinedFunction::E_FUNCTION_SQRT));
	if (function_name == "exp")
		return (new DefinedFunction(DefinedFunction::E_FUNCTION_EXPONENTIAL));
	if (function_name == "cos")
		return (new DefinedFunction(DefinedFunction::E_FUNCTION_COSINE));
	if (function_name == "sin")
		return (new DefinedFunction(DefinedFunction::E_FUNCTION_SINE));
	if (function_name == "tan")
		return (new DefinedFunction(DefinedFunction::E_FUNCTION_TANGENT));
	if (function_name == "rad")
		return (new DefinedFunction(DefinedFunction::E_FUNCTION_RADIAN));
	if (function_name == "deg")
		return (new DefinedFunction(DefinedFunction::E_FUNCTION_DEGREE));
	return (nullptr);
}

static IType*	find_function(IType *node, std::map<std::pair<std::string, 	\
		std::string>, const IType*> &stored)
{
	std::pair<std::string, std::string>	var_key;
	std::string							var_name;
	IType*								already_defined;
	const Polynomial*					polynomial;

	polynomial = dynamic_cast<const Polynomial*>(node);
	if (!polynomial)
		throw UnexpectedError(											\
				"Left side of function operator must be a variable");
	already_defined = defined_functions(polynomial->getName());
	if (already_defined)
		return (already_defined);
	var_name = to_lower(polynomial->getName());
	var_key.first = var_name;
	for (std::map<std::pair<std::string, std::string>, const IType*>	\
			::iterator it(stored.begin()); it != stored.end(); it++)
		if (to_lower(it->first.first) == to_lower(var_key.first) 		\
				&& !it->first.second.empty())
			return (it->second->clone());
	throw UnexpectedError("Function not found (" + var_name + ")");
	return (nullptr);
}

static IType*	matrix_operator(IType *left_entity, IType *right_entity)
{
	if (is_matrix(*left_entity) && is_matrix(*right_entity))
		return (left_entity->matrix_operator(*right_entity));
	return (*left_entity * *right_entity);
}

static IType*	get_result_function(IType *left, IType *right, 				\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	IType*	function;
	IType*	result;

	function = find_function(left, stored);
	try
	{
		result = function->function_operator(*right);
	}
	catch (...)
	{
		delete function;
		throw;
	}
	delete function;
	return (result);
}

static IType*	get_result(IType *left_entity, IType *right_entity, 		\
		Operator::t_operator op, 											\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	switch (op)
	{
		case Operator::E_ADD:
			return (*left_entity + *right_entity);
		case Operator::E_SUBTRACT:
			return (*left_entity - *right_entity);
		case Operator::E_MULTIPLY:
			return (*left_entity * *right_entity);
		case Operator::E_DIVIDE:
			return (*left_entity / *right_entity);
		case Operator::E_MODULO:
			return (*left_entity % *right_entity);
		case Operator::E_MATRIX:
			return (left_entity->matrix_operator(*right_entity));
		case Operator::E_POWER:
			return (*left_entity ^ *right_entity);
		case Operator::E_FUNCTION:
			return (get_result_function(left_entity, right_entity, stored));
		case Operator::E_INVERSE:
			return (right_entity->matrix_inversion());
		case Operator::E_UNKNOWN:
			return (matrix_operator(left_entity, right_entity));
		default:
			throw UnexpectedError("Unknown operator");
	}
	return (nullptr);
}


// Constructors and destructor

AST::AST(const Token &token, t_data &data): _left(nullptr), _right(nullptr)
{
	switch (token.getType())
	{
		case Token::E_TOKEN_OPERATOR_INVERSE:
			_node = new Operator(token);
			break;
		case Token::E_TOKEN_OPERATOR:
			_node = new Operator(token);
			break;
		case Token::E_TOKEN_NUMBER:
			_node = new Rational(token);
			break;
		case Token::E_TOKEN_IMAGINARY:
			_node = new Complex();
			break;
		case Token::E_TOKEN_MATRIX:
			_node = new Matrix(token, data);
			break;
		case Token::E_TOKEN_VECTOR:
			_node = new Vector(token, data);
			break;
		case Token::E_TOKEN_POLYNOMIAL:
			_node = new Polynomial(token);
			break;
		default:
			throw UnexpectedError("Invalid token type for AST node \
					(\"" + token.getValue() + "\")");
	}
}

AST::AST(const Token &token, AST *left, AST *right, t_data &data): 	\
		AST(token, data)
{
	this->_left = left;
	this->_right = right;
}

AST::~AST(void)
{
	this->free();
}


// Operator overloads

AST&	AST::operator=(const AST &other)
{
	if (this != &other)
	{
		this->free();
		this->_node = other._node->clone();
		this->_left = other._left;
		this->_right = other._right;
	}
	return (*this);
}


// Getters

IType*	AST::getNode(void) const
{
	return (this->_node);
}

AST*	AST::getLeft(void) const
{
	return (this->_left);
}

AST*	AST::getRight(void) const
{
	return (this->_right);
}


// Setters

void	AST::setLeft(AST *left)
{
	this->_left = left;
}

void	AST::setRight(AST *right)
{
	this->_right = right;
}


// Methods

std::ostream&	AST::print(std::ostream &os) const
{
	if (this->_left)
		os << *this->_left;
	if (this->getNode())
		os << *this->getNode();
	if (this->_right)
		os << *this->_right;
	return (os);
}

bool			AST::end_of_tree(void) const
{
	return (!this->_left && !this->_right);
}

void			AST::free(void)
{
	if (this->_left)
		delete this->_left;
	this->_left = nullptr;
	if (this->_right)
		delete this->_right;
	this->_right = nullptr;
	if (this->_node)
		delete this->_node;
	this->_node = nullptr;
}

void			AST::reduce_expression(	\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	Operator*	op;
	IType*		result;

	if (this->end_of_tree())
		return this->replace_variables(stored);
	if (this->_left)
		this->_left->reduce_expression(stored);
	if (this->_right)
		this->_right->reduce_expression(stored);
	if (this->end_of_tree())
		return;
	if (this->_left && !this->_right)
		result = this->_node->function_operator(*this->_left->_node);
	else
	{
		op = dynamic_cast<Operator*>(this->_node);
		if (!op)
			throw ERROR_OPERATOR_EXPECTED;
		result = get_result(this->_left ? this->_left->_node : nullptr, 	\
				this->_right->_node, op->getOperator(), stored);
	}
	this->free();
	this->_node = result;
	this->_left = nullptr;
	this->_right = nullptr;
}

void			AST::replace_variables(	\
		std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::pair<std::string, std::string>	var_key;
	const Polynomial*					polynomial;

	polynomial = dynamic_cast<const Polynomial*>(this->_node);
	if (!polynomial)
		return;
	var_key.first = to_lower(polynomial->getName());
	var_key.second = std::string();
	if (stored.find(var_key) != stored.end())
	{
		delete this->_node;
		this->_node = stored[var_key]->clone();
	}
}


// Output stream operator overload

std::ostream &operator<<(std::ostream &os, const AST &ast)
{
	return (ast.print(os));
}
