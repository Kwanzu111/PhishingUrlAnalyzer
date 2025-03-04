Phishing URL Analyzer is a web-based tool that analyzes URLs for potential phishing threats. It evaluates links based on HTTPS presence, suspicious keywords, and other risk factors, providing a risk score to help users identify potential phishing attempts.
Features

.Accepts URL input via GUI and CLI

.Detects suspicious keywords in URLs

.Checks if a URL uses HTTPS

.Calculates a risk score based on security factors

.Simple web-based interface with backend CGI scripts

Installation & Setup

1. Clone the Repository

       git clone https://github.com/kwanzu111/phishing-analyzer.git
   
       cd phishing-analyzer


3. Set Up the Web Server (Apache or Nginx with CGI)

    .Ensure CGI is enabled for your server.
   
    .Place the backend executable inside /srv/http/phishing/cgi-bin/
   
    .Serve frontend files from /srv/http/phishing/static/

5. Compile the Backend (C Code)

       gcc -o cgi-bin/analyzer backend.c -O2
   
       chmod +x cgi-bin/analyzer

4. Start the Server

       sudo systemctl restart httpd  # For Apache
    
       sudo systemctl restart nginx  # For Nginx  

Usage

CLI Mode

     curl -X POST -d "url=https://example.com/login" http://localhost/cgi-bin/analyzer

Web Interface

    http://localhost/frontend.html
                 
    Enter a URL in the input box and submit for analysis

Directory Structure

/srv/http/phishing/

│── cgi-bin/

│   ├── analyzer   
# Compiled backend (CGI)
│── static/

│   ├── frontend.html # Frontend UI

│   ├── styles.css # CSS Stylesheet

│── backend.c      # C source code

│── README.md

Contributing

.Fork the repository

.Create a new branch (git checkout -b feature-new)

.Commit your changes (git commit -m "Add new feature")

.Push to your fork and create a Pull Request

License

This project is licensed under the MIT License.
