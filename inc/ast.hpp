/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:51:38 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/19 15:00:50 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_HPP
# define AST_HPP

# include "Node.hpp"
# include "Token.hpp"

void	free_ast(Node *ast);
void	print_ast(Node *ast, unsigned int depth);
Node	*compute_expression(const std::string &line, 						\
		const std::map<std::string, std::regex> &patterns, 					\
		const std::map<const Token::t_token, std::regex> &tokens_types, 	\
		const std::map<std::string, const IType*> &stored);
Node	*make_ast(std::vector<Token> &tokens);
Node	*reduce_expression(Node *ast);

#endif
