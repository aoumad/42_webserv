#include "respond.hpp"

/* GET:
HTTP/1.1 200 OK
Server: MyServer/1.0
Content-Type: text/html
Content-Length: 123
Connection: close

<html>
<head>
    <title>Welcome to my website</title>
</head>
<body>
    <h1>Hello, world!</h1>
    <p>This is a sample HTML page served by MyServer.</p>
</body>
</html>
*/

/* put:
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 56
Server: MyServer/1.0
Connection: close

The file has been successfully updated.
*/

Respond::Respond()
{
    _http_method = "";
    _http_version = "HTTP/1.1";
    _response_body = "";
    _cgi_path = "";
    _location_path = "";
}

Respond::~Respond()
{}

std::string Respond::get_response(std::string http_method, std::string request_path)
{
    std::string response;

    if (http_method == "GET")
        response = handle_get_request(request_path);
    else if (http_method == "POST")
        response = handle_post_request(request_path);
    else if (http_method == "PUT")
        response = handle_put_request(request_path);
    else if (http_method == "DELETE")
        response = handle_delete_request(request_path);
    else
    {
        response = "HTTP/1.1" + " " + status_code(501) + "\r\n";
        response += "Content-Type: text/html\r\n";
        response += "Content-Length: " + std::to_string(_response_body.length()) + "\r\n";
        response += "\r\n";
    }

    _headers.erase("Content-Length"); // remove Content-Length header if it exists
    _headers["Content-Length"] = std::to_string(_response_body.length()); // add Content-Length header

    // add _headers to response

}

std::string Respond::handle_get_request(std::string request_path)
{
}