#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <unistd.h>    // For usleep()
#include <termios.h>   // For getch()
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> // For inet_pton()
#include <netinet/in.h>
#include <netdb.h>     // For gethostbyname()
#include <cstring>     // For memset

using std::cout;
using std::cin;
using std::endl;
using std::string;

#define MAX_SIZE 1024

// File paths for storing user data and chat history
const string USER_DATA_FILE = "users.txt";
const string CHAT_HISTORY_FILE = "chat_history.txt";

int client_sock;
string my_nick;

// Function to emulate getch() in Unix systems
int getch() {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);          // Get terminal attributes
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);        // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr); // Set terminal attributes
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); // Restore terminal attributes
    return ch;
}

// Function to set text color using ANSI escape codes
void textColor(int foreground) {
    // Colors: 0=black, 1=red, 2=green, 3=yellow, 4=blue, 5=magenta, 6=cyan, 7=white
    int color_code = 30 + (foreground % 8);
    printf("\033[%dm", color_code);
}

// Function to reset text color
void resetColor() {
    printf("\033[0m");
}

// Function prototypes for menus
void startMenu();
void loginMenu();
void searchIdMenu();
void searchPwMenu();
void createUserMenu();
void mainMenu();
void myMenu();
void friendsMenu();
void chattingMenu();
void settingMenu();

// Class to represent a user
struct User {
    string id;
    string pw;
    string name;
    string phone;
    string birth;
    string status;
    string song;
};

// Class to manage users and perform operations
class UserManager {
private:
    std::vector<User> users;
    string currentUserId;
    string currentUserName;
public:
    UserManager() {
        loadUsers();
    }

    void loadUsers() {
        std::ifstream inFile(USER_DATA_FILE);
        if (!inFile) {
            cout << "User data file not found. Starting with empty user list." << endl;
            return;
        }
        string line;
        while (getline(inFile, line)) {
            User user;
            std::stringstream ss(line);
            getline(ss, user.id, '|');
            getline(ss, user.pw, '|');
            getline(ss, user.name, '|');
            getline(ss, user.phone, '|');
            getline(ss, user.birth, '|');
            getline(ss, user.status, '|');
            getline(ss, user.song, '|');
            users.push_back(user);
        }
        inFile.close();
    }

    void saveUsers() {
        std::ofstream outFile(USER_DATA_FILE);
        for (const auto& user : users) {
            outFile << user.id << "|" << user.pw << "|" << user.name << "|" << user.phone << "|" << user.birth << "|" << user.status << "|" << user.song << endl;
        }
        outFile.close();
    }

    int login();
    void searchId();
    void searchPw();
    void createUser();
    void myProfile();
    void updateStatus();
    void updateSong();
    void showFriends();
    void searchBirth();
    void searchContentMessage();
    void searchDayMessage();
    void modifyPw();
    int deleteUser();
    string getName();
    void beforeChat();
};

// Function to display start menu
void startMenu() {
    system("clear");
    cout << "\n";
    cout << " *************************************************\n";
    cout << " *                                               *\n";
    cout << " *       *******      *       *       *  *       *\n";
    cout << " *          *        * *      *       * *        *\n";
    cout << " *          *       *****     *       **         *\n";
    cout << " *          *      *     *    *       * *        *\n";
    cout << " *          *     *       *   *****   *  *       *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               < 시작 화면 >                     *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               1.로그인                          *\n";
    cout << " *                                               *\n";
    cout << " *               2. ID 찾기                       *\n";
    cout << " *                                               *\n";
    cout << " *               3. PW 찾기                       *\n";
    cout << " *                                               *\n";
    cout << " *               4. 회원가입                       *\n";
    cout << " *                                               *\n";
    cout << " *               0. 종료                          *\n";
    cout << " *                                               *\n";
    cout << " *************************************************\n\n";
}

// Function to display login menu
void loginMenu() {
    system("clear");
    cout << "\n";
    cout << " *************************************************\n";
    cout << " *                                               *\n";
    cout << " *       *******      *       *       *  *       *\n";
    cout << " *          *        * *      *       * *        *\n";
    cout << " *          *       *****     *       **         *\n";
    cout << " *          *      *     *    *       * *        *\n";
    cout << " *          *     *       *   *****   *  *       *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               < 로그인 화면 >                    *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *              >> 아이디 입력                      *\n";
    cout << " *                                               *\n";
    cout << " *              >> 비밀번호 입력                    *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *              >> 완료시 로그인!                   *\n";
    cout << " *                                               *\n";
    cout << " *              >> 실패시 메인화면                   *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *************************************************\n\n";
}

