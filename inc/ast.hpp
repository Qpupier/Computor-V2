/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:51:38 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/17 19:12:22 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_HPP
# define AST_HPP

# include "Node.hpp"
# include "Type.hpp"

void	free_ast(Node *ast);
Node	*compute_expression(const std::string &line, 						\
		const std::map<std::string, std::regex> &patterns, 					\
		const std::map<const Token::t_token, std::regex> &tokens_types, 	\
		const std::map<std::string, Type> &stored);
Node	*make_ast(std::vector<Token> &tokens);
Node	*reduce_expression(Node *ast);

#endif
