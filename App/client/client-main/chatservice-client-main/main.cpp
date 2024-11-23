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
    cout << " *               < Start Screen >                *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               1. Login                        *\n";
    cout << " *                                               *\n";
    cout << " *               2. Find ID                      *\n";
    cout << " *                                               *\n";
    cout << " *               3. Find Password                *\n";
    cout << " *                                               *\n";
    cout << " *               4. Sign Up                      *\n";
    cout << " *                                               *\n";
    cout << " *               0. Exit                         *\n";
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
    cout << " *               < Login Screen >                *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *              >> Enter ID                      *\n";
    cout << " *                                               *\n";
    cout << " *              >> Enter Password                *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *              >> Login upon completion!        *\n";
    cout << " *                                               *\n";
    cout << " *              >> Return to main if failed      *\n";
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
    cout << " *               < Find ID >                     *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               >> Enter Name                   *\n";
    cout << " *                                               *\n";
    cout << " *               >> Enter Phone Number           *\n";
    cout << " *                                               *\n";
    cout << " *               >> Enter Birthdate (8 digits)   *\n";
    cout << " *                                               *\n";
    cout << " *               >> ID displayed upon success    *\n";
    cout << " *                                               *\n";
    cout << " *               >> Return to main if failed     *\n";
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
    cout << " *               < Find Password >               *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               >> Enter ID                     *\n";
    cout << " *                                               *\n";
    cout << " *               >> Enter Name                   *\n";
    cout << " *                                               *\n";
    cout << " *               >> Enter Phone Number           *\n";
    cout << " *                                               *\n";
    cout << " *               >> Enter Birthdate (8 digits)   *\n";
    cout << " *                                               *\n";
    cout << " *               >> Return to main upon success  *\n";
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
    cout << " *               < Sign Up >                     *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               >> Enter Name, Phone Number     *\n";
    cout << " *                                               *\n";
    cout << " *               >> Enter Birthdate (8 digits)   *\n";
    cout << " *                                               *\n";
    cout << " *               >> Enter ID                     *\n";
    cout << " *                                               *\n";
    cout << " *               >> Enter Password               *\n";
    cout << " *                                               *\n";
    cout << " *               >> Return to main upon success  *\n";
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
    cout << " *              < Status: Connected >            *\n";
    cout << " *                                               *\n";
    cout << " *                1. My Info                     *\n";
    cout << " *                                               *\n";
    cout << " *                2. Friends                     *\n";
    cout << " *                                               *\n";
    cout << " *                3. Chat Room                   *\n";
    cout << " *                                               *\n";
    cout << " *                4. Settings                    *\n";
    cout << " *                                               *\n";
    cout << " *                0. Exit                        *\n";
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
    cout << " *               < View My Info >                *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *              1. My Profile                    *\n";
    cout << " *                                               *\n";
    cout << " *              2. Set/Update Status Msg         *\n";
    cout << " *                                               *\n";
    cout << " *              3. Set/Update BGM                *\n";
    cout << " *                                               *\n";
    cout << " *              0. Go Back                       *\n";
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
    cout << " *               < My Friends Info >             *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               1. View Friend List             *\n";
    cout << " *                                               *\n";
    cout << " *               2. Search Birthdays             *\n";
    cout << " *                                               *\n";
    cout << " *               >> Search from MMDD to MMDD     *\n";
    cout << " *                                               *\n";
    cout << " *               0. Go Back                      *\n";
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
    cout << " *               < Enter/Search Chat >           *\n";
    cout << " *                                               *\n";
    cout << " *               1. Enter Chat Room              *\n";
    cout << " *                                               *\n";
    cout << " *               >> /whisper name message        *\n";
    cout << " *                                               *\n";
    cout << " *               >> Type /exit to leave          *\n";
    cout << " *                                               *\n";
    cout << " *               2. Search Chat Content          *\n";
    cout << " *                                               *\n";
    cout << " *               3. Search Chat by Date          *\n";
    cout << " *                                               *\n";
    cout << " *               0. Go Back                      *\n";
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
    cout << " *               < My Settings >                 *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               1. Change Password              *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               2. Delete Account               *\n";
    cout << " *                                               *\n";
    cout << " *                                               *\n";
    cout << " *               0. Go Back                      *\n";
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
            // Format: nickname : message
            std::stringstream ss(msg);
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
    cout << ">>ID: ";
    string id;
    cin >> id;
    cout << ">>Please enter your password: ";
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
            cout << endl << "▶Logging in. Please wait a moment." << endl << endl;
            cout << "▶Welcome, " << user.name << "." << endl;
            return 1;
        }
    }
    cout << "▶No matching information found." << endl;
    return 0;
}

