import requests

def upload_and_process_image(image_path, upload_url):
    try:
        with open(image_path, 'rb') as image_file:
            files = {'file': image_file}
            response = requests.post(upload_url, files=files)

            if response.status_code == 200:
                print("Image uploaded and processed successfully.")
                print("Response from server:")
                print(response.json())
            else:
                print(f"Failed to upload and process image. Status code: {response.status_code}")
    except FileNotFoundError:
        print("Image file not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    # Replace 'YOUR_IMAGE_PATH' with the path to the image you want to upload and process
    image_path = './www/1.jpg'
    
    # Replace 'YOUR_UPLOAD_URL' with the URL of your web server's API endpoint that handles image uploads and processing
    upload_url = './delete/images/'
    
    upload_and_process_image(image_path, upload_url)