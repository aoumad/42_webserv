## Table of content

- [Where do we start from?](#where-do-we-start-from)
- [An overview of HTTP](#an-overview-of-http)
- [Features](#features)
- [Installation](#installation)
- [HTTP Request](#http-request)
- [HTTP Response](#http-response)

> **Note**
> Concerning the implementation of our own http server, it's a huge project and requires a lot of time. Thus i implemented it with two of my friends; this is why in this `sub-repo` i will only be responsible of explaining the part of project i was responsible of (which is parsing the request that comes from the client through the server and generate the response and send it back to the server so that it can be shown in the preview of the client). However we made a repo of the whole code (soon here)

## Where do we start from?

- HTTP is everywhere! Every website we visit is ran on HTTP server. You may think that then what about HTTPS servers? Technically HTTPS is same HTTP with more security.
- Many programmers at some point or the other may get curious about how HTTP servers work and how to build one from scratch without any external libraries.

And I'm one of that many programmers.

- In this project we will build an http server using CPP 98... pretty old version of C++ but don't worry it's more than enough to implement an awesome http server ;)

## An overview of HTTP

- HTTP is a protocol for fetching resources such as HTML documents. It is the foundation of any data exchange on the Web and it is a client-server protocol, which means requests are initialed by the recipient, usually the Web browser. A complete document is reconstructed from the different sub-documents fetched, for instance, text, layout description, images, videos, scripts and more.
- As a `request-response` protocol, HTTP gives users a way to interact with web resources such as HTML files by transmitting hypertext messages between clients and servers.
- HTTP utilizes `specific request methods` in order to perform various tasks. All HTTP servers use the GET and HEAD methods, but not all support the rest of these request methods... However in our project we must implement at least the three neccessary methods (GET-POST-DELETE).
- Clients and servers communicate by exchanging individual messages (as opposed to a stream of data). The messages sent by the client, usually a Web browser, are called `requests` and the messages sent by the server as an answer are called `responses` and this is exactly what i am responsible of.

## Features
- Support for HTTP/1.1 protocol
- Parsing incoming requests to extract relevant information.
- The ability to match the URL/URI with the locations of the servers in the configue file either to match it by `exact location` `dynamic location in case we have CGI` or using `prefix location`.
- Generation of responses based on the parsed requests.
- The server can handle multi requests in same time and generate responses as well in same time.
- Implementation of GET, POST, DELETE.
- It handle CGI (php and python)
- Concerning GET method, it handles files, index, autoindex and CGI as well.
- Concerning POST method, it handles CGI, form-data, x-www-form-urlencoded (this one handles both CGI and decode files).
- Error handling and appropriate status code responses.

## Installation
- To use our code, go to this link where we have the whole code of HTTP/1.1 server and follow these steps:

1 - Clone this repository to your local machine.

2 - Ensure you have a compatible C++ compiler installed. (compile with `make && ./webserv /Users/aoumad/Desktop/(project name)/prs_rsc/server.conf` just replace the location part based on the location of the project you cloned.

3 - Run the compiled server executable.

4 - Go to POSTMAN or to the browser and test like 'http://localhost:8001/upload/' or any location you find in the configue file.

PS: check the location of the host if you got an error such as 'Bind system...'

## HTTP Request
