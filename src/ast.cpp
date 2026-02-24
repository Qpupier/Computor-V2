/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:48:49 by qpupier           #+#    #+#             */
/*   Updated: 2026/02/24 18:33:44 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

void	free_ast(Node *ast)
{
	if (!ast)
		return;
	// std::cout << "Freeing node with token: " << ast->getToken().getValue() << std::endl;// Debug
	free_ast(ast->getLeft());
	free_ast(ast->getRight());
	delete ast;
}

void	print_ast(Node *ast, unsigned int depth)
{
	if (!ast)
		return;
	// std::cout << std::string(depth * 2, ' ') << ast->getToken().getValue() << std::endl;
	print_ast(ast->getLeft(), depth + 1);
	std::cout << ast->getToken().getValue();
	print_ast(ast->getRight(), depth + 1);
}

std::vector<Token>	find_next_parenthesis_group(const std::vector<Token> &tokens)
{
	std::vector<Token>::const_iterator	start;
	unsigned int						depth;

	depth = 0;
	for (unsigned long int i = 0; i < tokens.size(); i++)
		if (tokens[i].getType() == Token::E_LEFT_PARENTHESIS)
		{
			if (!depth)
				start = tokens.begin() + static_cast<long>(i) + 1;
			depth++;
		}
		else if (tokens[i].getType() == Token::E_RIGHT_PARENTHESIS)
		{
			depth--;
			if (!depth)
				return (std::vector<Token>(start, tokens.begin() + static_cast<long>(i)));
		}
	if (depth)
		throw std::logic_error("Mismatched parentheses");
	return (std::vector<Token>());
}

static long int	select_operator(const std::vector<Token> &tokens, unsigned long int size, const std::vector<std::string> &operators)
{
	unsigned int	depth;

	depth = 0;
	for (size_t i = 0; i < size; i++)
	{
		unsigned long int	pos;

		pos = size - i - 1;
		if (!depth)
			for (const std::string &op: operators)
				if (tokens[pos].getValue() == op)
					return (static_cast<long int>(pos));
		if (tokens[pos].getType() == Token::E_RIGHT_PARENTHESIS)
			depth++;
		else if (tokens[pos].getType() == Token::E_LEFT_PARENTHESIS)
			depth--;
	}
	return (-1);
}

static long int	select_less_priority_operator(const std::vector<Token> &tokens)
{
	unsigned long int	size;
	long int			pos;

	size = tokens.size();
	pos = select_operator(tokens, size, {"-", "+"});
	if (pos != -1)
		return (pos);
	pos = select_operator(tokens, size, {"*", "/", "%"});
	if (pos != -1)
		return (pos);
	pos = select_operator(tokens, size, {"**", "^", "***"});
	if (pos != -1)
	{
		if (tokens[static_cast<unsigned long int>(pos)].getValue() == "***")
			std::cout << "Found *** operator at position " << pos << std::endl;
		return (pos);
	}
	// std::cout << "No operator found" << std::endl;
	// throw std::logic_error("No operator found in the expression");
	return (-1);
}

static bool	can_remove_external_parenthesis(const std::vector<Token> &tokens)
{
	int	depth;

	depth = 0;
	for (const Token &token : tokens)
	{
		if (token.getType() == Token::E_LEFT_PARENTHESIS)
			depth++;
		else if (token.getType() == Token::E_RIGHT_PARENTHESIS)
		{
			depth--;
			if (depth < 0)
				return (false);
		}
	}
	return (!depth);
}

static std::vector<Token>	*adapt_tokens(std::vector<Token> &tokens, std::vector<Token> &sub_tokens, long int *pos)
{
	if (tokens[0].getType() == Token::E_LEFT_PARENTHESIS 							\
			&& tokens[tokens.size() - 1].getType() == Token::E_RIGHT_PARENTHESIS 	\
			&& can_remove_external_parenthesis(sub_tokens))
		return (&sub_tokens);
	*pos = select_less_priority_operator(tokens);
	if (*pos < 0)
		return (nullptr);
	if (!*pos || static_cast<unsigned long int>(*pos) == tokens.size() - 1)
	{
		if (!*pos && (tokens[0].getValue() == "-" || tokens[0].getValue() == "+"))
		{
			if (tokens[0].getValue() == "-")
			{
				tokens[0].setValue(tokens[0].getValue() + "1");
				tokens[0].setType(Token::E_NUMBER);
				tokens.insert(tokens.begin() + 1, Token("*", Token::E_OPERATOR));
			}
			else
				tokens.erase(tokens.begin());
			return (&tokens);
		}
		throw std::logic_error("Operator cannot be at the beginning or end of an expression");
	}
	return (nullptr);
}

// Node	*make_ast(std::vector<Token> &tokens)
// {
// 	std::vector<Token>::const_iterator	tokens_begin;
// 	std::vector<Token>::const_iterator	tokens_end;
// 	std::vector<Token>::const_iterator	tokens_operator;
// 	std::vector<Token>					sub_tokens;
// 	std::vector<Token>					*adapted_tokens;
// 	Node 								*node;
// 	long int							pos;

// 	// std::cout << "Group: "; for (size_t i = 0; i < tokens.size(); i++) std::cout << "\033[30m[\033[32m" << tokens[i].getValue() << "\033[30m]\033[0m"; std::cout << std::endl;// Debug
// 	tokens_begin = tokens.begin();
// 	tokens_end = tokens.end();
// 	if (tokens.empty())
// 		return (nullptr);
// 	if (tokens.size() == 1)
// 		return (new Node(tokens[0]));
// 	sub_tokens = std::vector<Token>(tokens_begin + 1, tokens_end - 1);
// 	adapted_tokens = adapt_tokens(tokens, sub_tokens, &pos);
// 	if (adapted_tokens)
// 		return (make_ast(*adapted_tokens));
// 	if (pos == -1)
// 	{
// 		if (tokens.size() != 2)
// 			throw std::logic_error("Invalid expression: no operator found in a multi-token expression");
// 		return (new Node(Token(tokens[0].getValue() + tokens[1].getValue(), Token::E_FUNCTION)));
// 	}
// 	// std::cout << "Operator: " << tokens[static_cast<unsigned long int>(pos)].getValue() << std::endl;// Debug
// 	tokens_operator = tokens_begin + pos;
// 	node = new Node(tokens[static_cast<unsigned long int>(pos)]);
// 	std::vector<Token> left_tokens(tokens_begin, tokens_operator);
// 	std::vector<Token> right_tokens(tokens_operator + 1, tokens_end);
// 	node->setLeft(make_ast(left_tokens));
// 	node->setRight(make_ast(right_tokens));
// 	return (node);
// }


AST	*build_ast(std::vector<Token> &tokens)
{
	std::vector<Token>::const_iterator	tokens_begin;
	std::vector<Token>::const_iterator	tokens_end;
	std::vector<Token>::const_iterator	tokens_operator;
	std::vector<Token>					sub_tokens;
	std::vector<Token>					*adapted_tokens;
	std::vector<Token>					left_tokens;
	std::vector<Token>					right_tokens;
	AST 								*node;
	long int							pos;

	// std::cout << "Group: "; for (size_t i = 0; i < tokens.size(); i++) std::cout << "\033[30m[\033[32m" << tokens[i].getValue() << "\033[30m]\033[0m"; std::cout << std::endl;// Debug
	tokens_begin = tokens.begin();
	tokens_end = tokens.end();
	if (tokens.empty())
		return (nullptr);
	if (tokens.size() == 1)
		return (new AST(tokens[0]));
	sub_tokens = std::vector<Token>(tokens_begin + 1, tokens_end - 1);
	adapted_tokens = adapt_tokens(tokens, sub_tokens, &pos);
	if (adapted_tokens)
		return (build_ast(*adapted_tokens));
	if (pos == -1)
	{
		if (tokens.size() != 2)
			throw std::logic_error("Invalid expression: no operator found in a multi-token expression");
		return (new AST(Token(tokens[0].getValue() + tokens[1].getValue(), Token::E_FUNCTION)));
	}
	// std::cout << "Operator: " << tokens[static_cast<unsigned long int>(pos)].getValue() << std::endl;// Debug
	tokens_operator = tokens_begin + pos;
	left_tokens = std::vector<Token>(tokens_begin, tokens_operator);
	right_tokens = std::vector<Token>(tokens_operator + 1, tokens_end);
	return (new AST(tokens[static_cast<unsigned long int>(pos)], build_ast(left_tokens), build_ast(right_tokens)));
}


// AST*	Token_to_IType(Node *node)
// {
// 	if (!node)
// 		return (nullptr);
// 	if (node->getToken().getType() == Token::E_NUMBER)
// 		return (new AST(new Number(node)));
// 	if (node->getToken().getType() == Token::E_IMAGINARY)
// 		return (new AST(new Imaginary()));
// 	// if (node->getToken().getType() == Token::E_VARIABLE)
// 	// 	return (new AST(new Variable(node)));
// 	return (nullptr);
// }

// AST*	build_ast(const std::vector<Token> &tokens)
// {
// 	std::vector<Token>	tokens_copy(tokens);
// 	Node				*root;

// 	root = make_ast(tokens_copy);
	
// }
