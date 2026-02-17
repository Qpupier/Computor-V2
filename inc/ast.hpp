/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:51:38 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/17 16:53:43 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_HPP
# define AST_HPP

# include "Node.hpp"

void	free_ast(Node *ast);
Node	*compute_expression(const std::string &line, const std::map<std::string, std::regex> &patterns, const std::map<const Token::t_token, std::regex> &tokens_types);
Node	*make_ast(std::vector<Token> &tokens);

#endif
