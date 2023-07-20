#include "request.hpp"

request::request()
{
    this->contentLenght = 0;
    this->sizeReaded = 0;
	this->currentLenReaded = 0;
	this->outputFile = "./outputFile";
}

request::request(const request &obj)
{
    *this = obj;
}

request& request::operator=(const request &obj)
{
    this->contentLenght = obj.contentLenght;
    this->fileName = obj.fileName;
    this->sizeReaded = obj.sizeReaded;
	this->headerEnds = obj.headerEnds;
	this->locationWorkWith = obj.locationWorkWith;
	this->content = obj.content;
    return *this;
}

void    request::startRequest()
{
	this->ft_read();
	if (this->currentLenReaded == 0 || this->currentLenReaded == -1)
		this->closeConnection();
	if (this->headerEnds == 0){
		this->stringOfheaders();
		this->setHeadersInVector();
	}
	if (this->headerEnds == 1)
		parsRequest();
}

void	request::ft_read()
{
	this->content.resize(8000);
	this->currentLenReaded = read(this->clientFd, (void*)this->content.c_str(), 8000);
	std::cout << "read is : " << this->currentLenReaded << std::endl;
	// this->contentLenght -= this->currentLenReaded;
	this->sizeReaded += this->currentLenReaded;
}

void	request::closeConnection()
{
	std::cout << "GO OUT \n";
	this->filePost.close();
	this->endPost = 1;
	this->goToClient("", "");
}

void	request::stringOfheaders()
{
	// std::string tmpBuffer = buffer;
	this->headerSize = this->content.find("\r\n\r\n");
	if (this->headerSize != std::string::npos)
	{
		this->headerEnds = 1;
		this->headers = this->content.substr(0, this->headerSize+4);
		this->content = this->content.substr(this->headerSize+4, this->currentLenReaded);
		return ;
	}
	if (this->currentLenReaded >= 8000)
		this->goToClient("DefaultErrorPages/431", "431");
}

void	printVector(std::vector<std::string>& header)
{
	for (std::vector<std::string>::iterator iter = header.begin(); iter != header.end(); iter++)
		std::cout << "line is : " << *iter << std::endl;
}

void	request::setHeadersInVector()
{
	int firstLine = 0;
	//split header
	std::string::size_type pos = 0;
    std::string::size_type prev = 0;
	while ((pos = this->headers.find('\n', prev)) != std::string::npos)
	{
		std::string tmp = this->headers.substr(prev, pos - prev);
        this->headerLines.push_back(tmp);
		if (!firstLine)
		{
			CheckLineRequest();
			firstLine++;
		}
		this->collectHeaderData(tmp, "Content-Length: ");
		this->collectHeaderData(tmp, "Transfer-Encoding: ");
		this->collectHeaderData(tmp, "Content-Type: ");
        prev = pos + 1;
    }
    this->headerLines.push_back(this->headers.substr(prev));
	printVector(this->headerLines);
	this->checkHeadersErrors();
}

void	request::CheckLineRequest()
{
	std::vector<std::string> firstLineSplited;
	splitedLine(this->headerLines[0], firstLineSplited);
	if (firstLineSplited[1].size() >= 100)
		this->goToClient("DefaultErrorPages/414.html", "414");
	this->method = firstLineSplited[0];
	separatePathFromQuery(firstLineSplited[1]);
	// this->requestedPath = firstLineSplited[1];//fjklsd?fkd;
	// std::cout << "method : " << this->method << "\nrequest Path : " << this->requestedPath << "\n";
}

void	request::separatePathFromQuery(std::string URL)
{
	size_t	position;
	size_t	pos;

	std::cout << "jit hna" << std::endl;
	if ((position = URL.find(".php")) != std::string::npos)
	{
		this->requestedPath = URL.substr(0, position + 4);
		if ((pos = URL.find("?")) != std::string::npos)
			this->queryString = URL.substr(position + 5, URL.size() - (position + 5));
		this->scriptExtension = URL.substr(position + 1, 3);
		std::cout << "full URL " << URL << std::endl;
		std::cout << "requested path  " << this->requestedPath << std::endl;
		std::cout << "query " << this->queryString << std::endl;
		std::cout << "extension " << this->scriptExtension << std::endl;
		return ;
	}
	if ((position = URL.find(".py")) != std::string::npos)
	{
		this->requestedPath = URL.substr(0, position + 3);
		if ((pos = URL.find("?")) != std::string::npos)
			this->queryString = URL.substr(position + 4, URL.size() - (position + 4));
		this->scriptExtension = URL.substr(position + 1, 2);
		std::cout << "full URL " << URL << std::endl;
		std::cout << "requested path  " << this->requestedPath << std::endl;
		std::cout << "query " << this->queryString << std::endl;
		std::cout << "extension " << this->scriptExtension << std::endl;
		return ;
	}
	else
		this->requestedPath = URL;
}

