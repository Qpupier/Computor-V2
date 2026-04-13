/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:18:03 by qpupier           #+#    #+#             */
/*   Updated: 2026/04/13 17:47:27 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "Operator.hpp"
#include "Rational.hpp"
#include "Complex.hpp"
#include "Matrix.hpp"
#include "Polynomial.hpp"

// Utils
static IType*	find_function(IType *node, std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	std::map<std::pair<std::string, std::string>, const IType*>::iterator	it;
	const Polynomial	*polynomial;
	std::string			var_name;
	std::pair<std::string, std::string>	var_key;

	polynomial = dynamic_cast<const Polynomial*>(node);
	if (!polynomial)
		throw UnexpectedError("Left side of function operator must be a polynomial");
	var_name = to_lower(polynomial->getName());
	var_key.first = var_name;
	for (it = stored.begin(); it != stored.end(); it++)
	{
		if (to_lower(it->first.first) == to_lower(var_key.first) && !it->first.second.empty())
			return (const_cast<IType*>(it->second));
	}
	throw UnexpectedError("Function not found: " + var_name);
	return (nullptr);
}

static IType*	get_result(IType *left_entity, IType *right_entity, 	\
		Operator::t_operator op, std::map<std::pair<std::string, std::string>, const IType*> &stored)
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
			return (find_function(left_entity, stored)->function_operator(*right_entity));
		case Operator::E_UNKNOWN:
			if (dynamic_cast<Matrix*>(left_entity) && dynamic_cast<Matrix*>(right_entity))
				return (left_entity->matrix_operator(*right_entity));
			else
				return (*left_entity * *right_entity);
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
		case Token::E_OPERATOR:
		{
			_node = new Operator(token);
			break;
		}
		case Token::E_NUMBER:
		{
			_node = new Rational(token);
			break;
		}
		case Token::E_IMAGINARY:
		{
			_node = new Complex();
			break;
		}
		case Token::E_MATRIX:
		{
			_node = new Matrix(token, data);
			break;
		}
		case Token::E_POLYNOMIAL:
		{
			_node = new Polynomial(token);
			break;
		}
		default:
			throw UnexpectedError("Invalid token type for AST node");
	}
}

AST::AST(const Token &token, AST *left, AST *right, t_data &data): AST(token, data)
{
	this->_left = left;
	this->_right = right;
}

AST::~AST(void)
{
	if (this->_left)
		delete this->_left;
	if (this->_right)
		delete this->_right;
	if (this->_node)
		delete this->_node;
}


// Operator overloads
AST&	AST::operator=(const AST &other)
{
	if (this != &other)
	{
		delete this;
		_node = other._node->clone();
		_left = other._left;
		_right = other._right;
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
	if (!this->_left || !this->_right)
	{
		if (this->_left || this->_right)
			throw ERROR_OPERATOR_EXPECTED;
		return (true);
	}
	return (false);
}

void			AST::reduce_expression(std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	Operator*	op;
	IType*		result;

	this->replace_variables(stored);
	if (this->end_of_tree())
		return;
	this->_left->reduce_expression(stored);
	this->_right->reduce_expression(stored);
	if (this->end_of_tree())
		return;
	op = dynamic_cast<Operator*>(this->_node);
	if (!op)
		throw ERROR_OPERATOR_EXPECTED;
	try
	{
		result = get_result(this->_left->_node, this->_right->_node, op->getOperator(), stored);
	}
	catch(const std::exception& e)
	{
		delete this;
		throw;
	}
	this->~AST();
	this->_node = result;
	this->_left = nullptr;
	this->_right = nullptr;
}

void			AST::replace_variables(std::map<std::pair<std::string, std::string>, const IType*> &stored)
{
	const Polynomial					*polynomial;
	std::pair<std::string, std::string>	var_key;

	if (this->end_of_tree())
	{
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
		return;
	}
	this->_left->replace_variables(stored);
	this->_right->replace_variables(stored);
}


// Output stream operator overload
std::ostream &operator<<(std::ostream &os, const AST &ast)
{
	return (ast.print(os));
}
