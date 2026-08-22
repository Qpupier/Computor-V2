/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 18:01:55 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/22 14:41:33 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_HPP
# define AST_HPP

# include <stdio.h>
# include "computor-v2.hpp"
# include "IType.hpp"
# include "backtracking_possibilities.hpp"

class	AST
{
	public:
		// Constructors and destructor
		AST(IType *node): _node(node), _left(nullptr), _right(nullptr) {};
		AST(IType *node, AST *left, AST *right): _node(node), _left(left), _right(right) {};
		AST(const Token &token, t_data &data);
		AST(const Token &token, AST *left, AST *right, t_data &data);
		AST(const AST &other): _node(other._node), _left(other._left), _right(other._right) {};
		~AST(void);

		// Operator overloads
		AST& operator=(const AST &other);

		// Getters
		IType*	getNode(void) const;
		AST*	getLeft(void) const;
		AST*	getRight(void) const;

		// Setters
		void	setLeft(AST *left);
		void	setRight(AST *right);

		// Methods
		std::ostream&	print(std::ostream &os) const;
		bool			end_of_tree(void) const;
		void			free(void);
		void			reduce_expression(std::map<std::pair<std::string, std::string>, const IType*> &stored);
		void			replace_variables(std::map<std::pair<std::string, std::string>, const IType*> &stored);

	private:
		// Members
		IType*	_node;
		AST*	_left;
		AST*	_right;
};

// Output stream operator overload
std::ostream &operator<<(std::ostream &os, const AST &ast);

// Functions
unsigned char	read_line(bool is_interactive, std::string& str_line, t_data& data);
bool			set_function_left(const std::vector<Token>& tokens, std::map<std::pair<std::string, std::string>, const IType*> &stored);
AST*			build_ast(t_possibility tokens, t_data &data);
AST*			compute_expression(const std::string &line, t_data &data, bool is_right_side, const bool eval = true);
AST*			get_the_only_possibility(std::vector<t_possibility>& possibilities, t_data &data, bool is_right_side, const bool eval);
AST*			handle_external_brackets(t_possibility& possibility, t_bracket brackets_type, t_data& data);
void			clean_tokens(t_possibility& possibility);
void			compute_equation(const std::string &line, t_data &data, const bool eval);
void			delete_empty_function_stored(std::map<std::pair<std::string, std::string>, const IType*> &stored, const std::string error_msg, const bool throw_error = false);
void			history(std::smatch match, const std::vector<std::string>& history_results);
void			print_expression(const std::string &line, t_data &data);
void			print_variables(const t_data& data, const bool print_preset, const bool print_variables);
void			set_function_right(t_data &data, AST* ast);
void			stop_preset_terms(const std::string& name, bool change = true);
void			stored_variables(const t_data& data);
void			trim_string(std::string &s);
void			verif_preset_terms(AST* ast, bool left = true, bool last_operator_is_function = false);

#endif