// Function to display ID search menu
void searchIdMenu() {
    system("clear");
    cout << "\n";
    cout << " *************************************************\n";
    cout << " *                                               *\n";
    cout << " *       *******      *       *       *  *       *\n";
    cout << " *          *        * *      *       * *        *\n";
    cout << " *          *       *****     *       **         *\n";
    cout << " *          *      *     *    *       * *        *\n";
    cout << " *          *     *       *   *****   *  *       *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               < 아이디 찾기 >                    *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               >> 이름 입력                      *\n";
    cout << " *                                               *\n";
    cout << " *               >> 번호 입력                      *\n";
    cout << " *                                               *\n";
    cout << " *               >> 생년월일(8자리) 입력             *\n";
    cout << " *                                               *\n";
    cout << " *               >> 완료시 아이디 출력               *\n";
    cout << " *                                               *\n";
    cout << " *               >> 실패시 메인화면                 *\n";
    cout << " *                                               *\n";
    cout << " *************************************************\n\n";
}

// Function to display password search menu
void searchPwMenu() {
    system("clear");
    cout << "\n";
    cout << " *************************************************\n";
    cout << " *                                               *\n";
    cout << " *       *******      *       *       *  *       *\n";
    cout << " *          *        * *      *       * *        *\n";
    cout << " *          *       *****     *       **         *\n";
    cout << " *          *      *     *    *       * *        *\n";
    cout << " *          *     *       *   *****   *  *       *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               < 비밀번호 찾기 >               *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               >> 아이디 입력                  *\n";
    cout << " *                                               *\n";
    cout << " *               >> 이름 입력                    *\n";
    cout << " *                                               *\n";
    cout << " *               >> 전화번호 입력                *\n";
    cout << " *                                               *\n";
    cout << " *               >> 생년월일(8자리) 입력         *\n";
    cout << " *                                               *\n";
    cout << " *               >> 완료시 메인화면              *\n";
    cout << " *                                               *\n";
    cout << " *************************************************\n\n";
}

// Function to display user creation menu
void createUserMenu() {
    system("clear");
    cout << "\n";
    cout << " *************************************************\n";
    cout << " *                                               *\n";
    cout << " *       *******      *       *       *  *       *\n";
    cout << " *          *        * *      *       * *        *\n";
    cout << " *          *       *****     *       **         *\n";
    cout << " *          *      *     *    *       * *        *\n";
    cout << " *          *     *       *   *****   *  *       *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               <  회원 가입  >                 *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               >> 아이디 입력                  *\n";
    cout << " *                                               *\n";
    cout << " *               >> 비밀번호 입력                *\n";
    cout << " *                                               *\n";
    cout << " *               >> 이름, 전화번호 입력          *\n";
    cout << " *                                               *\n";
    cout << " *               >> 생년월일(8자리) 입력         *\n";
    cout << " *                                               *\n";
    cout << " *               >> 완료시 메인화면              *\n";
    cout << " *                                               *\n";
    cout << " *************************************************\n\n";
}

// Function to display main menu
void mainMenu() {
    usleep(500 * 1000);
    system("clear");
    cout << "\n";
    cout << " *************************************************\n";
    cout << " *                                               *\n";
    cout << " *       *******      *       *       *  *       *\n";
    cout << " *          *        * *      *       * *        *\n";
    cout << " *          *       *****     *       **         *\n";
    cout << " *          *      *     *    *       * *        *\n";
    cout << " *          *     *       *   *****   *  *       *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *              < 현재 상태 : 접속 중 >          *\n";
    cout << " *                                               *\n";
    cout << " *                1. 내 정보                     *\n";
    cout << " *                                               *\n";
    cout << " *                2. 친구                        *\n";
    cout << " *                                               *\n";
    cout << " *                3. 대화방                      *\n";
    cout << " *                                               *\n";
    cout << " *                4. 설정                        *\n";
    cout << " *                                               *\n";
    cout << " *                0. 종료                        *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *************************************************\n\n";
}

