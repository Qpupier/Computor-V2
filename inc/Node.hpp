/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Node.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 14:34:41 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/19 13:27:30 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef NODE_HPP
# define NODE_HPP

# include "IType.hpp"
# include "Token.hpp"

class	Node
{
	public:
		// Constructors and destructor
		Node(std::string value, Token::t_token type): _token(value, type), _left(nullptr), _right(nullptr) {};
		Node(const Token &token): _token(token), _left(nullptr), _right(nullptr) {};
		Node(const Node &other): _token(other._token), _left(other._left), _right(other._right) {};
		~Node(void) {};

		// Operator overloads
		Node& operator=(const Node &other);
		// Node* operator*(const Node &other) const {return new Node(*this);(void)other;};
		// Node* operator+(const Node &other) const {return new Node(*this);(void)other;};
		// Node* operator-(const Node &other) const {return new Node(*this);(void)other;};
		// Node* operator/(const Node &other) const {return new Node(*this);(void)other;};
		// // Node* operator**(const Node &other) const {return new Node(*this);(void)other;};
		// Node* operator%(const Node &other) const {return new Node(*this);(void)other;};
		// Node* operator^(const Node &other) const {return new Node(*this);(void)other;};

		// Getters
		Token	getToken(void) const;
		Node*	getLeft(void) const;
		Node*	getRight(void) const;

		// Setters
		void	setLeft(Node* left);
		void	setRight(Node* right);

	private:
		// Members
		Token	_token;
		Node*	_left;
		Node*	_right;
};

#endif
