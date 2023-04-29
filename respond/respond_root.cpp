/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond_root.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 14:53:31 by aoumad            #+#    #+#             */
/*   Updated: 2023/04/29 17:38:55 by aoumad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "respond.hpp"

void    Respond::response_root()
{
    // step 1 :check the location
    ft_parse_location();

    // step 2 : check the redirectation
    ft_parse_url_forwarding();

    // step 3 : check the validation of rooted path
    ft_parse_root_path();
    
    // step 4 : check the allowed methods
    ft_check_allowed_methods();

    // step 5 : check the autoindex
    ft_check_autoindex();

    // methods area
    if (r.get_method() == "Get")
        handle_get_response();
    else if (r.get_method() == "Post")
        handle_post_response();
    else if (r.get_method() == "Delete")
        handle_delete_response();
    else // unsupported http method
        handle_error_response(405);
    
    // set the response
    set_response_body();
}

void  Respond::ft_parse_location()
{
    // exact location body code
    std::string path = r.get_uri();
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; j < server[i]._location.size(); j++)
        {
            if (server[i]._location[j].location_name == path)
            {
                _path_found = server[i]._location[j].location_name;
            }
        }
    }

    // prefix location body code
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; j < server[i]._location.size(); j++)
        {
            if (path.find(server[i]._location[j].location_name) == 0)
            {
                _path_found = server[i]._location[j].location_name;
            }
        }
    }

    // regex location body code
    std::string::size_type pos = path.find(".");
    if (pos != std::string::npos)
    {
        std::string extension = ".*" + path.substr(pos);
        for (int i = 0; i < server.size(); i++)
        {
            for (int j = 0; j < server[i]._location.size(); j++)
            {
                if (server[i]._location[j].location_name == extension)
                {
                    _path_found = server[i]._location[j].location_name;
                    _is_cgi = true;
                }
            }
        }
    }

    // root location
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; j < server._location.size(); j++)
        {
            if (server[i]._location[j].location_name == "/")
            {
                _path_found = server[i]._location[j].location_name;
            }
        }
    }
}

void    Respond::ft_parse_url_forwarding()
{
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0;  j < server._location.size(); j++)
        {
            if (_path_found == server[i]._location[j].location_name)
            {
                // check for redirection ===== where redirection is make_pair
                if (!server[i]._location[j].redirection.first.first.empty() &&
                    !server[i]._location[j].redirection.first.second.empty())
                {
                    int status_code = server[i]._location[j].redirection.second;
                    // search for message of the status_code
                    std::string message = get_response_status(status_code);
                    set_status_code(status_code);
                    set_status_message(message);
                    set_header("Location", server[i]._location[j].redirection.first);
                    _is_redirection = true;
                    return ;
                }
            }
            else
            {
                set_status_code(200);
                set_status_message("OK");
                return ;
            }
        }
    }
}

void    Respond::ft_check_allowed_methods()
{
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; server._location.size(); j++)
        {
            if (_path_found == server[i]._location[j].location_name)
            {
                // get the autoindex
                // _autoindex = server[i]._location[j].autoindex;
                // check for allowed methods
                std::vector<std::string> allowed_methods = server[i]._location[j].allowed_methods;
                for (int k = 0; k < allowed_methods.size(); k++)
                {
                    if (allowed_methods[k] == r.get_method())
                    {
                        _is_allowed_method = true;
                        return ;
                    }
                }
                set_status_code(405);
                set_status_message("Method Not Allowed");
                return ;
            }
        }
    }
}

void    Respond::ft_check_autoindex()
{
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; server._location.size(); j++)
        {
            if (_path_found == server[i]._location[j].location_name)
            {
                // check for autoindex
                if (server[i]._location[j].autoindex == true)
                {
                    _is_autoindex = true;
                    return ;
                }
            }
        }
    }
}