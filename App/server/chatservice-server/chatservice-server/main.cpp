#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <sstream>
#include <cstring>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

#define MAX_SIZE 1024 // Constant declaration
#define MAX_CLIENT 3  // Maximum number of clients is 3

struct SOCKET_INFO { // Structure definition
    int sck = 0;      // Socket number
    string user = ""; // User name
};

vector<SOCKET_INFO> sck_list; // Variable to store clients connected to the server.
SOCKET_INFO server_sock;      // Structure to store server socket information.
int client_count = 0;         // Used to count the number of currently connected clients.

void mainMenu();
void server_init();                           // Create socket for server
void add_client();                            // Client connection function
void send_msg(const char* msg);               // Function to send message to clients
void send_msg_notMe(const char* msg, int sender_idx);
void sendWhisper(int position, string sbuf, int idx);
void recv_msg(int idx);       // Receive message from client
void del_client(int idx);     // Function to disconnect from client
void print_clients();

int main() {
    system("echo \033[0;36m"); // Set terminal color (Mac)
    mainMenu();

    server_init(); // Initialize server

    std::thread th1[MAX_CLIENT];
    for (int i = 0; i < MAX_CLIENT; i++) {
        th1[i] = std::thread(add_client);
    }

    while (1) {
        string text, msg = "";
        std::getline(cin, text);
        const char* buf = text.c_str();
        msg = server_sock.user + " : " + buf;
        send_msg(msg.c_str());
    }

    for (int i = 0; i < MAX_CLIENT; i++) {
        th1[i].join();
    }

    close(server_sock.sck);

    return 0;
}

void mainMenu() {
    cout << "\n";
    cout << "*************************************************\n";
    cout << "*                                               *\n";
    cout << "*       *******      *       *       *  *       *\n";
    cout << "*          *        * *      *       * *        *\n";
    cout << "*          *       *****     *       **         *\n";
    cout << "*          *      *     *    *       * *        *\n";
    cout << "*          *     *       *   *****   *  *       *\n";
    cout << "*                                               *\n";
    cout << "*                 < SERVER ON >                 *\n";
    cout << "*                                               *\n";
    cout << "*************************************************\n\n";
}

void server_init() {
    server_sock.sck = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7777);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock.sck, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_sock.sck, SOMAXCONN);
    server_sock.user = "server";
}

void add_client() {
    sockaddr_in addr = {};
    socklen_t addrsize = sizeof(addr);
    char buf[MAX_SIZE] = {};

    SOCKET_INFO new_client = {};
    new_client.sck = accept(server_sock.sck, (struct sockaddr*)&addr, &addrsize);
    recv(new_client.sck, buf, MAX_SIZE, 0);
    new_client.user = string(buf);

    string msg = "▶" + new_client.user + " has entered.";
    cout << msg << endl;
    sck_list.push_back(new_client);
    print_clients();

    std::thread th(recv_msg, client_count);
    th.detach();
    client_count++;

    cout << "▷Current number of connections: " << client_count << endl;
    send_msg(msg.c_str());
}

void send_msg(const char* msg) {
    for (int i = 0; i < client_count; i++) {
        send(sck_list[i].sck, msg, MAX_SIZE, 0);
    }
}

void send_msg_notMe(const char* msg, int sender_idx) {
    for (int i = 0; i < client_count; i++) {
        if (i != sender_idx) {
            send(sck_list[i].sck, msg, MAX_SIZE, 0);
        }
    }
}

void sendWhisper(int position, string sbuf, int idx) {
    int cur_position = position + 1;
    position = sbuf.find(" ", cur_position);
    int len = position - cur_position;
    string receiver = sbuf.substr(cur_position, len);
    cur_position = position + 1;
    string dm = sbuf.substr(cur_position);
    string msg = "※Whisper received from [" + sck_list[idx].user + "] : " + dm;
    for (int i = 0; i < client_count; i++) {
        if (receiver.compare(sck_list[i].user) == 0) {
            send(sck_list[i].sck, msg.c_str(), MAX_SIZE, 0);
        }
    }
}

void recv_msg(int idx) {
    char buf[MAX_SIZE] = {};
    string msg = "";

    while (1) {
        memset(buf, 0, MAX_SIZE);

        if (recv(sck_list[idx].sck, buf, MAX_SIZE, 0) > 0) {
            string whisper(buf);
            int position = whisper.find(" ", 0);
            int message = position - 0;
            string flag = whisper.substr(0, message);
            if (string(buf) == "/exit") {
                msg = "▶" + sck_list[idx].user + " has left.";
                cout << msg << endl;
                send_msg(msg.c_str());
                del_client(idx);
                return;
            } else if (flag.compare("/whisper") == 0) {
                sendWhisper(position, whisper, idx);
            } else {
                msg += "--------------------------------------------------";
                msg += "\n▷Sender: " + sck_list[idx].user + "  " + "▷Time Sent: Soon\n";
                msg += "▷Content: " + whisper + "\n";
                msg += "--------------------------------------------------\n";
                cout << msg << endl;
                send_msg_notMe(msg.c_str(), idx);
            }
        } else {
            msg = "[Notice] " + sck_list[idx].user + " has left.";
            cout << msg << endl;
            send_msg(msg.c_str());
            del_client(idx);
            return;
        }
    }
}

void del_client(int idx) {
    std::thread th(add_client);
    close(sck_list[idx].sck);
    client_count--;
    cout << "▷Current number of connections: " << client_count << endl;
    sck_list.erase(sck_list.begin() + idx);
    th.join();
}

void print_clients() {
    cout << "▷Currently connected: ";
    for (auto& client : sck_list) {
        cout << client.user << " ";
    }
    cout << endl;
}