void	request::parsRequest()
{
	if (method == "GET")
		GetMethod(this->requestedPath);
	else if (method == "POST")
		PostMethod(this->requestedPath);
	else if (method == "DELETE")
		DeleteMethod();
	else
	{
		errorPages(405);
		this->goToClient("DefaultErrorPages/405.html", "405");
	}
	// std::cout << "method is : " << firstLineSplited[0] << "\npath is : " << firstLineSplited[1] << "\n" << firstLineSplited[2] << std::endl;
}

void	request::DeleteMethod()
{
	std::cout << "Work on delete" << std::endl;
	findTheLocation(this->requestedPath);
	methodAllowd("DELETE");
	joinLocationWithRoot(this->requestedPath, this->locationWorkWith.getRoot());
	if (checkIsADirectory())
		goToClient("DefaultErrorPages/409.html", "409");
	else
		performDeleteOnFile();
}

void	request::performDeleteOnFile()
{
	if (access(this->fullPath.c_str(), F_OK) == 0)
	{
		remove(this->fullPath.c_str());
		this->goToClient("DefaultErrorPages/200.html", "200");
	}
	else
		this->goToClient("DefaultErrorPages/404.html", "404");
}

void	request::GetMethod(std::string path)
{
	std::cout << "IM HERE\n";
	findTheLocation(path);
	methodAllowd("GET");
	handleGetRequest();
}

void	request::findTheLocation(std::string path)
{
	matchLocation(path);
	if (!this->locationWorkWith.getRedirection().empty())
	{
		if (this->method == "POST")
			this->endPost = 1;
		this->redirection = this->locationWorkWith.getRedirection();
		this->goToClient("DefaultErrorPages/301.html", "301");
	}
}

void	request::methodAllowd(std::string method)
{
	// Element in vector.
	if (std::find(this->locationWorkWith.getMethods().begin(), this->locationWorkWith.getMethods().end(), method) == this->locationWorkWith.getMethods().end())
	{
		std::cout << "METHOD NOT ALLOWD\n";
		this->endPost = 1;
		this->goToClient("DefaultErrorPages/405.html", "405");
	}
}

void	request::handleGetRequest()
{
	if (checkIsADirectory())
		serveDirectory();
	else
		serveFile();
}

void	request::matchLocation(std::string path)
{
	convertHex(path);
	this->sortLocationsVector(this->server.getLocations());
	std::vector<location>::iterator iter = this->server.getLocations().begin();
	size_t found;
	for (; iter != this->server.getLocations().end(); iter++)
	{
		found = path.find(iter->getPathOfLocation());
		if (found != std::string::npos)
		{
			joinLocationWithRoot(path, iter->getRoot());
			this->locationWorkWith = *iter;
			return ;
		}
	}
	errorPages(404);
	this->goToClient("DefaultErrorPages/404.html", "404");
}

void	request::errorPages(int errorCode)
{
	std::map<int, std::string>::iterator iter = this->server.getErrorPages().begin();
	if (this->server.getErrorPages().count(errorCode) > 0)
	{
		iter = this->server.getErrorPages().lower_bound(errorCode);
		goToClient(iter->second, "404");
	}
}

void	request::joinLocationWithRoot(std::string path, std::string root)
{
	this->pathForIndex = path;
	if (root[root.size() - 1] == '/')
		path.erase(0, 1);
	this->fullPath = root + path;
}

int	request::checkIsADirectory()
{
	const char* directory = this->fullPath.c_str();
	std::cout << "FULL : " << directory << "\n";
	DIR* directoryStream = opendir(directory);
	if (directoryStream != NULL)
	{
		closedir(directoryStream);
		return (1);
	}
	return (0);
}

// void	request::serveFile()
// {
// 	if (access(this->fullPath.c_str(), F_OK) == 0)
// 		goToClient(this->fullPath, "200");
// 	else
// 	{
// 		errorPages(404);
// 		this->goToClient("DefaultErrorPages/404.html", "404");
// 	}
// }

