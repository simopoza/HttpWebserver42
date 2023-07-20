#include "request.hpp"

void	request::cgiHandler()
{
	int	pid;

	std::cout << "start cgi " << std::endl;
	pid = fork();
	if (pid == -1)
	{
		// fork has failed
	}
	if (pid == 0)
	{
		prepareEnv();
		SetUpInputOutputFiles();
		execve(this->av[0], this->av, this->envp);
		exit(0);
	}
	else if (pid > 0)
		waitForChildProcess(pid);
}

void	request::waitForChildProcess(int pid)
{
	int	status = 0;
	waitpid(pid, &status, 0);
	// std::cout << "l9lawi 6" << std::endl;
		// send response 
		std::cout << "out" << std::endl;
		// this->filePost.close();
		this->endPost = 1;
		this->CGI = true;
		this->goToClient(this->outputFile, "200");
}

void	request::setInterpreterPath()
{
	if (this->scriptExtension == "php")
		this->interpreterPath = "/Users/sel-ouaf/Desktop/server/cgi-bin/php-cgi";
	else if (this->scriptExtension == "py")
		this->interpreterPath = "/Users/sel-ouaf/Desktop/server/cgi-bin/python-cgi";
	std::cout << "this is it " << this->interpreterPath << std::endl;
}

void	request::SetUpInputOutputFiles()
{
	int	output;
	int	input;

	if (this->method == "POST")
	{
		input = open(this->postFileName.c_str(), O_RDONLY, 0777);
		if (input == -1)
		{
			std::cout << "errrrooooooor cgi" << std::endl;
		}
		dup2(input, 0);
		// close(input);
	}
	output = open(this->outputFile.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (output == -1)
	{
		std::cout << "error " << std::endl;
	}
	dup2(output, 1);
}

void	request::prepareEnv()
{
	std::string tmpp;
	std::string			contLength;
	std::stringstream	convert;
	convert << this->contentLenght;
	convert >> contLength;

	setInterpreterPath();

	this->av = new char*[3];
	av[0] = new char[this->interpreterPath.size() + 1];
	strcpy(av[0], this->interpreterPath.c_str());
	av[1] = new char[this->fullPath.size() + 1];
	strcpy(av[1], this->fullPath.c_str());
	av[2] = NULL;

	if (this->method == "POST")
		this->envp = new char*[15];
	else
		this->envp = new char*[13];
	tmpp = "SERVER_PORT=2000";
	this->envp[0] = new char[tmpp.size() + 1];
	strcpy(this->envp[0], (tmpp.c_str()));
	tmpp = "REQUEST_METHOD=" + this->method;
	this->envp[1] = new char[tmpp.size() + 1];
	strcpy(this->envp[1], (tmpp.c_str()));
	tmpp = "REMOTE_ADDR=10.11.11.6";
	this->envp[2] = new char[tmpp.size() + 1];
	strcpy(this->envp[2], (tmpp.c_str()));
	tmpp = "REDIRECT_STATUS=200";
	this->envp[3] = new char[tmpp.size() + 1];
	strcpy(this->envp[3], (tmpp.c_str()));
	tmpp = "QUERY_STRING=" + this->queryString;
	this->envp[4] = new char[tmpp.size() + 1];
	strcpy(this->envp[4], (tmpp.c_str()));
	tmpp = "PATH_INFO=" + this->fullPath;
	this->envp[5] = new char[tmpp.size() + 1];
	strcpy(this->envp[5], (tmpp.c_str()));
	tmpp = "SCRIPT_NAME=" + this->fullPath;
	this->envp[6] = new char[tmpp.size() + 1];
	strcpy(this->envp[6], (tmpp.c_str()));
	tmpp = "SCRIPT_FILENAME=" + this->fullPath;
	this->envp[7] = new char[tmpp.size() + 1];
	strcpy(this->envp[7], (tmpp.c_str()));
	tmpp = "SERVER_PROTOCOL=HTTP/1.1"; 
	this->envp[8] = new char[tmpp.size() + 1];
	strcpy(this->envp[8], (tmpp.c_str()));
	tmpp = "REQUEST_URI=" + this->fullPath;
	this->envp[9] = new char[tmpp.size() + 1];
	strcpy(this->envp[9], (tmpp.c_str()));
	tmpp = "GATEWAY_INTERFACE=CGI/1.1";
	this->envp[10] = new char[tmpp.size() + 1];
	strcpy(this->envp[10], (tmpp.c_str()));
	tmpp = "HTTP_ACCEPT=text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
	this->envp[11] = new char[tmpp.size() + 1];
	strcpy(this->envp[11], (tmpp.c_str()));
	if (this->method == "POST")
	{
		tmpp = "CONTENT_TYPE=" + this->contentType;
		this->envp[12] = new char[tmpp.size() + 1];
		strcpy(this->envp[12], (tmpp.c_str()));
		tmpp = "CONTENT_LENGTH=" + contLength;
		this->envp[13] = new char[tmpp.size() + 1];
		strcpy(this->envp[13], (tmpp.c_str()));
		this->envp[14] = NULL;
	}
	else
		this->envp[12] = NULL;
}