// Function to display "My Menu"
void myMenu() {
    system("clear");
    cout << "\n";
    cout << " *************************************************\n";
    cout << " *                                               *\n";
    cout << " *       *******      *       *       *  *       *\n";
    cout << " *          *        * *      *       * *        *\n";
    cout << " *          *       *****     *       **         *\n";
    cout << " *          *      *     *    *       * *        *\n";
    cout << " *          *     *       *   *****   *  *       *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               < 내 정보 보기 >                *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *              1. 내 프로필                     *\n";
    cout << " *                                               *\n";
    cout << " *              2. 상메 설정/수정                *\n";
    cout << " *                                               *\n";
    cout << " *              3. BGM 설정/수정                 *\n";
    cout << " *                                               *\n";
    cout << " *              0. 뒤로가기                      *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *************************************************\n\n";
}

// Function to display friends menu
void friendsMenu() {
    system("clear");
    cout << "\n";
    cout << " *************************************************\n";
    cout << " *                                               *\n";
    cout << " *       *******      *       *       *  *       *\n";
    cout << " *          *        * *      *       * *        *\n";
    cout << " *          *       *****     *       **         *\n";
    cout << " *          *      *     *    *       * *        *\n";
    cout << " *          *     *       *   *****   *  *       *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               < 내 친구 정보 >                *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               1. 친구 목록 보기               *\n";
    cout << " *                                               *\n";
    cout << " *               2. 친구 생일 검색               *\n";
    cout << " *                                               *\n";
    cout << " *               >> 월일 ~ 월일 검색             *\n";
    cout << " *                                               *\n";
    cout << " *               0. 뒤로가기                     *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *************************************************\n\n";
}

// Function to display chatting menu
void chattingMenu() {
    system("clear");
    cout << "\n";
    cout << " *************************************************\n";
    cout << " *                                               *\n";
    cout << " *       *******      *       *       *  *       *\n";
    cout << " *          *        * *      *       * *        *\n";
    cout << " *          *       *****     *       **         *\n";
    cout << " *          *      *     *    *       * *        *\n";
    cout << " *          *     *       *   *****   *  *       *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               < 채팅 입장/검색 >              *\n";
    cout << " *                                               *\n";
    cout << " *               1. 채팅방 입장                  *\n";
    cout << " *                                               *\n";
    cout << " *               >> /귓말 이름 내용입력          *\n";
    cout << " *                                               *\n";
    cout << " *               >> /종료 입력시 퇴장            *\n";
    cout << " *                                               *\n";
    cout << " *               2. 채팅 내용 검색               *\n";
    cout << " *                                               *\n";
    cout << " *               3. 채팅 기간 검색               *\n";
    cout << " *                                               *\n";
    cout << " *               0. 뒤로가기                     *\n";
    cout << " *************************************************\n\n";
}

// Function to display settings menu
void settingMenu() {
    system("clear");
    cout << "\n";
    cout << " *************************************************\n";
    cout << " *                                               *\n";
    cout << " *       *******      *       *       *  *       *\n";
    cout << " *          *        * *      *       * *        *\n";
    cout << " *          *       *****     *       **         *\n";
    cout << " *          *      *     *    *       * *        *\n";
    cout << " *          *     *       *   *****   *  *       *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               < 내 설정 >                     *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               1. 비밀번호 변경                *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               2. 회원 탈퇴                    *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               0. 뒤로가기                     *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *************************************************\n\n";
}

// Function to receive chat messages
int chat_recv() {
    char buf[MAX_SIZE] = { };
    string msg;

    while (1) {
        memset(buf, 0, MAX_SIZE);
        ssize_t bytes_received = recv(client_sock, buf, MAX_SIZE, 0);
        if (bytes_received > 0) {
            textColor(7);
            msg = buf;
            //닉네임 : 메시지
            std::stringstream ss(msg);  // 문자열을 스트림화
            string user;
            ss >> user;
            if (user != my_nick) cout << buf << endl;
            resetColor();
        }
        else {
            cout << "Server Off" << endl;
            return -1;
        }
    }
}

// Implementation of UserManager methods

