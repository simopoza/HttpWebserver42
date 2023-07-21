import http.client
import urllib.parse

def upload_file(file_path, upload_url):
    try:
        with open(file_path, 'rb') as file:
            file_contents = file.read()

        parsed_url = urllib.parse.urlsplit(upload_url)
        connection = http.client.HTTPSConnection(parsed_url.netloc)

        # Prepare the headers
        headers = {
            'Content-Type': 'application/octet-stream',
            'Content-Disposition': f'attachment; filename="{parsed_url.path.split("/")[-1]}"'
        }

        # Send the POST request with the file contents as the body
        connection.request('POST', parsed_url.path, body=file_contents, headers=headers)

        response = connection.getresponse()

        if response.status == 200:
            print("File uploaded successfully.")
        else:
            print(f"Failed to upload file. Status code: {response.status}")
            print(response.read().decode())

        connection.close()

    except FileNotFoundError:
        print("File not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    # Replace 'YOUR_FILE_PATH' with the path to the file you want to upload
    file_path = 'YOUR_FILE_PATH'
    
    # Replace 'YOUR_UPLOAD_URL' with the URL of the server that handles file uploads
    upload_url = 'YOUR_UPLOAD_URL'
    
    upload_file(file_path, upload_url)