void	request::serveFile()
{
	// int result;
	// 
	std::cout << "Working With File ??" << std::endl;
	std::cout << "working path : " << this->fullPath << std::endl;
	if (access(this->fullPath.c_str(), F_OK) == 0)
	{
		// check if extension is .php or .perl and cgi is on, and the script is executable
		// if yes go to cgi, else go to client
		std::cout << "YES" << std::endl;
		if (this->locationWorkWith.getCGI() && !this->scriptExtension.empty())
		{
			// std::cout << "ZBIIIIIII " << std::endl;
			std::cout << "CGI" << std::endl;
			cgiHandler();
		}
		else
			goToClient(this->fullPath, "200");
	}
	else
	{
		std::cout << "404 not found" << std::endl;
		errorPages(404);
		this->goToClient("DefaultErrorPages/404.html", "404");
	}
}

void	request::serveDirectory()
{
	if (this->requestedPath[this->requestedPath.size()-1] != '/' && this->requestedPath != "/favicon.ico")
	{
		this->redirection = this->requestedPath + "/";
		this->goToClient("DefaultErrorPages/301.html", "301");
	}
	if (!this->locationWorkWith.getDefaultFile().empty())
		ifDefaultFileSpecified();
	else if (this->locationWorkWith.getDefaultFile().empty())
		ifNoDefaultFileSpecified();
}

void	request::ifDefaultFileSpecified()
{
	std::string tmp;
	if (this->fullPath[this->fullPath.size() - 1] != '/')
		tmp = this->fullPath + "/" + this->locationWorkWith.getDefaultFile();
	else if (this->fullPath[this->fullPath.size() - 1] == '/')
		tmp = this->fullPath + this->locationWorkWith.getDefaultFile();
	if (access(tmp.c_str(), R_OK) == 0)
		this->goToClient(tmp, "200");
	else if (!this->locationWorkWith.getAutoindex())
	{
		errorPages(403);
		this->goToClient("DefaultErrorPages/403.html", "403");
	}
	else if (this->locationWorkWith.getAutoindex())
		generateAutoIndex();
}

void	request::ifNoDefaultFileSpecified()
{
	std::string tmp;
	if (this->fullPath[this->fullPath.size() - 1] != '/')
		tmp = this->fullPath + "/index.html";
	else if (this->fullPath[this->fullPath.size() - 1] == '/')
		tmp = this->fullPath + "index.html";
	if (access(tmp.c_str(), F_OK) == 0)
		this->goToClient(tmp, "200");
	else if (!this->locationWorkWith.getAutoindex())
	{
		errorPages(403);
		goToClient("DefaultErrorPages/403.html", "403");
	}
	else if (this->locationWorkWith.getAutoindex())
		generateAutoIndex();
}

void	request::generateAutoIndex()
{
		std::ofstream autoIndex;
		autoIndex.open("autoIndex.html", std::ios::out);
		genAutoIndex(autoIndex);
		autoIndex.close();
		goToClient("autoIndex.html", "200");
}

void	request::genAutoIndex(std::ofstream& output)
{
	std::string tmpJoin;
	std::string parentDir;
	DIR* dir = opendir(this->fullPath.c_str());
	if (dir)
	{
		output << "<html><head><title>Index of " << this->pathForIndex << "</title></head><body>";
		output << "<h1>Index of " << this->pathForIndex << "</h1><hr><pre>";
		struct dirent *entryPoint;
		entryPoint = readdir(dir);
		output << "<table>";
		while ((entryPoint = readdir(dir)) != NULL)
		{
			struct stat fileInfo;
			std::string tmp = this->fullPath + "/" + entryPoint->d_name;
			stat(tmp.c_str(), &fileInfo);
			if (entryPoint->d_name[0] == '.' && strlen(entryPoint->d_name) == 1)
				continue;
			else if(entryPoint->d_name[0] == '.' && entryPoint->d_name[1] == '.' && strlen(entryPoint->d_name) == 2)
			{
				tmpJoin = this->pathForIndex.substr(0, this->pathForIndex.find_last_of("/"));
				std::cout << tmpJoin << std::endl;
			}
			else
				tmpJoin = this->pathForIndex + "/" + entryPoint->d_name;
			output << "<tr>";
				output << "<td style=\"padding-right: 150px\"><a href=\"" << tmpJoin << "\">" << entryPoint->d_name << "</a></td>";
				output << "<td style=\"padding-right: 150px\">" << ctime(&fileInfo.st_mtime) << "</td>";
				output << "<td style=\"padding-right: 150px\">" << fileInfo.st_size << "</td>";
			output << "</tr>";
		}
		output << "</table></pre><hr></body></html>";
		closedir(dir);
	}
}

void	request::goToClient(std::string file, std::string num)
{
	this->fileName = file;
	throw Exception (num);
}

// [error] socket: unable to connect sock.c:282: Operation timed out
//[error] socket: 176771072 address is unavailable.: Can't assign requested address
//[alert] socket: read check timed out(30) sock.c:273: Operation timed out