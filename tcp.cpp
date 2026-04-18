#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
using namespace std;
#define PORT 8080
#define BUFFER_SIZE 1024


void handle_request(int client_sockfd) {   
    char buf[BUFFER_SIZE];
    ssize_t recv_len = recv(client_sockfd, buf, BUFFER_SIZE, 0); // client'ten gelen mesajı alıyoruz
    if (recv_len < 0) {
        cerr << "Mesaj alınamadı" << endl;
        return;
    }
    cout << "Mesaj alındı: " << string(buf, (size_t)recv_len) << endl;
    string request = string(buf, (size_t)recv_len);
    cout << "Request: " << request << endl;
    string first_line = request.substr(0, request.find("\r\n"));
    size_t first_space = first_line.find(" ");
    size_t second_space = first_line.find(" ", first_space + 1);

    string method  = first_line.substr(0, first_space);
    string path    = first_line.substr(first_space + 1, second_space - first_space - 1);
    string version = first_line.substr(second_space + 1);

    cout << "Method: "  << method  << endl;
    cout << "Path: "    << path    << endl;
    cout << "Version: " << version << endl;
    string body = "<h1>Merhaba Dünya!</h1>";
    string status = "200 OK";
    string response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + to_string(body.size()) + "\r\n"
        "\r\n" +
        body;

    send(client_sockfd, response.c_str(), response.size(), 0);
    if (close(client_sockfd) < 0) {
        cerr << "Client socket kapanamadı" << endl;
        return;
    }
    
}

int create_server() {
    int domain = AF_INET; // IPv4 adres family'i seçiyoruz
    int type = SOCK_STREAM; // TCP , SOCK_DGRAM = UDP
    int protocol = 0; // type'a göre uygun olanı seçiyoruz
    int sockfd = socket(domain, type, protocol);
    if (sockfd < 0) {
        cerr << "Socket oluşturulamadı" << endl;
        return -1;
    }
    int opt = 1;
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = domain;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // socket'ın adresini tekrar kullanabiliriz
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) { // socket'ın adresini bağlıyoruz
        cerr << "Bind başarısız" << endl;
        return -1;
    }   
    if (listen(sockfd, 5) < 0) { // 5 client'ı bekliyoruz
        cerr << "Listen başarısız" << endl;
        return -1;
    }
    return sockfd;
}

int main() {   
    int sockfd = create_server();
    while (true) {
        int client_sockfd = accept(sockfd, nullptr, nullptr); // client'ın bağlantısını kabul ediyoruz
        if (client_sockfd < 0) {
            cerr << "Bağlantı kabul edilmedi" << endl;
            continue;
        }
        cout << "Bağlantı kabul edildi, client socket: " << client_sockfd << endl;
        thread t(handle_request, client_sockfd); // client'ın mesajını işlemeye başlıyoruz
        t.detach(); // thread'ın çalışmasını bırakıyoruz ve main thread'ın çalışmasına izin veriyoruz. detach ile gelen multi thread'leri main thread'den ayırıyoruz.Favicon ico isteği chrome otomatik olarak atıyor.
    }
    if (close(sockfd) < 0) { // server socket'ını kapatıyoruz
        cerr << "Server socket kapanamadı" << endl;   
        return 1;
    }
    return 0;
}