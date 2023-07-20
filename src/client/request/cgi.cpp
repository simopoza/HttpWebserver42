#include "request.hpp"

void	request::cgiHandler()
{
	// Create child process
		// fork()
	std::cout << "Hello CGI" << std::endl;
	int	pid;

	// if (this->method == "POST")
	// {
	// 	char buf[1024];
	// 	int tmp = open(this->postFileName.c_str(), O_RDONLY, 0666);
	// 	if (tmp == -1)
	// 	{
	// 		std::cout << "errrrooooooor" << std::endl;
	// 	}
	// 	int r = 1;
	// 	while (r != 0)
	// 	{
	// 		r = read(tmp, buf, 1024);
	// 		// std::cout << "9rit :" << rd << std::endl;
	// 	}
	// 	buf[29] = '\0';
	// 	this->queryString = buf;
	// 	std::cout << "query is " << this->queryString << std::endl;
	// 	close(tmp);
	// }
		std::string tmpp;
		std::string			contLength;
		std::stringstream	convert;
		convert << this->contentLenght;
		convert >> contLength;
		setInterpreterPath();
		// char* const av[3] = {const_cast<char*>(this->interpreterPath.c_str()), const_cast<char*>(this->fullPath.c_str()), NULL};
		char**av = new char*[3];
		av[0] = new char[this->interpreterPath.size() + 1];
		strcpy(av[0], this->interpreterPath.c_str());
		av[1] = new char[this->fullPath.size() + 1];
		strcpy(av[1], this->fullPath.c_str());
		av[2] = NULL;
		char** envp;
		if (this->method == "POST")
			envp = new char*[15];
		else
			envp = new char*[13];
		tmpp = "SERVER_PORT=2000";
		envp[0] = new char[tmpp.size() + 1];
		strcpy(envp[0], (tmpp.c_str()));
		tmpp = "REQUEST_METHOD=" + this->method;
		envp[1] = new char[tmpp.size() + 1];
		strcpy(envp[1], (tmpp.c_str()));
		tmpp = "REMOTE_ADDR=10.11.11.6";
		envp[2] = new char[tmpp.size() + 1];
		strcpy(envp[2], (tmpp.c_str()));
		tmpp = "REDIRECT_STATUS=200";
		envp[3] = new char[tmpp.size() + 1];
		strcpy(envp[3], (tmpp.c_str()));
		tmpp = "QUERY_STRING=" + this->queryString;
		envp[4] = new char[tmpp.size() + 1];
		strcpy(envp[4], (tmpp.c_str()));
		tmpp = "PATH_INFO=" + this->fullPath;
		envp[5] = new char[tmpp.size() + 1];
		strcpy(envp[5], (tmpp.c_str()));
		tmpp = "SCRIPT_NAME=" + this->fullPath;
		envp[6] = new char[tmpp.size() + 1];
		strcpy(envp[6], (tmpp.c_str()));
		tmpp = "SCRIPT_FILENAME=" + this->fullPath;
		envp[7] = new char[tmpp.size() + 1];
		strcpy(envp[7], (tmpp.c_str()));
		tmpp = "SERVER_PROTOCOL=HTTP/1.1"; 
		envp[8] = new char[tmpp.size() + 1];
		strcpy(envp[8], (tmpp.c_str()));
		tmpp = "REQUEST_URI=" + this->fullPath;
		envp[9] = new char[tmpp.size() + 1];
		strcpy(envp[9], (tmpp.c_str()));
		tmpp = "GATEWAY_INTERFACE=CGI/1.1";
		envp[10] = new char[tmpp.size() + 1];
		strcpy(envp[10], (tmpp.c_str()));
		tmpp = "HTTP_ACCEPT=text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
		envp[11] = new char[tmpp.size() + 1];
		strcpy(envp[11], (tmpp.c_str()));
		if (this->method == "POST")
		{
			tmpp = "CONTENT_TYPE=" + this->contentType;
			envp[12] = new char[tmpp.size() + 1];
			strcpy(envp[12], (tmpp.c_str()));
			tmpp = "CONTENT_LENGTH=" + contLength; 
			envp[13] = new char[tmpp.size() + 1];
			strcpy(envp[13], (tmpp.c_str()));
			envp[14] = NULL;
		}
		else
			envp[12] = NULL;
		for (int i = 0; envp[i]; i++)
			std::cout << envp[i] << std::endl;
		for (int i = 0; av[i]; i++)
			std::cout << i << " is " << av[i] << std::endl;
	// const char* contentTypee = std::getenv("CONTENT_TYPE");

    // if (contentTypee) {
    //     std::cout << "-------Content-type: " << contentTypee << "\r\n\r\n";
    // } else {
    //     // Default to a content type (e.g., plain text) if CONTENT_TYPE is not set
    //     std::cout << "---Content-type: text/plain\r\n\r\n";
    // }
	pid = fork();
	if (pid == -1)
	{
		// fork has failed
	}
	if (pid == 0)
	{
		// prepareEnv();
		// SetUpInputOutputFiles();
	int	output;
	int	input;
	
		if (this->method == "POST")
		{
			std::cout << "filein is :   " << this->postFileName.c_str() << std::endl;
			input = open(this->postFileName.c_str(), O_RDONLY, 0777);
			if (input == -1)
			{
				std::cout << "errrrooooooor cgi" << std::endl;
			}
			// int test = open("./cgi.txt", O_CREAT | O_RDWR | O_TRUNC, 0777);
			// if (test == -1)
			// 	std::cout << "error al9lawi" << std::endl;
			// int g = 1;
			// char bf[1024];
			// while (g != 0)
			// {
			// 	g = read(input, bf, 1024);
			// 	// std::cout << "9rit hadchi " << bf << std::endl;
			// 	write(test, &bf, g);
			// }
			// std::cout << "done writing" << std::endl;
			dup2(input, 0);
			// close(input);
		}

		output = open(this->outputFile.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0777);
		if (output == -1)
		{
			std::cout << "error " << std::endl;
		}
		dup2(output, 1);
		execve(av[0], av, envp);
		exit(0);
	}

	// ...
	// std::cout << "l9lawi 5" << std::endl;
	else if (pid > 0)
		waitForChildProcess(pid);
	// back to the parent process
		// send response
			// wait for the child process 
			// read response from output file 
}

