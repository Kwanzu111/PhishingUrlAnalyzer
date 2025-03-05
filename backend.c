#include <stdio.h>     // Standard I/O functions (printf, fread.)
#include <string.h>    // String manipulation functions (strcpy, strstr.)
#include <stdlib.h>    // Memory allocation and conversion functions (atoi, getenv.)
#include <ctype.h>     // Character processing functions (isxdigit)

// Global variable for URL this store the url that is sent
char url[1024];

// Function to decode URL-encoded characters example for spaces is %20 and %2F for :
void urldecode(char *src) {
    char a, b;
    char *dst = src;

    while (*src) {
        if (*src == '%' && isxdigit(src[1]) && isxdigit(src[2])) {
            // Convert first hex digit (X)
            if (src[1] >= 'a' && src[1] <= 'f') {
                a = src[1] - 'a' + 10; // converts a to 10 and b to 12
            } else if (src[1] >= 'A' && src[1] <= 'F') {
                a = src[1] - 'A' + 10;// same as the above
            } else {
                a = src[1] - '0';// coverts the  the integers
            }

            // Convert second hex digit (Y)
            if (src[2] >= 'a' && src[2] <= 'f') {
                b = src[2] - 'a' + 10;
            } else if (src[2] >= 'A' && src[2] <= 'F') {
                b = src[2] - 'A' + 10;
            } else {
                b = src[2] - '0';
            }
            *dst++ = (char)(16 * a + b); // this converts the hexadecimal
            src += 3;  // Skip  '%XX'
        } else {
            *dst++ = *src++;// this make sure the text its not changed by copying it
        }
    }
    *dst = '\0'; // ensure why it is wll terminated
}


// Function prototypes
int has_https(const char *url);
int is_suspicious(const char *url);
int calculate_risk(const char *url);

int main() {
    // GetS CONTENT_LENGTH FROM A SERVER ENVIRONMENT IF ITS MISSING ITS AN ERROR
    char *content_length = getenv("CONTENT_LENGTH");
    if (!content_length) {
        printf("Missing CONTENT_LENGTH!\n");
        return 1;
    }
    // THIS CONVERTS THE CONTENT LENGTH INTO INTEGERS.
    int len = atoi(content_length);
    if (len <= 0 || len >= sizeof(url)) {// IF ITS LESS THAN O OR MORE THAN THE SIVE I HAVE ALLOCATED 1024 ITS TOO BIG
        printf("Invalid CONTENT_LENGTH: %d\n", len);
        return 1;
    }

    // Read input from stdin
    fread(url, 1, len, stdin);
    url[len] = '\0';  // Null-terminator THIS PREVENTS BUFFERS AND OVERFLOWS

    // Debug: Print raw received data
    printf("Content-Type: text/html\n\n");
    printf("<p>Received Data: %s</p>\n", url);

    // Parse "url=INPUT"
    char *url_start = strstr(url, "url=");
    if (!url_start) {
        printf("<p>Invalid input! (Missing 'url=' key)</p>\n");
        return 1;
    }
    url_start += 4;  // Skip "url="

    // Copy and ensure null termination
    strncpy(url, url_start, sizeof(url) - 1);
    url[sizeof(url) - 1] = '\0';

    urldecode(url);  // Decode %20, %3A.

    // Calculate risk
    int risk_score = calculate_risk(url);//tenary operator below
    const char *risk_class = (risk_score >= 70) ? "risk-high" :
                             (risk_score >= 40) ? "risk-medium" : "risk-low";

    // Output HTML
    printf("<html>\n<head>\n<link rel='stylesheet' href='styles.css'>\n</head>\n");
    printf("<body>\n<div class='container'>\n");

    printf("<h2>Phishing Analysis Report</h2>\n");
    printf("<p><strong>URL:</strong> %s</p>\n", url);
    printf("<p><strong>Risk Score:</strong> <span class='%s'>%d/100</span></p>\n", risk_class, risk_score);

    printf("<ul>\n");
    printf("<li>HTTPS: %s</li>\n", has_https(url) ? "Yes (Good)" : "No (Risk!)");
    printf("<li>Suspicious Keywords: %s</li>\n", is_suspicious(url) ? "Detected!" : "None");
    printf("</ul>\n</div>\n</body>\n</html>\n");

    return 0;
}

// Check if URL uses HTTPS
int has_https(const char *url) {
    return strstr(url, "https://") != NULL;
}

// Check for suspicious keywords
int is_suspicious(const char *url) {
    const char *keywords[] = {"login", "verify", "bank", "paypal", "urgent"};
    for (int i = 0; i < 5; i++) {
        if (strstr(url, keywords[i]) != NULL) return 1;
    }
    return 0;
}

// Calculate risk score
int calculate_risk(const char *url) {
    int risk = 0;
    if (!has_https(url)) risk += 30;
    if (is_suspicious(url)) risk += 50;
    return risk;
}