int UserManager::login() {
    cout << ">>아이디 : ";
    string id;
    cin >> id;
    cout << ">>비밀번호를 입력해주세요 : ";
    string pw;
    cin >> pw;
    char ch = ' ';
    while ((ch = getch()) != '\n' && ch != '\r') {
        if (ch == 127 || ch == 8) { // Backspace
            if (!pw.empty()) {
                pw.pop_back();
                cout << "\b \b";
            }
        } else {
            pw.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;

    for (const auto& user : users) {
        if (user.id == id && user.pw == pw) {
            currentUserId = id;
            currentUserName = user.name;
            cout << endl << "▶로그인 중입니다. 잠시만 기다려주세요." << endl << endl;
            cout << "▶" << user.name << "님 환영합니다." << endl;
            return 1;
        }
    }
    cout << "▶해당하는 정보가 없습니다." << endl;
    return 0;
}

void UserManager::searchId() {
    cout << ">>이름 : ";
    string name;
    cin >> name;
    cout << ">>전화번호 : ";
    string phone;
    cin >> phone;
    string birth;
    while (true) {
        cout << ">>생년월일 : ";
        cin >> birth;
        if (birth.length() != 8) {
            cout << "▶생년월일은 8자리로 입력해주세요." << endl;
            continue;
        }
        break;
    }

    for (const auto& user : users) {
        if (user.name == name && user.phone == phone && user.birth == birth) {
            cout << "▶" << name << "님의 아이디는 " << user.id << "입니다." << endl;
            usleep(3000 * 1000);
            return;
        }
    }
    cout << "▶해당하는 정보가 없습니다.!" << endl;
    usleep(500 * 1000);
}

void UserManager::searchPw() {
    cout << ">>아이디 : ";
    string id;
    cin >> id;
    cout << ">>이름 : ";
    string name;
    cin >> name;
    cout << ">>전화번호 : ";
    string phone;
    cin >> phone;
    string birth;
    while (true) {
        cout << ">>생년월일 : ";
        cin >> birth;
        if (birth.length() != 8) {
            cout << "▶생년월일은 8자리로 입력해주세요." << endl;
            continue;
        }
        break;
    }

    for (auto& user : users) {
        if (user.id == id && user.name == name && user.phone == phone && user.birth == birth) {
            while (1) {
                string new_pw = "";
                cout << ">>새로운 비밀번호를 입력해주세요 : ";
//                char ch = ' ';
//                while ((ch = getch()) != '\n' && ch != '\r') {
//                    if (ch == 127 || ch == 8) { // Backspace
//                        if (!new_pw.empty()) {
//                            new_pw.pop_back();
//                            cout << "\b \b";
//                        }
//                    } else {
//                        new_pw.push_back(ch);
//                        cout << '*';
//                    }
//                }
                cin >> new_pw;
                cout << endl;

                string renew_pw = "";
                cout << ">>다시 한번 입력해주세요. : ";
                cin >> renew_pw;
//                while ((ch = getch()) != '\n' && ch != '\r') {
//                    if (ch == 127 || ch == 8) { // Backspace
//                        if (!renew_pw.empty()) {
//                            renew_pw.pop_back();
//                            cout << "\b \b";
//                        }
//                    } else {
//                        renew_pw.push_back(ch);
//                        cout << '*';
//                    }
//                }
                cout << endl;

                if (new_pw == renew_pw) {
                    user.pw = new_pw;
                    saveUsers();
                    printf("▶새로운 비밀번호로 변경되었습니다.\n");
                    usleep(500 * 1000);
                    return;
                } else {
                    cout << "▶비밀번호가 다릅니다." << endl;
                    usleep(500 * 1000);
                }
            }
        }
    }
    cout << "▶해당하는 정보가 없습니다." << endl;
    usleep(500 * 1000);
}

void UserManager::createUser() {
    string id, pw, name, phone, birth;
    while (1) {
        cout << ">>아이디 : ";
        cin >> id;
        bool idExists = false;
        for (const auto& user : users) {
            if (user.id == id) {
                cout << "▶중복된 아이디가 있습니다." << endl;
                idExists = true;
                break;
            }
        }
        if (!idExists) {
            cout << "▶중복체크 완료." << endl;
            break;
        }
    }
    while (1) {
        cout << ">>비밀번호를 입력해주세요 : ";
        pw = "";
        cin >> pw;
//        char ch = ' ';
//        while ((ch = getch()) != '\n' && ch != '\r') {
//            if (ch == 127 || ch == 8) { // Backspace
//                if (!pw.empty()) {
//                    pw.pop_back();
//                    cout << "\b \b";
//                }
//            } else {
//                pw.push_back(ch);
//                cout << '*';
//            }
//        }
        cout << endl;
        string new_pw = "";
        cout << ">>다시 한번 입력해주세요. : ";
        cin >> new_pw;
//        while ((ch = getch()) != '\n' && ch != '\r') {
//            if (ch == 127 || ch == 8) { // Backspace
//                if (!new_pw.empty()) {
//                    new_pw.pop_back();
//                    cout << "\b \b";
//                }
//            } else {
//                new_pw.push_back(ch);
//                cout << '*';
//            }
//        }
        cout << endl;
        if (pw == new_pw) {
            break;
        } else {
            cout << "▶비밀번호가 다릅니다." << endl;
        }
    }
    cout << ">>이름 : ";
    cin >> name;
    cout << ">>전화번호 : ";
    cin >> phone;
    while (true) {
        cout << ">>생년월일 : ";
        cin >> birth;
        if (birth.length() != 8) {
            cout << "▶생년월일은 8자리로 입력해주세요." << endl;
            continue;
        }
        break;
    }
    User newUser = { id, pw, name, phone, birth, "", "" };
    users.push_back(newUser);
    saveUsers();
    cout << "▶회원가입이 완료되었습니다." << endl;
    usleep(500 * 1000);
}

void UserManager::myProfile() {
    for (const auto& user : users) {
        if (user.id == currentUserId) {
            cout << "-----------------------------------------" << endl;
            cout << "▷이름 : " << user.name << endl;
            if (user.status == "") {
                cout << "▷상메 : 없음" << endl;
            } else {
                cout << "▷상메 : " << user.status << endl;
            }
            if (user.song == "") {
                cout << "▷노래 : 없음" << endl;
            } else {
                cout << "▷노래 : " << user.song << endl;
            }
            cout << "▷생일 : " << user.birth << endl;
            cout << "▷번호 : " << user.phone << endl;
            cout << "-----------------------------------------" << endl;
            return;
        }
    }
}

void UserManager::updateStatus() {
    cout << ">>상태메시지 입력 : ";
    string status;
    cin.ignore();
    getline(cin, status);
    for (auto& user : users) {
        if (user.id == currentUserId) {
            user.status = status;
            saveUsers();
            printf("▶업데이트 되었습니다.\n");
            return;
        }
    }
}

void UserManager::updateSong() {
    cout << ">>노래 입력 : ";
    string song;
    cin.ignore();
    getline(cin, song);
    for (auto& user : users) {
        if (user.id == currentUserId) {
            user.song = song;
            saveUsers();
            printf("▶업데이트 되었습니다.\n");
            return;
        }
    }
}

void UserManager::showFriends() {
    for (const auto& user : users) {
        if (user.id != currentUserId) {
            cout << "-----------------------------" << endl;
            cout << "▷이름 : " << user.name << endl;
            if (user.status == "") {
                cout << "▷상메 : 없음" << endl;
            } else {
                cout << "▷상메 : " << user.status << endl;
            }
            if (user.song == "") {
                cout << "▷노래 : 없음" << endl;
            } else {
                cout << "▷노래 : " << user.song << endl;
            }
            cout << "▷생일 : " << user.birth << endl;
            cout << "▷번호 : " << user.phone << endl;
        }
    }
}

void UserManager::searchBirth() {
    string startDay, endDay;
    cout << "시작월일 4자리를 입력해주세요.(ex.0201) : ";
    cin >> startDay;
    cout << "종료월일 4자리를 입력해주세요.(ex.0405) : ";
    cin >> endDay;
    bool found = false;
    for (const auto& user : users) {
        if (user.id != currentUserId) {
            string userMonthDay = user.birth.substr(4, 4);
            if (userMonthDay >= startDay && userMonthDay <= endDay) {
                cout << "-----------------------------------------------" << endl;
                cout << "▷이름 : " << user.name << endl;
                cout << "▷생일 : " << user.birth << endl;
                cout << "▷번호 : " << user.phone << endl;
                found = true;
            }
 
