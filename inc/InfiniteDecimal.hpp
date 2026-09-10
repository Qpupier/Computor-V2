/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfiniteDecimal.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:38:24 by qpupier           #+#    #+#             */
/*   Updated: 2026/08/31 17:28:34 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef INFINITEDECIMAL_HPP
# define INFINITEDECIMAL_HPP

# include "InfiniteInt.hpp"

class	InfiniteDecimal
{
	public:
		// Constructors and destructor
		InfiniteDecimal(const InfiniteInt integer_part, 					\
				const InfiniteInt decimal_part, const bool is_negative = false);
		InfiniteDecimal(const InfiniteInt integer): 						\
				InfiniteDecimal(integer, InfiniteInt(), 					\
					integer.getIsNegative()) {};
		InfiniteDecimal(void): InfiniteDecimal(InfiniteInt()) {};
		InfiniteDecimal(const std::vector<unsigned char> integer_digits, 	\
				const std::vector<unsigned char> decimal_digits 		\
					= std::vector<unsigned char>(), 					\
				bool is_negative = false): 								\
					InfiniteDecimal(InfiniteInt(integer_digits), 			\
						InfiniteInt(decimal_digits, false, false), 		\
						is_negative) {};
		InfiniteDecimal(const long long int value, 						\
				bool is_integer_part = true): 							\
					InfiniteDecimal(InfiniteInt(value, is_integer_part)) {};
		InfiniteDecimal(const InfiniteDecimal &other): 						\
				InfiniteDecimal(other.getIntegerPart(), 					\
				other.getDecimalPart(), other.getIsNegative()) {};
		InfiniteDecimal(const std::string integer_part, 					\
				const std::string decimal_part = "0", 					\
				bool is_negative = false): 								\
					InfiniteDecimal(InfiniteInt(integer_part), 			\
						InfiniteInt(decimal_part, false, false), 		\
						is_negative) {};
		~InfiniteDecimal(void) {};

		// Operator overloads
		explicit			operator bool(void) const;
		InfiniteDecimal&		operator=(const InfiniteDecimal &other);
		InfiniteDecimal&		operator=(const InfiniteInt &other);
		InfiniteDecimal&		operator=(const long long int value);
		bool				operator==(const InfiniteDecimal &other) const;
		bool				operator==(const InfiniteInt &other) const;
		bool				operator==(const long long int value) const;
		bool				operator!=(const InfiniteDecimal&other) const;
		bool				operator!=(const InfiniteInt&other) const;
		bool				operator!=(const long long int value) const;
		bool				operator<(const InfiniteDecimal &other) const;
		bool				operator<(const InfiniteInt &other) const;
		bool				operator<(const long long int value) const;
		bool				operator<=(const InfiniteDecimal &other) const;
		bool				operator<=(const InfiniteInt &other) const;
		bool				operator<=(const long long int value) const;
		bool				operator>(const InfiniteDecimal &other) const;
		bool				operator>(const InfiniteInt &other) const;
		bool				operator>(const long long int value) const;
		bool				operator>=(const InfiniteDecimal &other) const;
		bool				operator>=(const InfiniteInt &other) const;
		bool				operator>=(const long long int value) const;
		InfiniteDecimal		operator+(const InfiniteDecimal &other) const;
		InfiniteDecimal		operator+(const InfiniteInt &other) const;
		InfiniteDecimal		operator+(const long long int value) const;
		// IType*				operator+(const IType &other) const;
		void				operator+=(const InfiniteDecimal &other);
		void				operator+=(const InfiniteInt &other);
		void				operator+=(const long long int value);
		InfiniteDecimal&		operator++(void);
		InfiniteDecimal		operator++(int);
		InfiniteDecimal		operator-(void) const;
		InfiniteDecimal		operator-(const InfiniteDecimal &other) const;
		InfiniteDecimal		operator-(const InfiniteInt &other) const;
		InfiniteDecimal		operator-(const long long int value) const;
		void				operator-=(const InfiniteDecimal &other);
		void				operator-=(const InfiniteInt &other);
		void				operator-=(const long long int value);
		InfiniteDecimal&		operator--(void);
		InfiniteDecimal		operator--(int);
		InfiniteDecimal		operator*(const InfiniteDecimal &other) const;
		InfiniteDecimal		operator*(const InfiniteInt &other) const;
		InfiniteDecimal		operator*(const long long int value) const;
		void				operator*=(const InfiniteDecimal &other);
		void				operator*=(const InfiniteInt &other);
		void				operator*=(const long long int value);
		InfiniteDecimal		operator/(const InfiniteDecimal &other) const;
		InfiniteDecimal		operator/(const InfiniteInt &other) const;
		InfiniteDecimal		operator/(const long long int value) const;
		void				operator/=(const InfiniteDecimal &other);
		void				operator/=(const InfiniteInt &other);
		void				operator/=(const long long int value);
		InfiniteDecimal		operator%(const InfiniteDecimal &other) const;
		InfiniteDecimal		operator%(const InfiniteInt &other) const;
		InfiniteDecimal		operator%(const long long int value) const;
		void				operator%=(const InfiniteDecimal &other);
		void				operator%=(const InfiniteInt &other);
		void				operator%=(const long long int value);
		InfiniteDecimal		operator^(const InfiniteDecimal &other) const;
		InfiniteDecimal		operator^(const InfiniteInt &other) const;
		InfiniteDecimal		operator^(const long long int value) const;
		void				operator^=(const InfiniteDecimal &other);
		void				operator^=(const InfiniteInt &other);
		void				operator^=(const long long int value);

		// Getters
		InfiniteInt	getDecimalPart(void) const;
		InfiniteInt	getIntegerPart(void) const;
		bool		getIsNegative(void) const;

		// Setters
		void	setDecimalPart(const InfiniteInt &decimal_part);
		void	setIntegerPart(const InfiniteInt &integer_part);
		void	setIsNegative(bool is_negative);

		// Methods
		InfiniteDecimal	abs(void) const;
		InfiniteDecimal	sqrt(void) const;
		int				to_int(void) const;
		bool			in_D(void) const;
		void			push_back_decimal(unsigned char digit);
		void			push_back_integer(unsigned char digit);
		void			reduce(void);

		//Constants
		static constexpr unsigned char	MAX_PRECISION = 32;
		static constexpr unsigned char	CALCULATION_PRECISION = 16;
		static constexpr unsigned char	PRINT_PRECISION = 8;

	private:
		// Members
		InfiniteInt	_integer_part;
		InfiniteInt	_decimal_part;
		bool		_isNegative;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const InfiniteDecimal &num);

#endif