void UserManager::searchId() {
    cout << ">>Name: ";
    string name;
    cin >> name;
    cout << ">>Phone Number: ";
    string phone;
    cin >> phone;
    string birth;
    while (true) {
        cout << ">>Birthdate: ";
        cin >> birth;
        if (birth.length() != 8) {
            cout << "▶Please enter birthdate in 8 digits." << endl;
            continue;
        }
        break;
    }
    
    for (const auto& user : users) {
        if (user.name == name && user.phone == phone && user.birth == birth) {
            cout << "▶" << name << "'s ID is " << user.id << "." << endl;
            usleep(3000 * 1000);
            return;
        }
    }
    cout << "▶No matching information found!" << endl;
    usleep(500 * 1000);
}

void UserManager::searchPw() {
    cout << ">>ID: ";
    string id;
    cin >> id;
    cout << ">>Name: ";
    string name;
    cin >> name;
    cout << ">>Phone Number: ";
    string phone;
    cin >> phone;
    string birth;
    while (true) {
        cout << ">>Birthdate: ";
        cin >> birth;
        if (birth.length() != 8) {
            cout << "▶Please enter birthdate in 8 digits." << endl;
            continue;
        }
        break;
    }
    
    for (auto& user : users) {
        if (user.id == id && user.name == name && user.phone == phone && user.birth == birth) {
            while (1) {
                string new_pw = "";
                cout << ">>Please enter a new password: ";
                cin >> new_pw;
                cout << endl;
                
                string renew_pw = "";
                cout << ">>Please re-enter to confirm: ";
                cin >> renew_pw;
                cout << endl;
                
                if (new_pw == renew_pw) {
                    user.pw = new_pw;
                    saveUsers();
                    printf("▶Password has been changed to the new password.\n");
                    usleep(500 * 1000);
                    return;
                } else {
                    cout << "▶Passwords do not match." << endl;
                    usleep(500 * 1000);
                }
            }
        }
    }
    cout << "▶No matching information found." << endl;
    usleep(500 * 1000);
}

void UserManager::createUser() {
    string id, pw, name, phone, birth;
    
    while (1) {
        cout << ">>Name: ";
        cin >> name;
        cout << ">>Phone Number: ";
        cin >> phone;
        bool phoneExists = false;
        for (const auto& user : users) {
            if (user.phone == phone) {
                cout << "▶An account already exists with the same phone number." << endl;
                phoneExists = true;
                break;
            }
        }
        if (phoneExists) {
            continue;
        }
        else {
            break;
        }
    }
        
    while (1) {
        cout << ">>Birthdate: ";
        cin >> birth;
        if (birth.length() != 8) {
            cout << "▶Please enter birthdate in 8 digits." << endl;
            continue;
        }
        break;
    }

    while (1) {
        cout << ">>ID: ";
        cin >> id;
        bool idExists = false;
        for (const auto& user : users) {
            if (user.id == id) {
                cout << "▶This ID is already taken." << endl;
                idExists = true;
                break;
            }
        }
        if (!idExists) {
            cout << "▶ID is available." << endl;
            break;
        }
    }
        

    while (1) {
        cout << ">>Please enter a password: ";
        pw = "";
        cin >> pw;
        cout << endl;
        string new_pw = "";
        cout << ">>Please re-enter to confirm: ";
        cin >> new_pw;
        cout << endl;
        if (pw == new_pw) {
            break;
        } else {
            cout << "▶Passwords do not match." << endl;
        }
    }
    User newUser = { id, pw, name, phone, birth, "", "" };
    users.push_back(newUser);
    saveUsers();
    cout << "▶Sign up completed." << endl;
    usleep(500 * 1000);
}

void UserManager::myProfile() {
    for (const auto& user : users) {
        if (user.id == currentUserId) {
            cout << "-----------------------------------------" << endl;
            cout << "▷Name: " << user.name << endl;
            if (user.status == "") {
                cout << "▷Status Msg: None" << endl;
            } else {
                cout << "▷Status Msg: " << user.status << endl;
            }
            if (user.song == "") {
                cout << "▷Song: None" << endl;
            } else {
                cout << "▷Song: " << user.song << endl;
            }
            cout << "▷Birthdate: " << user.birth << endl;
            cout << "▷Phone: " << user.phone << endl;
            cout << "-----------------------------------------" << endl;
            return;
        }
    }
}

void UserManager::updateStatus() {
    cout << ">>Enter status message: ";
    string status;
    cin.ignore();
    getline(cin, status);
    for (auto& user : users) {
        if (user.id == currentUserId) {
            user.status = status;
            saveUsers();
            printf("▶Updated successfully.\n");
            return;
        }
    }
}

void UserManager::updateSong() {
    cout << ">>Enter song: ";
    string song;
    cin.ignore();
    getline(cin, song);
    for (auto& user : users) {
        if (user.id == currentUserId) {
            user.song = song;
            saveUsers();
            printf("▶Updated successfully.\n");
            return;
        }
    }
}

