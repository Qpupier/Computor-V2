/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphic.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpupier <qpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 18:51:44 by qpupier           #+#    #+#             */
/*   Updated: 2026/09/10 19:00:10 by qpupier          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHIC_HPP
# define GRAPHIC_HPP

# include "Polynomial.hpp"

# define GRAPHIC_WIDTH	80
# define GRAPHIC_HEIGHT	24
# define GRAPHIC_X_MIN	-10
# define GRAPHIC_X_MAX	10
# define GRAPHIC_X_STEP	"0.5"

std::ostream&	display_graphic(std::ostream& os, 	\
		std::vector<std::pair<Rational, Rational>> points);

#endif
