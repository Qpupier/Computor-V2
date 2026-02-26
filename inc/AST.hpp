/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:01:55 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/26 12:51:33 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_HPP
# define AST_HPP

# include "Token.hpp"
# include "IType.hpp"

class	AST
{
	public:
		// Constructors and destructor
		AST(IType *node): _node(node), _left(nullptr), _right(nullptr) {};
		AST(IType *node, AST *left, AST *right): _node(node), _left(left), _right(right) {};
		// AST(Rational *node): _node(node), _left(nullptr), _right(nullptr) {};
		AST(const Token &token);
		AST(const Token &token, AST *left, AST *right);
		AST(const AST &other): _node(other._node), _left(other._left), _right(other._right) {};
		~AST(void);

		// Operator overloads
		AST& operator=(const AST &other);

		// Getters
		IType*	getNode(void) const;

		// Setters
		void	setLeft(AST *left);
		void	setRight(AST *right);

		// Methods
		std::ostream	&print(std::ostream &os) const;
		void			reduce_expression(void);

	private:
		// Members
		IType*	_node;
		AST*	_left;
		AST*	_right;
};

// Output stream operator overload
std::ostream &operator<<(std::ostream &os, const AST &ast);

AST*	compute_expression(const std::string &line, 						\
		const std::map<std::string, std::regex> &patterns, 					\
		const std::map<const Token::t_token, std::regex> &tokens_types, 	\
		const std::map<std::string, const IType*> &stored);
AST	*build_ast(std::vector<Token> &tokens);


#endif