void UserManager::showFriends() {
    for (const auto& user : users) {
        if (user.id != currentUserId) {
            cout << "-----------------------------" << endl;
            cout << "▷Name: " << user.name << endl;
            if (user.status == "") {
                cout << "▷Status Msg: None" << endl;
            } else {
                cout << "▷Status Msg: " << user.status << endl;
            }
            if (user.song == "") {
                cout << "▷Song: None" << endl;
            } else {
                cout << "▷Song: " << user.song << endl;
            }
            cout << "▷Birthdate: " << user.birth << endl;
            cout << "▷Phone: " << user.phone << endl;
        }
    }
}

void UserManager::searchBirth() {
    string startDay, endDay;
    cout << "Please enter the start month and day (4 digits, e.g., 0201): ";
    cin >> startDay;
    cout << "Please enter the end month and day (4 digits, e.g., 0405): ";
    cin >> endDay;
    bool found = false;
    for (const auto& user : users) {
        if (user.id != currentUserId) {
            string userMonthDay = user.birth.substr(4, 4);
            if (userMonthDay >= startDay && userMonthDay <= endDay) {
                cout << "-----------------------------------------------" << endl;
                cout << "▷Name: " << user.name << endl;
                cout << "▷Birthdate: " << user.birth << endl;
                cout << "▷Phone: " << user.phone << endl;
                found = true;
            }
        }
    }
    if (!found) {
        cout << "No results found." << endl;
    }
}

void UserManager::searchContentMessage() {
    string content;
    cout << ">>Search messages by content: ";
    cin >> content;
    std::ifstream inFile(CHAT_HISTORY_FILE);
    if (!inFile) {
        cout << "No chat history." << endl;
        return;
    }
    string line;
    bool found = false;
    while (getline(inFile, line)) {
        if (line.find(content) != string::npos) {
            cout << line << endl;
            found = true;
        }
    }
    inFile.close();
    if (!found) {
        cout << "No results found." << endl;
    }
}

void UserManager::searchDayMessage() {
    string startDay, endDay;
    cout << "Please enter the start month and day (4 digits, e.g., 0201): ";
    cin >> startDay;
    cout << "Please enter the end month and day (4 digits, e.g., 0405): ";
    cin >> endDay;
    std::ifstream inFile(CHAT_HISTORY_FILE);
    if (!inFile) {
        cout << "No chat history." << endl;
        return;
    }
    string line;
    bool found = false;
    while (getline(inFile, line)) {
        // Assuming the date is in the format YYYYMMDD at the beginning of the line
        string date = line.substr(0, 8);
        string monthDay = date.substr(4, 4);
        if (monthDay >= startDay && monthDay <= endDay) {
            cout << line << endl;
            found = true;
        }
    }
    inFile.close();
    if (!found) {
        cout << "No results found." << endl;
    }
}

void UserManager::modifyPw() {
    cout << ">>Please enter your current password: ";
    string existPw = "";
    cin >> existPw;
    cout << endl;
    
    for (auto& user : users) {
        if (user.id == currentUserId && user.pw == existPw) {
            while (1) {
                string new_pw = "";
                cout << ">>Please enter a new password: ";
                cin >> new_pw;
                cout << endl;
                
                string renew_pw = "";
                cout << ">>Please re-enter to confirm: ";
                cin >> renew_pw;
                cout << endl;
                
                if (new_pw == renew_pw) {
                    user.pw = new_pw;
                    saveUsers();
                    printf("▶Password has been changed to the new password.\n");
                    break;
                } else {
                    cout << "▶Passwords do not match." << endl;
                }
            }
            return;
        }
    }
    cout << "▶Incorrect password." << endl;
}

int UserManager::deleteUser() {
    cout << ">>Please enter your current password: ";
    string existPw = "";
    cin >> existPw;
    cout << endl;
    
    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->id == currentUserId && it->pw == existPw) {
            cout << "Are you sure you want to delete your account? This cannot be undone. 1. Continue, 2. Cancel: ";
            char lastCheck;
            cin >> lastCheck;
            if (lastCheck == '1') {
                users.erase(it);
                saveUsers();
                cout << "▶Thank you for using our service. Please come again." << endl;
                return 1;
            } else if (lastCheck == '2') {
                cout << "Good decision." << endl;
                return 2;
            } else {
                cout << "▶Invalid input." << endl;
                return 2;
            }
        }
    }
    cout << "▶Incorrect password." << endl;
    return 2;
}

string UserManager::getName() {
    return currentUserName;
}

