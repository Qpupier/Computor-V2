/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfiniteInt.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:39:49 by qpupier           #+#    #+#             */
/*   Updated: 2026/05/22 12:15:25 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef INFINITEINT_HPP
# define INFINITEINT_HPP

# include "computor-v2.hpp"

class	InfiniteDouble;

class	InfiniteInt
{
	public:
		// Constructors and destructor
		InfiniteInt(const std::vector<unsigned char> digits, 	\
				bool is_negative = false, bool is_integer_part = true);
		InfiniteInt(void): InfiniteInt(std::vector<unsigned char>()) {};
		InfiniteInt(const InfiniteInt &other): 					\
				InfiniteInt(other.getDigits(), other.getIsNegative(), other.getIsIntegerPart()) {};
		InfiniteInt(const std::string str, bool is_negative = false, bool is_integer_part = true);
		InfiniteInt(const long long int value, bool is_integer_part = true): 				\
				InfiniteInt(std::to_string(std::llabs(value)), value < 0, is_integer_part) {};
		~InfiniteInt(void) {};

		// Operator overloads
		explicit			operator bool(void) const;
		InfiniteInt&		operator=(const InfiniteInt &other);
		InfiniteInt&		operator=(const long long int value);
		unsigned char		operator[](	\
				std::vector<unsigned char>::size_type index) const;
		unsigned char		operator[](	\
				std::vector<unsigned char>::size_type index);
		bool				operator==(const InfiniteInt &other) const;
		bool				operator==(const InfiniteDouble &other) const;
		bool				operator==(const long long int value) const;
		bool				operator!=(const InfiniteInt&other) const;
		bool				operator!=(const InfiniteDouble&other) const;
		bool				operator!=(const long long int value) const;
		bool				operator<(const InfiniteInt &other) const;
		bool				operator<(const InfiniteDouble &other) const;
		bool				operator<(const long long int value) const;
		bool				operator<=(const InfiniteInt &other) const;
		bool				operator<=(const InfiniteDouble &other) const;
		bool				operator<=(const long long int value) const;
		bool				operator>(const InfiniteInt &other) const;
		bool				operator>(const InfiniteDouble &other) const;
		bool				operator>(const long long int value) const;
		bool				operator>=(const InfiniteInt &other) const;
		bool				operator>=(const InfiniteDouble &other) const;
		bool				operator>=(const long long int value) const;
		InfiniteInt			operator+(const InfiniteInt &other) const;
		InfiniteDouble		operator+(const InfiniteDouble &other) const;
		InfiniteInt			operator+(const long long int value) const;
		void				operator+=(const InfiniteInt &other);
		void				operator+=(const long long int value);
		InfiniteInt&		operator++(void);
		InfiniteInt			operator++(int);
		InfiniteInt			operator-(void) const;
		InfiniteInt			operator-(const InfiniteInt &other) const;
		InfiniteDouble		operator-(const InfiniteDouble &other) const;
		InfiniteInt			operator-(const long long int value) const;
		void				operator-=(const InfiniteInt &other);
		void				operator-=(const long long int value);
		InfiniteInt&		operator--(void);
		InfiniteInt			operator--(int);
		InfiniteInt			operator*(const InfiniteInt &other) const;
		InfiniteDouble		operator*(const InfiniteDouble &other) const;
		InfiniteInt			operator*(const long long int value) const;
		void				operator*=(const InfiniteInt &other);
		void				operator*=(const long long int value);
		InfiniteInt			operator/(const InfiniteInt &other) const;
		InfiniteDouble		operator/(const InfiniteDouble &other) const;
		InfiniteInt			operator/(const long long int value) const;
		void				operator/=(const InfiniteInt &other);
		void				operator/=(const long long int value);
		InfiniteInt			operator%(const InfiniteInt &other) const;
		InfiniteDouble		operator%(const InfiniteDouble &other) const;
		InfiniteInt			operator%(const long long int value) const;
		void				operator%=(const InfiniteInt &other);
		void				operator%=(const long long int value);
		InfiniteInt			operator^(const InfiniteInt &other) const;
		InfiniteDouble		operator^(const InfiniteDouble &other) const;
		InfiniteInt			operator^(const long long int value) const;
		void				operator^=(const InfiniteInt &other);
		void				operator^=(const long long int value);

		// Getters
		std::vector<unsigned char>	getDigits(void) const;
		bool						getIsIntegerPart(void) const;
		bool						getIsNegative(void) const;

		// Setters
		void	setDigits(const std::vector<unsigned char> &digits);
		void	setIsIntegerPart(bool is_integer_part);
		void	setIsNegative(bool is_negative);

		// Methods
		InfiniteInt	sqrt(void) const;
		std::string	to_string(void) const;
		std::size_t	size(void) const;
		void		push_back(unsigned char digit);
		void		reverse(void);
		void		reduce(void);

	private:
		// Members
		std::vector<unsigned char>	_digits;
		bool						_isIntegerPart;
		bool						_isNegative;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const InfiniteInt &num);

# include "InfiniteDouble.hpp"

#endif
