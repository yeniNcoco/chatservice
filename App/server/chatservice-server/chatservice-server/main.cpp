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

#define MAX_SIZE 1024 // 경성 선언
#define MAX_CLIENT 3  // 최대 인원 3

struct SOCKET_INFO { // 구조체 정의
    int sck = 0; // 소켓 번호
    string user = ""; // 유저 이름
};

vector<SOCKET_INFO> sck_list; // 서버에 연결된 클라이언트를 저장할 변수.
SOCKET_INFO server_sock; // 서버 소켓의 정보를 저장할 구조체.
int client_count = 0; // 현재 접속된 클라이언트 수 카운트 용단.

void mainMenu();
void server_init(); // 서버용 소켓을 만든다
void add_client(); // 클라이언트 연결 함수
void send_msg(const char* msg); // 클라이언트로 메시지 전송 함수
void send_msg_notMe(const char* msg, int sender_idx);
void sendWhisper(int position, string sbuf, int idx);
void recv_msg(int idx); // 클라이언트로부터 메시지 수신
void del_client(int idx); // 클라이언트와의 연결을 끊는 함수
void print_clients();

int main() {
    system("echo \033[0;36m"); // 터미널 색상 설정 (Mac용)
    mainMenu();

    server_init(); // 서버 초기화

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

    string msg = "▶" + new_client.user + " 님이 입장하셨습니다.";
    cout << msg << endl;
    sck_list.push_back(new_client);
    print_clients();

    std::thread th(recv_msg, client_count);
    th.detach();
    client_count++;

    cout << "▷현재 접속자 수 : " << client_count << "명" << endl;
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
    string msg = "※귓속말 도착 [" + sck_list[idx].user + "] : " + dm;
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
            if (string(buf) == "/종료") {
                msg = "▶" + sck_list[idx].user + " 님이 퇴장하셨습니다.";
                cout << msg << endl;
                send_msg(msg.c_str());
                del_client(idx);
                return;
            } else if (flag.compare("/귓말") == 0) {
                sendWhisper(position, whisper, idx);
            } else {
                msg += "--------------------------------------------------";
                msg += "\n▷보낸사람: " + sck_list[idx].user + "  " + "▷보낸시간: 잠시후\n";
                msg += "▷내용 : " + whisper + "\n";
                msg += "--------------------------------------------------\n";
                cout << msg << endl;
                send_msg_notMe(msg.c_str(), idx);
            }
        } else {
            msg = "[공지] " + sck_list[idx].user + " 님이 퇴장하셨습니다.";
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
    cout << "▷현재 접속자 수 : " << client_count << "명" << endl;
    sck_list.erase(sck_list.begin() + idx);
    th.join();
}

void print_clients() {
    cout << "▷현재 접속 : ";
    for (auto& client : sck_list) {
        cout << client.user << " ";
    }
    cout << endl;
}

