/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 14:34:41 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/13 17:57:31 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_HPP
# define AST_HPP

# include "computor-v2.hpp"

class Node
{
	public:
		Node(std::string value): _value(std::move(value)), _left(nullptr), _right(nullptr) {};
		~Node() {};

		std::string	getValue() const { return _value; }
		Node*		getLeft() const { return _left; }
		Node*		getRight() const { return _right; }
		void setLeft(Node* left) { _left = left; }
		void setRight(Node* right) { _right = right; }

	private:
		std::string _value;
		Node*		_left;
		Node*		_right;
};

#endif
