/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   romi.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smun <smun@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 18:06:24 by smun              #+#    #+#             */
/*   Updated: 2022/02/12 21:27:12 by smun             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef ROMI_HPP
#define ROMI_HPP

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdarg>
#include <cstdio>
#include <arpa/inet.h>

void    Hello();

#endif // ROMI_HPP
