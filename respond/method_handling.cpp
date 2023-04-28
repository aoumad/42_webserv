/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_handling.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoumad <aoumad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:52:50 by aoumad            #+#    #+#             */
/*   Updated: 2023/04/28 16:57:16 by aoumad           ###   ########.fr       */
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
    if (get_upload_store() == false || server.get_upload() == "off")
        return ;
    _rooted_path.append(_upload_store);
    // if (stat(_rooted_path.c_str(), &st != 0))
    // {

    // }

    // Find the first boundary
    size_t  pos = r.get_body().find(_boundary);
    if (pos == std::string::npos)
        return ;

    // Loop through the form data, locating boundaries and reading data betweem them
    while (true)
    {
        // find the next boundary
        size_t  next = r.get_body().find(_boundary, pos + _boundary.length());
        if (next == std::string::npos)
            break;
        
        // Read the data between the boundaries
        FormData formData = read_form_data(next);
        if (formData.isValid())
            _form_data.push_back(formData); // Add the form data to the list
    }
}

// Helper function to locate the next boundary in the form data
size_t Response::find_boundary(size_t pos)
{
    return (r.get_body().find(_boundary, pos));
}

// Helper function to read the form data between two boundaries
Response::FormData Response::read_form_data(size_t pos)
{
    FormData form_data;
    size_t   end_pos;

    // Read the first line (should be the form name)
    end_pos = r.get_body().find("\r\n", pos);
    if (end_pos == std::string::npos)
        return (form_data); // Invalid form data
    form_data.name = r.get_body().substr(pos, end_pos - pos);
    
    // Read the second line line (should be the content type)
    pos = end_pos + 2;
    end_pos = r.get_body().find("\r\n\r\n", pos);
    if (end_pos == std::string::npos)
        return (form_data); // Invalid form data

    form_data.content_type = r.get_body().substr(pos, end_pos - pos);

    // Read the data (everything after the content type)
    pos = end_pos + 4;
    end_pos = find_boundary(pos);
    if (end_pos == std::string::npos)
        return (form_data); // Invalid form data
        _form_data.data = r.get_body().substr(pos, end_pos - pos - 2);
    
    return (form_data);
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