#ifndef RESPOND_HPP
#define RESPOND_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <fstream>

class Respond
{
    private:
        std::string _http_method;
        std::string _http_version;
        std::string _http_path;
        std::string _response_body;
        std::map<std::string, std::string> _headers;

        std::string get_file_contents(std::string path);
        std::string status_code(int code);

        std::string handle_get_request(std::string request_path);
        std::string handle_post_request(std::string request_path);
        std::string handle_put_request(std::string request_path);
        std::string handle_delete_request(std::string request_path);
    public:
        Respond();
        ~Respond();
        std::string get_response(std::string request_path, std::string http_method);
        std::string get_response_status(int status_code);
        std::string get_mime_type(std::string file_extension);
};

#endif