/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:01:55 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/24 18:31:00 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_HPP
# define AST_HPP

# include "Node.hpp"
# include "Token.hpp"
# include "IType.hpp"

class	AST
{
	public:
		// Constructors and destructor
		AST(void): _node(nullptr), _left(nullptr), _right(nullptr) {};
		AST(IType *node): _node(node), _left(nullptr), _right(nullptr) {};
		AST(IType *node, AST *left, AST *right): _node(node), _left(left), _right(right) {};
		AST(const Token &token);
		AST(const AST &other): _node(other._node), _left(other._left), _right(other._right) {};
		~AST(void) {};

		// Operator overloads
		AST& operator=(const AST &other);

		// Methods
		// static void	free_ast(Node *ast);
		// static void	print_ast(Node *ast, unsigned int depth = 0);

	private:
		// Members
		IType	*_node;
		AST		*_left;
		AST		*_right;
};

void	free_ast(Node *ast);
void	print_ast(Node *ast, unsigned int depth);
Node	*compute_expression(const std::string &line, 						\
		const std::map<std::string, std::regex> &patterns, 					\
		const std::map<const Token::t_token, std::regex> &tokens_types, 	\
		const std::map<std::string, const IType*> &stored);
Node	*make_ast(std::vector<Token> &tokens);
Node	*reduce_expression(Node *ast);

#endif
