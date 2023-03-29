/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 21:18:13 by aoumad            #+#    #+#             */
/*   Updated: 2023/03/28 21:42:41 by aoumad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

int ft_check_request_line(std::string method, std::string uri, std::string version)
{
    if (method != "GET" && method != "POST" && method != "HEAD" && method != "PUT")
    {
        if (method == "DELETE" || method == "OPTIONS" || method == "TRACE")
            std::cerr << "Method not implemented" << std::endl;
        return (0);
    }
    if (uri[0] != '/')
        return (0);
    if (version != "HTTP/1.1")
        return (0);
    return (1);
}