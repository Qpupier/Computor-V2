/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InfiniteDouble.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:38:24 by qpupier           #+#    #+#             */
/*   Updated: 2026/06/26 17:02:26 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef INFINITEDOUBLE_HPP
# define INFINITEDOUBLE_HPP

# include "InfiniteInt.hpp"

class	InfiniteDouble
{
	public:
		// Constructors and destructor
		InfiniteDouble(const InfiniteInt integer_part, 						\
				const InfiniteInt decimal_part = InfiniteInt(0, false), 	\
				bool is_negative = false);
		InfiniteDouble(void): InfiniteDouble(InfiniteInt()) {};
		InfiniteDouble(const std::vector<unsigned char> integer_digits, 	\
				const std::vector<unsigned char> decimal_digits 			\
					= std::vector<unsigned char>(), 						\
				bool is_negative = false): 									\
					InfiniteDouble(InfiniteInt(integer_digits), 			\
						InfiniteInt(decimal_digits, false, false), 			\
						is_negative) {};
		InfiniteDouble(const long long int value, 							\
				bool is_integer_part = true): 								\
					InfiniteDouble(InfiniteInt(value, is_integer_part)) {};
		InfiniteDouble(const InfiniteDouble &other): 						\
				InfiniteDouble(other.getIntegerPart(), 						\
				other.getDecimalPart(), other.getIsNegative()) {};
		InfiniteDouble(const std::string integer_part, 						\
				const std::string decimal_part = "0", 						\
				bool is_negative = false): 									\
					InfiniteDouble(InfiniteInt(integer_part, false), 		\
						InfiniteInt(decimal_part, false, false), 			\
						is_negative) {};
		~InfiniteDouble(void) {};

		// Operator overloads
		explicit			operator bool(void) const;
		InfiniteDouble&		operator=(const InfiniteDouble &other);
		InfiniteDouble&		operator=(const InfiniteInt &other);
		InfiniteDouble&		operator=(const long long int value);
		bool				operator==(const InfiniteDouble &other) const;
		bool				operator==(const InfiniteInt &other) const;
		bool				operator==(const long long int value) const;
		bool				operator!=(const InfiniteDouble&other) const;
		bool				operator!=(const InfiniteInt&other) const;
		bool				operator!=(const long long int value) const;
		bool				operator<(const InfiniteDouble &other) const;
		bool				operator<(const InfiniteInt &other) const;
		bool				operator<(const long long int value) const;
		bool				operator<=(const InfiniteDouble &other) const;
		bool				operator<=(const InfiniteInt &other) const;
		bool				operator<=(const long long int value) const;
		bool				operator>(const InfiniteDouble &other) const;
		bool				operator>(const InfiniteInt &other) const;
		bool				operator>(const long long int value) const;
		bool				operator>=(const InfiniteDouble &other) const;
		bool				operator>=(const InfiniteInt &other) const;
		bool				operator>=(const long long int value) const;
		InfiniteDouble		operator+(const InfiniteDouble &other) const;
		InfiniteDouble		operator+(const InfiniteInt &other) const;
		InfiniteDouble		operator+(const long long int value) const;
		IType*				operator+(const IType &other) const;
		void				operator+=(const InfiniteDouble &other);
		void				operator+=(const InfiniteInt &other);
		void				operator+=(const long long int value);
		InfiniteDouble&		operator++(void);
		InfiniteDouble		operator++(int);
		InfiniteDouble		operator-(void) const;
		InfiniteDouble		operator-(const InfiniteDouble &other) const;
		InfiniteDouble		operator-(const InfiniteInt &other) const;
		InfiniteDouble		operator-(const long long int value) const;
		void				operator-=(const InfiniteDouble &other);
		void				operator-=(const InfiniteInt &other);
		void				operator-=(const long long int value);
		InfiniteDouble&		operator--(void);
		InfiniteDouble		operator--(int);
		InfiniteDouble		operator*(const InfiniteDouble &other) const;
		InfiniteDouble		operator*(const InfiniteInt &other) const;
		InfiniteDouble		operator*(const long long int value) const;
		void				operator*=(const InfiniteDouble &other);
		void				operator*=(const InfiniteInt &other);
		void				operator*=(const long long int value);
		InfiniteDouble		operator/(const InfiniteDouble &other) const;
		InfiniteDouble		operator/(const InfiniteInt &other) const;
		InfiniteDouble		operator/(const long long int value) const;
		void				operator/=(const InfiniteDouble &other);
		void				operator/=(const InfiniteInt &other);
		void				operator/=(const long long int value);
		InfiniteDouble		operator%(const InfiniteDouble &other) const;
		InfiniteDouble		operator%(const InfiniteInt &other) const;
		InfiniteDouble		operator%(const long long int value) const;
		void				operator%=(const InfiniteDouble &other);
		void				operator%=(const InfiniteInt &other);
		void				operator%=(const long long int value);
		InfiniteDouble		operator^(const InfiniteDouble &other) const;
		InfiniteDouble		operator^(const InfiniteInt &other) const;
		InfiniteDouble		operator^(const long long int value) const;
		void				operator^=(const InfiniteDouble &other);
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
		InfiniteDouble	sqrt(void) const;
		bool			in_D(void) const;
		void			push_back_decimal(unsigned char digit);
		void			push_back_integer(unsigned char digit);
		void			reduce(void);

		//Constants
		static constexpr unsigned char	MAX_PRECISION = 32;
		static constexpr unsigned char	PRECISION = 8;

	private:
		// Members
		InfiniteInt	_integer_part;
		InfiniteInt	_decimal_part;
		bool		_isNegative;
};

// Output stream operator overload
std::ostream&	operator<<(std::ostream &os, const InfiniteDouble &num);

#endif