void	request::waitForChildProcess(int pid)
{
	int	status = 0;

	// sleep sometime so the script can finish executing
	// sleep(2);
	waitpid(pid, &status, 0);
	// std::cout << "l9lawi 6" << std::endl;
		// send response 
		std::cout << "out" << std::endl;
		// this->filePost.close();
		this->endPost = 1;
		this->goToClient(this->outputFile, "200");
}

void	request::setInterpreterPath()
{
	// std::cout << "l0lawo" << std::endl;
	// std::cout << this->scriptExtension << " extension" << std::endl;
	if (this->scriptExtension == "php")
		this->interpreterPath = "/Users/mannahri/Desktop/webserv/php-cgi";
	else if (this->scriptExtension == "pl")
		this->interpreterPath = "/usr/bin/perl";

	// std::cout << "l9lawi " << this->interpreterPath << std::endl;
}

// void	request::SetUpInputOutputFiles()
// {
// 	int	output;
// 	int	input;
	

// 	// Opening the file that contains the body of the request
// 	// in case of POST and redirect it to STDIN using dup2
// 	if (this->method == "POST")
// 	{
// 		char buf[1024];
// 		std::cout << "method is  " << this->method << std::endl;
// 		input = open(this->postFileName.c_str(), O_RDONLY, 0666);
// 		if (input == -1)
// 		{
// 			std::cout << "errrrooooooor" << std::endl;
// 		}
// 		int rd = 1;
// 		// // std::cout << "fd is inside : " << this->post << std::endl;
// 		int tmp = open("./check", O_CREAT | O_RDWR | O_TRUNC, 0777);
// 		if (tmp == -1)
// 		{
// 			std::cout << "w93 mochkil" << std::endl;
// 		}
// 		while (rd != 0)
// 		{
// 			rd = read(input, buf, 1024);
// 			write(tmp, &buf, rd);
// 			// std::cout << "9rit :" << rd << std::endl;
// 		}
// 		write(tmp, "\0", 1);
// 		// this->queryString = buf;
// 		// std::cout << "query is " << this->queryString << std::endl;
// 		// read(this->post, buf, this->postFileName.size());
// 		// std::cout << "size read from post " << rd << std::endl;
// 		std::cout << "done writing" << std::endl;
// 		dup2(tmp, 0);
// 		// close(input);
// 	}

// 	// Opening a file used to store the output of the cgi
// 	output = open(this->outputFile.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0666);
// 	if (output == -1)
// 	{
// 		std::cout << "error azbi " << std::endl;
// 	}
// 	// dup2(output, 1);
// 	// close(output);
// }

// void	request::prepareEnv()
// {
// 	std::string			contLength;
// 	std::stringstream	convert;
// 	convert << this->contentLenght;
// 	convert >> contLength;

// 	setenv("CONTENT_TYPE", this->contentType.c_str(), 1);
// 	setenv("REQUESTED_METHOD", this->method.c_str(), 1);
// 	setenv("CONTENT_LENGTH", contLength.c_str(), 1);
// 	setenv("SCRIPT_NAME", this->fullPath.c_str(), 1);
// 	setenv("SCRIPT_FILENAME", this->fullPath.c_str(), 1);
// 	setenv("PATH_INFO", this->fullPath.c_str(), 1);
// 	setenv("REDIRECT_STATUS", "200", 1);
// 	setenv("QUERY_STRING", this->queryString.c_str(), 1);
// 	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
// }

// std::string	request::retriveQueryString()
// {
// 	std::size_t	position;
// 	std::string	query = "";

// 	position = this->fullPath.find_first_of("?");
// 	if (found = std::string::npos)
// 		return (query);
// 	else
// 		query = this->fullPath.substr(position + 1, this->fullPath.size() - (position + 1));
// 	return (query);
// }

