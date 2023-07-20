#include "request.hpp"

void	request::collectHeaderData(std::string& line, std::string stringToFind)
{
	size_t place = line.find(stringToFind);
	if (place != std::string::npos)
	{
		std::string headerValue = line.substr(place + stringToFind.size(), line.size()-(stringToFind.size()+1));
		std::cout << "HEAD IS : |" << headerValue << "|\n";
		if (stringToFind == "Content-Type: ")
			this->contentType = headerValue;
		if (stringToFind == "Content-Length: ")
		{
			this->contentLenght = atol(headerValue.c_str());
			this->flagContentLength = true;
		}
		if (stringToFind == "Transfer-Encoding: ")
		{
			this->transferEncoding = headerValue;
			this->flagTransferEncoding = true;
		}
	}
}

void	request::checkHeadersErrors()
{
	//throw exception type not definde;
	if (this->method == "POST")
	{
		if (this->flagContentLength && this->flagTransferEncoding)
		{
			this->filePost.close();
			this->endPost = 1;
			this->goToClient("DefaultErrorPages/400.html", "400");
		}
		if (!this->flagContentLength && !this->flagTransferEncoding)
		{
			this->filePost.close();
			this->endPost = 1;
			this->goToClient("DefaultErrorPages/411.html", "411");
		}
	}
}

void	request::PostMethod(std::string path)
{
	findTheLocation(path);
	methodAllowd("POST");
	//check if cgi on and path executable .php .py
	//flag cgi
	//file.pl
	// if (this->locationWorkWith.getCGI() && access(path.c_str(), X_OK) && !cgi.empty())
	// int flag = 1;

		this->noChunckedRequest();
}

void	request::noChunckedRequest()
{
	if (this->server.getClientBodySize() < this->contentLenght)
	{
		std::cout << "BODY SIZE\n";
		this->endPost = 1;
		this->goToClient("DefaultErrorPages/413.html", "413");
	}
	if (this->sizeReaded <= 8000)
	{
		this->createTheUploadFile();
		this->filePost.write(this->content.c_str(), this->content.size());
	}
	else
		this->filePost.write(this->content.c_str(), this->currentLenReaded);
	if ( this->contentLenght -  this->sizeReaded < 0 )
	{
		if (this->locationWorkWith.getCGI() && !this->scriptExtension.empty())
		{
			// std::cout << "ZBIIIIIII " << std::endl;
			std::cout << "CGI" << std::endl;
			this->filePost.close();
			this->cgiHandler();
		}
		this->endPost = 1;
		this->goToClient("DefaultErrorPages/200.html", "200");
	}
}

void	request::chunckedRequest()
{
	if (this->sizeReaded <= 8000)
	{
		this->createTheUploadFile();
	}
	if (this->content.find("\r\n0\r\n\r\n") != std::string::npos)
	{
		this->filePost.close();
		this->endPost = 1;
		this->goToClient("DefaultErrorPages/200.html", "200");
	}
}

void	request::createTheUploadFile()
{
	this->ExtentionType();
	this->postFileName = "../../goinfre/" + std::to_string(this->clientFd) + "file";
	this->filePost.open(this->postFileName);
}

void	request::ExtentionType()
{
	size_t place = this->contentType.find("/");
	if (place != std::string::npos)
		this->extention = this->contentType.substr(place+1, this->contentType.size()-(place+2));
}

//Search for all read/recv/write/send on a socket and check that, if an error is returned, the client is removed.