void UserManager::beforeChat() {
    std::ifstream inFile(CHAT_HISTORY_FILE);
    if (!inFile) {
        cout << "No chat history." << endl;
        return;
    }
    std::vector<string> lines;
    string line;
    while (getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();
    int num = lines.size();
    for (int i = num - 5; i < num; ++i) {
        if (i >= 0) {
            cout << lines[i] << endl;
        }
    }
}

int main() {
initialize:
    UserManager userManager;
    bool loginSuccess = false;
    
    // Start screen
    while (!loginSuccess) {
        startMenu();
        char startIn = 0;
        cout << "▶ ";
        cin >> startIn;
        switch (startIn) {
            case '1': // Login
                loginMenu();
                if (userManager.login() == 1) {
                    loginSuccess = true;
                    break;
                }
                continue;
            case '2': // Search ID
                searchIdMenu();
                userManager.searchId();
                continue;
            case '3': // Search Password
                searchPwMenu();
                userManager.searchPw();
                continue;
            case '4': // Create User
                createUserMenu();
                userManager.createUser();
                continue;
            case '0': // Exit
                return -1;
            default:
                cout << "▶Invalid input. Please try again." << endl;
                continue;
        }
    }
    
    // Main menu
    while (1) {
        mainMenu();
        int mainIn = 0;
        cout << "▶ ";
        cin >> mainIn;
        
        if (mainIn == 1) { // My Info
            myMenu();
            bool backButton = false;
            while (!backButton) {
                char informationIn = 0;
                cout << "▶ ";
                cin >> informationIn;
                switch (informationIn) {
                    case '1':
                        userManager.myProfile();
                        break;
                    case '2':
                        userManager.updateStatus();
                        continue;
                    case '3':
                        userManager.updateSong();
                        continue;
                    case '0':
                        backButton = true;
                        break;
                    default:
                        cout << "▶Invalid input. Please try again." << endl;
                        continue;
                }
            }
        }
        else if (mainIn == 2) { // Friends
            friendsMenu();
            bool backButton = false;
            while (!backButton) {
                char friendsIn = 0;
                cout << "▶ ";
                cin >> friendsIn;
                switch (friendsIn) {
                    case '1':
                        userManager.showFriends();
                        continue;
                    case '2':
                        userManager.searchBirth();
                        continue;
                    case '0':
                        backButton = true;
                        break;
                    default:
                        cout << "▶Invalid input. Please try again." << endl;
                        continue;
                }
            }
        }
        else if (mainIn == 3) { // Chatting
            chattingMenu();
            bool backButton = false;
            while (!backButton) {
                char chattingIn = 0;
                cout << "▶ ";
                cin >> chattingIn;
                switch (chattingIn) {
                    case '1':{
                        userManager.beforeChat();
                        client_sock = socket(AF_INET, SOCK_STREAM, 0);
                        if (client_sock < 0) {
                            cout << "Socket creation error" << endl;
                            return -1;
                        }
                        struct sockaddr_in serv_addr;
                        memset(&serv_addr, 0, sizeof(serv_addr));
                        serv_addr.sin_family = AF_INET;
                        serv_addr.sin_port = htons(7777);
                        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
                            cout << "Invalid address/ Address not supported" << endl;
                            return -1;
                        }
                        if (connect(client_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
                            cout << "Connection Failed" << endl;
                            return -1;
                        }
                        cout << "< Entering the chat room. >" << endl;
                        my_nick = userManager.getName();
                        send(client_sock, my_nick.c_str(), my_nick.length(), 0);
                        std::thread th2(chat_recv);
                        while (1) {
                            string text;
                            std::getline(cin, text);
                            const char* buffer = text.c_str();
                            send(client_sock, buffer, strlen(buffer), 0);
                            if (text == "/exit") {
                                close(client_sock);
                                backButton = true;
                                break;
                            }
                        }
                        th2.join();
                        break;
                    }
                    case '2':
                        userManager.searchContentMessage();
                        continue;
                    case '3':
                        userManager.searchDayMessage();
                        continue;
                    case '0':
                        backButton = true;
                        break;
                    default:
                        cout << "▶Invalid input. Please try again." << endl;
                        continue;
                }
            }
        }
        else if (mainIn == 4) { // Settings
            settingMenu();
            bool backButton = false;
            while (!backButton) {
                char settingIn = 0;
                cout << "▶ ";
                cin >> settingIn;
                switch (settingIn)
                {
                    case '1':
                        userManager.modifyPw();
                        continue;
                    case '2':
                        if (userManager.deleteUser() == 1) goto initialize;
                        else backButton = true;
                        break;
                    case '0':
                        backButton = true;
                        break;
                    default:
                        cout << "▶Invalid input. Please try again." << endl;
                }
            }
        }
        else if (mainIn == 0) { // Exit
            cout << "Exiting the program." << endl;
            return 0;
        }
        else {
            cout << "▶Invalid input. Please try again." << endl;
        }
    }
    return 0;
}