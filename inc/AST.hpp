/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 14:34:41 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/16 19:46:56 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_HPP
# define AST_HPP

# include "computor-v2.hpp"

class Token;

class Node
{
	public:
		Node(Token &token): _token(token), _left(nullptr), _right(nullptr) {};
		~Node() {};
	
		Token&		getToken() const { return _token; }
		Node*		getLeft() const { return _left; }
		Node*		getRight() const { return _right; }
		void setLeft(Node* left) { _left = left; }
		void setRight(Node* right) { _right = right; }

	private:
		Token&		_token;
		Node*		_left;
		Node*		_right;
};

#endif
