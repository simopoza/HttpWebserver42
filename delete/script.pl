#!/usr/bin/perl

use strict;
use warnings;
use CGI;

# Create a CGI object
my $cgi = CGI->new;

# Get the form data
my $name = $cgi->param('name');
my $email = $cgi->param('email');

# Print the content type and HTML header
print $cgi->header('text/html');

# Process and display the input
print "<html><head><title>Form Input Result</title></head><body>";
print "<h1>Form Input Result</h1>";

if ($name && $email) {
    print "Name: $name<br>";
    print "Email: $email<br>";
} else {
    print "Please provide both name and email.";
}

print "</body></html>";