/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_handling.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:52:50 by aoumad            #+#    #+#             */
/*   Updated: 2023/04/27 20:11:56 by aoumad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "respond.hpp"

std::string Respond::handle_get_response()
{
    // step 1: checking if it's a redirection
    if (_is_redirection == true)
        ft_handle_redirection();
    
    // step 2: check if it's a CGI or not (like if `index` of the configuration file has .py or .php...etc)
    if (_is_cgi == true)
        ft_handle_cgi();
    
    // step 3: check if it's a file or not
    if (ft_check_file() == true)
        ft_handle_file();

    // step 4 : check the index in the configuration file and render it
    if (_is_index == true)
        ft_handle_index();
    
    // step 5: check if the autoindex if on or off
    ft_handle_autoindex();
    
    // ft_handle_error(404);
    
    return ("");
}

std::string Respond::handle_post_response()
{
    if (check_post_type() == "application/x-www-form-urlencoded" && _is_cgi == true)
    {
        // need to call cgi and execute it
        cout_response();
    }
    if (check_post_type() == "multipart/form-data")
    {
        handle_form_data();
    }

}

void    Respond::handle_form_data()
{
    struct stat st;
    if (get_upload_store() == false || server.get_upload == "off")
        return ;
    _rooted_path.append(_upload_store);
    // if (stat(_rooted_path.c_str(), &st != 0))
    // {

    // }

    size_t  pos;
    std::stringstream body(r.get_body());
    std::string data;
    std::vector<FormData> form_fields;

    while (true)
    {
        std::getline(body, data);
        if (data.empty())
            break ;
    }

    if (data.find("Content-Type: ") != std::string::npos)
    {
        FormData field;
        field.content_type = data.substr(data.find("Content-Type: ") + 14);
        field.content_type.erase(field.content_type.find("\r\n\r\n"));
        
        std::string instance_form;
        while (true)
        {
            std::getline(body, data);
            if (data.find(_boundary) != std::string::npos 
                && data.find("\r\n") != std::string::npos)
                break ;
        }
        
        instance_form += data + "\n";
        
        field.data = instance_form;
        field.name = field.name;
        form_fields.push_back(field);
    }
    else
    {
        
    }
}

/*
    while (std::getline(body, line))
    {
        if (!line.empty())
            if (line.find("name=") != std::string::npos)
            {
                size_t start = line.find("name=") + 6;
                size_t end = line.find("\"", start);
                std::string form_name = line.substr(start, end - start);
                std::string file_name;
                if ((pos = line.find("filename=")) != std::string::npos)
                {
                    start = line.find("filename=") + 10;
                    end = line.find("\"", start);
                    file_name = line.substr(start, end - start);
                }
                else if (line.find("Content-Type: ") != std::string::npos)
                {
                    _content_type = line.substr(line.find("Content-Type: ") + 14);
                    _content_type.erase(_content_type.find("\r\n\r\n"));

                    // Read the next line (which contains the form data)
                    std::getline(body, data);
                    
                }
            }
    }
*/

int Respond::get_upload_store()
{
    for (int i = 0; i < server.size(); i++)
    {
        for (int j = 0; j < server[i]._location.size(); j++)
        {
            if (server[i]._location[j].location_name == _path_found)
            {
                if (server[i]._location[j].upload_store.empty())
                    return (0);
                else
                {
                    _upload_store = server[i]._location[j].upload_store;
                    return (1);
                }
            }
        }
    }
}

std::string Respond::check_post_type()
{
    if(r.get_header("Content-Type").find("multipart") != std::string::npos)
        return ("form-data");
    else if(r.get_header("Content-Type").find("application/x-www-form-urlencoded") != std::string::npos)
        return ("x-www-form-urlencoded");
    else
        return ("regular");
}

void    Respond::ft_handle_delete_response()
{
    std::string content_type;
    
    content_type = get_content_type(_path_found);
    if (ft_check_file() == true)
    {
        if (std::remove(_rooted_path.c_str()) == 0)
        {
            _status_code = "200";
            _status_message = get_response_status(_status_code);
            set_response_body("File deleted successfully");
            set_header("Content-Type", content_type);
            set_header("Content-Length", std::to_string(_response_body.length()));
            set_header("Connection", "keep-alive");
        }
        else
        {
            _status_code = "500";
            _status_message = get_response_status(_status_code);
            set_response_body("Error deleting file");
            set_header("Content-Type", content_type);
            set_header("Content-Length", std::to_string(_response_body.length()));
            set_header("Connection", "keep-alive");
        }
    }
    else
    {
        _status_code = "404";
        _status_message = "Not Found";
        set_response_body("File not found");
        set_header("Content-Type", content_type);
        set_header("Content-Length", std::to_string(_response_body.length()));
        set_header("Connection", "keep-alive");
    }
}