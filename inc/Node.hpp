/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Node.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 14:34:41 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/17 18:44:34 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef NODE_HPP
# define NODE_HPP

# include "Token.hpp"

class	Node
{
	public:
		// Constructors and destructor
		Node(void): _tokens({Token()}), _left(nullptr), _right(nullptr) {};
		Node(const std::vector<Token> &tokens): _tokens(tokens), _left(nullptr), _right(nullptr) {};
		Node(const Node &other): _tokens(other._tokens), _left(other._left), _right(other._right) {};
		~Node(void) {};

		// Operator overloads
		Node& operator=(const Node &other);

		// Getters
		std::vector<Token>	getTokens(void) const;
		Node*				getLeft(void) const;
		Node*				getRight(void) const;

		// Setters
		void	setLeft(Node* left);
		void	setRight(Node* right);

	private:
		// Members
		std::vector<Token>	_tokens;
		Node*	_left;
		Node*	_right;
};

#endif
