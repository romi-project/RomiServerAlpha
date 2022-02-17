/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   romi.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smun <smun@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 18:06:24 by smun              #+#    #+#             */
/*   Updated: 2022/02/17 13:50:53 by smun             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef ROMI_HPP
#define ROMI_HPP

#include <iostream>
#include <string>
#include <utility>
#include <cstdio>
#include <cstdint>

#if defined(_WIN32) || defined(_WIN64)

# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif

# include <windows.h>
# include <winsock2.h>

#else

# include <netinet/in.h>
# include <sys/event.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <unistd.h>
# include <arpa/inet.h>

#endif

#endif // ROMI_HPP
