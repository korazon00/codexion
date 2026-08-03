/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_of_coder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faeljedd <faeljedd@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-08-03 17:29:03 by faeljedd          #+#    #+#             */
/*   Updated: 2026-08-03 17:29:03 by faeljedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	life_of_coder(t_sim *sim, t_coder *coder)
{
	compiling(sim, coder);
	release_dongles(coder);
	debuging(sim, coder);
	refactoring(sim, coder);
}