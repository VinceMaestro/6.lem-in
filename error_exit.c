/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpetit <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 17:37:52 by vpetit            #+#    #+#             */
/*   Updated: 2018/04/10 17:37:56 by vpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include <unistd.h>
#include <stdlib.h>

void					error_exit(
	const char *const restrict error_code,
	const unsigned long long size)
{
	write(2, error_code, size);
	exit(1);
}
