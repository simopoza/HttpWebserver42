#!/bin/bash

# Set the Content-Type header to indicate the response type (e.g., HTML)
echo "Content-Type: text/html"
echo

# Read form data from standard input (POST method) or environment variables (GET method)
if [ "$REQUEST_METHOD" = "POST" ]; then
    read -n $CONTENT_LENGTH FORM_DATA <&0
else
    FORM_DATA="$QUERY_STRING"
fi

# Parse the form data to retrieve individual fields (assuming the form has a 'name' field)
NAME=$(echo "$FORM_DATA" | grep -o 'name=[^&]*' | sed 's/name=//')

# Start generating the HTML content
cat <<EOF
<!DOCTYPE html>
<html>
<head>
  <title>CGI Form Handler</title>
</head>
<body>
  <h1>Hello, $NAME!</h1>
  <p>Thank you for submitting the form.</p>
</body>
</html>
EOF