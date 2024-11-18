#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h> //Including library for socket programming on Windows
#include <WS2tcpip.h> //Provides additional utilities like address conversion
#include <string> // For using std::string
#include <sstream> // For working with string streams
#include <iostream> //For input/output operations
#include <thread> // Enables multithreading
#include <mysql/jdbc.h> // Library for connecting and interacting with MySQL databases
#include <conio.h> // For console input/output functions (e.g., getch)
#include <Windows.h> // For using Windows API functions

using std::cout;
using std::cin;
using std::endl;
using std::string;

#define MAX_SIZE 1024 // Defines the maximum buffer size, used for chat message limits

const string server = "tcp://127.0.0.1:3306"; // Address of the MySQL server. functions like localhost in this case
const string username = "root";  // Username for the database
const string password = "07wd2713"; // Password for the database

// setting pointers
sql::mysql::MySQL_Driver* driver; // Pointer to the MySQL driver object
sql::Connection* con; // Pointer to the database connection object
sql::PreparedStatement* pstmt; // Pointer for executing pre-compiled SQL statements
sql::ResultSet* result; // Pointer to store the results of an executed SQL query
sql::Statement* stmt; // Pointer for executing general SQL statements


SOCKET client_sock; // Variable to store the client socket
string my_nick; // Variable to store the user's nickname

void startMenu()
{
    system("cls");
    cout << "\n";
    cout << " "; cout << "*************************************************\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*       *******      *       *       *  *       *\n";
    cout << " "; cout << "*          *        * *      *       * *        *\n";
    cout << " "; cout << "*          *       *****     *       **         *\n";
    cout << " "; cout << "*          *      *     *    *       * *        *\n";
    cout << " "; cout << "*          *     *       *   *****   *  *       *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               < Start page >                   *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               1. Login                        *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               2. Find ID                       *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               3. Find PW                       *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               4. Register                     *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               0. Exit                          *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*************************************************\n\n";
}
void login()
{
    system("cls");
    cout << "\n";
    cout << " "; cout << "*************************************************\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*       *******      *       *       *  *       *\n";
    cout << " "; cout << "*          *        * *      *       * *        *\n";
    cout << " "; cout << "*          *       *****     *       **         *\n";
    cout << " "; cout << "*          *      *     *    *       * *        *\n";
    cout << " "; cout << "*          *     *       *   *****   *  *       *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               < Login page >                  *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*              >> Enter Id                      *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*              >> Enter pw                      *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*            >> login if success                *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*            >> Return to start page if fail    *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*************************************************\n\n";
}
void searchId()
{
    system("cls");
    cout << "\n";
    cout << " "; cout << "*************************************************\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*       *******      *       *       *  *       *\n";
    cout << " "; cout << "*          *        * *      *       * *        *\n";
    cout << " "; cout << "*          *       *****     *       **         *\n";
    cout << " "; cout << "*          *      *     *    *       * *        *\n";
    cout << " "; cout << "*          *     *       *   *****   *  *       *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               < Forgot ID? >                  *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >> Enter your name              *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >> Enter your number            *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >> Enter birth(8-digit)         *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >> print ID if success          *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*           >> return to start page if fail     *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*************************************************\n\n";
}
void searchPw()
{
    system("cls");
    cout << "\n";
    cout << " "; cout << "*************************************************\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*       *******      *       *       *  *       *\n";
    cout << " "; cout << "*          *        * *      *       * *        *\n";
    cout << " "; cout << "*          *       *****     *       **         *\n";
    cout << " "; cout << "*          *      *     *    *       * *        *\n";
    cout << " "; cout << "*          *     *       *   *****   *  *       *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               < Forgot PW >                   *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >> Enter ID                     *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >> Enter name                   *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >> Enter the number             *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >> Enter birth(8-digit)         *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*          >> Return to start page if success   *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*************************************************\n\n";
}
void createUser()
{
    system("cls");
    cout << "\n";
    cout << " "; cout << "*************************************************\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*       *******      *       *       *  *       *\n";
    cout << " "; cout << "*          *        * *      *       * *        *\n";
    cout << " "; cout << "*          *       *****     *       **         *\n";
    cout << " "; cout << "*          *      *     *    *       * *        *\n";
    cout << " "; cout << "*          *     *       *   *****   *  *       *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               < Register  >                   *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >> Enter ID                     *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >> Enter PW                     *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >> Enter name and number        *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >>  Enter birth(8-digit)        *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >> Return to main page          *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*************************************************\n\n";
}
void mainMenu()
{
    Sleep(500);
    system("color 06");
    system("cls");
    cout << "\n";
    cout << " "; cout << "*************************************************\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*       *******      *       *       *  *       *\n";
    cout << " "; cout << "*          *        * *      *       * *        *\n";
    cout << " "; cout << "*          *       *****     *       **         *\n";
    cout << " "; cout << "*          *      *     *    *       * *        *\n";
    cout << " "; cout << "*          *     *       *   *****   *  *       *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*              < Current Status : Connected >   *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                1. My Info                     *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                2. Friends                     *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                3. Chat Rooms                  *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                4. Settings                    *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                0. Exit                        *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*************************************************\n\n";
}
void myMenu()
{
    system("cls");
    cout << "\n";
    cout << " "; cout << "*************************************************\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*       *******      *       *       *  *       *\n";
    cout << " "; cout << "*          *        * *      *       * *        *\n";
    cout << " "; cout << "*          *       *****     *       **         *\n";
    cout << " "; cout << "*          *      *     *    *       * *        *\n";
    cout << " "; cout << "*          *     *       *   *****   *  *       *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               < View My Info >                *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*              1. My Profile                    *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*              2. Set/Modify Status Message     *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*              3. Set/Modify BGM                *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*              0. Go Back                       *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*************************************************\n\n";
}
void friends()
{
    system("cls");
    cout << "\n";
    cout << " "; cout << "*************************************************\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*       *******      *       *       *  *       *\n";
    cout << " "; cout << "*          *        * *      *       * *        *\n";
    cout << " "; cout << "*          *       *****     *       **         *\n";
    cout << " "; cout << "*          *      *     *    *       * *        *\n";
    cout << " "; cout << "*          *     *       *   *****   *  *       *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               < Friend Information >          *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               1. View Friend List             *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               2. Search Friend's Birthday     *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >> Search by Month/Day Range    *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               0.  Go Back                     *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*************************************************\n\n";
}
void chatting()
{
    system("cls");
    cout << "\n";
    cout << " "; cout << "*************************************************\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*       *******      *       *       *  *       *\n";
    cout << " "; cout << "*          *        * *      *       * *        *\n";
    cout << " "; cout << "*          *       *****     *       **         *\n";
    cout << " "; cout << "*          *      *     *    *       * *        *\n";
    cout << " "; cout << "*          *     *       *   *****   *  *       *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               < Chat Entry/Search >           *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               1. Enter Chat Room              *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >> /whisper Name Message        *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               >> /Type /exit to Leave         *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               2. Search Chat Content          *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               3. Search by Chat Period        *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               0. Go Back                      *\n";
    cout << " "; cout << "*************************************************\n\n";
}
void setting()
{
    system("cls");
    cout << "\n";
    cout << " "; cout << "*************************************************\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*       *******      *       *       *  *       *\n";
    cout << " "; cout << "*          *        * *      *       * *        *\n";
    cout << " "; cout << "*          *       *****     *       **         *\n";
    cout << " "; cout << "*          *      *     *    *       * *        *\n";
    cout << " "; cout << "*          *     *       *   *****   *  *       *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               < Settings >                    *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               1. Change Password              *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               2. Delete Account               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*               0. Go Back                      *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*                                               *\n";
    cout << " "; cout << "*************************************************\n\n";
}
void textColor(int foreground, int background)
{
    int color = foreground + background * 16; // Can combine foreground and background colors using the formula
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); // Set the console text color using the Windows API
}

int chat_recv() {
    char buf[MAX_SIZE] = { }; // Buffer to store incoming messages, initialized to zero
    string msg; // String to temporarily store the received message

    while (1) { // Infinite loop to keep listening for incoming messages
        ZeroMemory(&buf, MAX_SIZE); // Clear the buffer before each receive operation
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) { // Receive data from the client socket
            textColor(7, 0); // Set the text color to white foreground and black background
            msg = buf; // Convert the received char array to a string
            std::stringstream ss(msg); // Turn the message string into a stream for parsing
            string user;
            ss >> user; // Extract the first word (assumed to be the username)
            if (user != my_nick) // Check if the message is from someone else
                cout << buf << endl; // Display the message if it's not sent by the current user
        }
        else { // If recv fails or the server disconnects
            cout << "Server Off" << endl; // Notify the user that the server is down
            return -1; // Exit the function with an error code
        }
    }
}



class SQL
{
private:
    string id, pw, name, phone, status, birth, song;
public:
    SQL()
    {
        try {
            driver = sql::mysql::get_mysql_driver_instance();
            con = driver->connect(server, username, password);
        }
        catch (sql::SQLException& e) {
            cout << "Could not connect to server. Error message: " << e.what() << endl;
            exit(1);
        }

        con->setSchema("project1");

        // DB to save Korean
        stmt = con->createStatement();
        stmt->execute("set names euckr");
        if (stmt) { delete stmt; stmt = nullptr; }
    }
    int login()
    {
        cout << ">>ID : ";
        cin >> id;
        this->id = id;
        cout << ">>Please enter PW : ";
        char ch = ' ';
        while (ch != 13) { //Stop input when Enter key is pressed
            ch = _getch();
            if (ch == 13) break; //Stop input when Enter key is pressed
            else if (ch == 8) { // If Backspace is pressed
                if (!pw.empty()) { // If there are characters entered
                    pw.pop_back(); //  Delete the last character
                    cout << "\b \b"; // Move the cursor one position to the left, print a space to erase the character,
                                     // and then move the cursor back to its original position
                }
            }
            else {
                pw.push_back(ch);
                cout << '*'; // Replace character by an asterisk (*)
            }
        }
        cout << endl;
        pstmt = con->prepareStatement("SELECT id, pw, name FROM user \
            WHERE id=?");
        pstmt->setString(1, id);
        result = pstmt->executeQuery();

        if (result->next())
        { // If there is result of Query
            string db_id = result->getString(1); // ID brought form DB
            string db_pw = result->getString(2); // PW brought form DB

            if (db_id == id && db_pw == pw)
            {
                name = result->getString(3);
                this->name = name;
                cout << endl << "▶Logging in. Please wait." << endl << endl;

                cout << "▶" << "Welcome " << name << endl;
                return 1;
            }
            else cout << "▶No matching information found." << endl;
        }
        else cout << "▶No matching information found." << endl;
        pw.clear();
    }
   void searchId()
{
    cout << ">>Name : ";
    cin >> name; // Input user's name
    cout << ">>Phone number : ";
    cin >> phone; // Input user's phone number
    while (true)
    {
        cout << ">>Birth : ";
        cin >> birth; // Input user's birth date in YYYYMMDD format
        if (birth.length() != 8) { // Check if the input is 8 digits
            cout << "▶Please enter in 8-digit." << endl; // Prompt for correct input
            continue;
        }
        break; // Exit the loop if input is valid
    }

    // Split birthdate into year, month, and day for formatting
    string year = birth.substr(0, 4);
    string month = birth.substr(4, 2);
    string day = birth.substr(6, 2);
    string DATE = year + "-" + month + "-" + day; // Converting to YYYY-MM-DD format

    // Query to find ID based on phone number
    pstmt = con->prepareStatement("SELECT id, name, phone, birth FROM user WHERE phone=?");
    pstmt->setString(1, phone);
    result = pstmt->executeQuery();

    if (result->next())
    {
        string db_id = result->getString(1); // Retrieved ID from the database
        string db_name = result->getString(2); // Retrieved name
        string db_phone = result->getString(3); // Retrieved phone number
        string db_birth = result->getString(4); // Retrieved birth date in DB format

        // Verify all user details match
        if (db_name == name && db_phone == phone && db_birth == DATE)
        {
            cout << "▶" << "Your ID is " << db_id << endl; // Display retrieved ID
            Sleep(3000); // Pause for 3 seconds
        }
        else
        {
            cout << "▶No matching information found!" << endl; // If any detail doesn't match
            Sleep(500);
        }
    }
    else {
        cout << "▶No matching information found." << endl; // No user found in the database
        Sleep(500);
    }
}

void searchPw()
{
    cout << ">>ID : ";
    cin >> id; // Input user's ID
    cout << ">>Name : ";
    cin >> name; // Input user's name
    cout << ">>Phone number : ";
    cin >> phone; // Input user's phone number
    while (true)
    {
        cout << ">>Birth : ";
        cin >> birth; // Input user's birth date in YYYYMMDD format
        if (birth.length() != 8) { // Check for correct format
            cout << "▶Please enter in 8-digit." << endl;
            continue;
        }
        break; // Exit the loop if input is valid
    }

    // Format birthdate
    string year = birth.substr(0, 4);
    string month = birth.substr(4, 2);
    string day = birth.substr(6, 2);
    string DATE = year + "-" + month + "-" + day; // Convert to YYYY-MM-DD format

    // Query to find user information based on ID
    pstmt = con->prepareStatement("SELECT id, name, phone, birth FROM user WHERE id=?");
    pstmt->setString(1, id);
    result = pstmt->executeQuery();

    if (result->next())
    {
        string db_id = result->getString(1); // Retrieved ID from the database
        string db_name = result->getString(2); // Retrieved name
        string db_phone = result->getString(3); // Retrieved phone number
        string db_birth = result->getString(4); // Retrieved birth date in DB format

        // Verify all user details match
        if (db_id == id && db_name == name && db_phone == phone && db_birth == DATE)
        {
            while (1)
            {
                string new_pw = "";
                cout << ">>Enter a new password : ";
                char ch = ' ';
                while (ch != 13) { // Accept input until Enter is pressed
                    ch = _getch();
                    if (ch == 13) break; // Stop input on Enter
                    if (ch == 8) { // Handle backspace
                        if (!new_pw.empty()) { // If there are characters
                            new_pw.pop_back(); // Remove the last character
                            cout << "\b \b"; // Erase the character visually
                        }
                    }
                    else {
                        new_pw.push_back(ch); // Add character to the new password
                        cout << '*'; // Display an asterisk (*)
                    }
                }
                cout << endl;

                string renew_pw = "";
                cout << ">>Please enter again. : ";
                char rech = ' ';
                while (rech != 13) { // Confirm the new password
                    rech = _getch();
                    if (rech == 13) break;
                    if (rech == 8) { // Handle backspace
                        if (!renew_pw.empty()) {
                            renew_pw.pop_back();
                            cout << "\b \b";
                        }
                    }
                    else {
                        renew_pw.push_back(rech);
                        cout << '*';
                    }
                }
                cout << endl;

                if (new_pw == renew_pw) // Verify if both inputs match
                {
                    // Update the password in the database
                    pstmt = con->prepareStatement("UPDATE user SET pw = ? WHERE id = ?");
                    pstmt->setString(1, new_pw);
                    pstmt->setString(2, id);
                    pstmt->executeQuery();
                    printf("▶Your password has been changed.\n");
                    Sleep(500); // Pause for a moment
                    break; // Exit the loop after successful change
                }
                else {
                    cout << "▶The passwords do not match." << endl; // Notify mismatch
                    Sleep(500);
                }
            }
        }
        else {
            cout << "▶No matching information found." << endl; // User details don't match
            Sleep(500);
        }
    }
    else {
        cout << "▶No matching information found." << endl; // No user found in the database
        Sleep(500);
    }
}

    void crateUser()
    {
        while (1)
        {
            cout << ">>ID : ";
            cin >> id;
            pstmt = con->prepareStatement("SELECT id FROM user WHERE id=?");
            pstmt->setString(1, id);
            result = pstmt->executeQuery();

            if (result->next())
            {
                string db_id = result->getString(1);
                if (db_id == id) {
                    cout << "▶This username is already taken." << endl;
                    continue;
                }
            }
            else {
                cout << "▶complete." << endl;
                break;
            }
        }
        while (1)
        {
            cout << ">>Enter PW : ";
            char ch = ' ';
            while (ch != 13) { // Stop input when Enter key is pressed
                ch = _getch();
                if (ch == 13) break; //Stop input when Enter key is pressed
                if (ch == 8) { // If Backspace is pressed
                    if (!pw.empty()) { // If there are characters entered
                        pw.pop_back(); // Delete the last character
                        cout << "\b \b"; // Move the cursor one position to the left, print a space to erase the character, and then move the cursor back to its original position
                    }
                }
                else {
                    pw.push_back(ch);
                    cout << '*';
                }
            }
            cout << endl;
            string new_pw = "";
            cout << endl << ">>Please enter again. : ";
            char rech = ' ';
            while (rech != 13) {
                rech = _getch();
                if (rech == 13) break;
                else if (rech == 8) {
                    if (!new_pw.empty()) {
                        new_pw.pop_back();
                        cout << "\b \b";
                    }
                }
                else {
                    new_pw.push_back(rech);
                    cout << '*';
                }

            }
            cout << endl;

            if (pw == new_pw)
            {
                break;
            }
            else
            {
                cout << "▶Wrong pw." << endl;
                pw.clear();
            }
        }

        cout << ">>Name : ";
        cin >> name;
        cout << ">>Number : ";
        cin >> phone;
        while (true)
        {
            cout << ">>Birth : ";
            cin >> birth;
            if (birth.length() != 8) {
                cout << "▶Please enter your date of birth in 8 digits." << endl;
                continue;
            }
            break;
        }

        string year = birth.substr(0, 4);
        string month = birth.substr(4, 2);
        string day = birth.substr(6, 2);
        string DATE = year + "-" + month + "-" + day;

        pstmt = con->prepareStatement("INSERT INTO user(id,pw, name, phone, birth) VALUE(?,?, ?, ?, ?)");
        pstmt->setString(1, id);
        pstmt->setString(2, pw);
        pstmt->setString(3, name);
        pstmt->setString(4, phone);
        pstmt->setString(5, DATE);
        pstmt->execute();
        cout << "▶Your registration has been completed.Welcome" << endl;
        pw.clear();
        Sleep(500);
    }
    void myProfile()
{
    pstmt = con->prepareStatement("SELECT name, status, song, birth, phone FROM user WHERE id = ?;");
    pstmt->setString(1, id); // Bind the user ID to the SQL query
    result = pstmt->executeQuery(); // Execute the query and retrieve user profile details

    while (result->next())
    {
        // Displaying user profile details
        cout << "-----------------------------------------" << endl;
        cout << "▷Name : " << result->getString("name") << endl;

        // Display current vibe; check if it's empty before printing
        if (result->getString("status") == "")
        {
            cout << "▷Current vibe : " << endl;
        }
        else
        {
            cout << "▷Current vibe : " << result->getString("status") << endl;
        }

        // Display song; check if it's empty before printing
        if (result->getString("song") == "")
        {
            cout << "▷Song : " << endl;
        }
        else
        {
            cout << "▷Song : " << result->getString("song") << endl;
        }

        // Display other details
        cout << "▷Birth : " << result->getString("birth") << endl;
        cout << "▷Number : " << result->getString("phone") << endl;
        cout << "-----------------------------------------" << endl;
    }
}

void updateStatus()
{
    cout << ">>Current vibe : ";
    cin.ignore(); // Ignore any leftover newline character in the input buffer
    getline(cin, status); // Take a full line of input for the status update

    // Prepare and execute the update query for status
    pstmt = con->prepareStatement("UPDATE user SET status = ? WHERE id = ?");
    pstmt->setString(1, status); // Bind the new status to the query
    pstmt->setString(2, id); // Bind the user ID to the query
    pstmt->executeQuery(); // Execute the query

    printf("▶Successfully updated.\n"); // Notify the user of success
}

void updateSong()
{
    cout << ">>Share the song : ";
    cin.ignore(); // Ignore any leftover newline character in the input buffer
    getline(cin, song); // Take a full line of input for the song update

    // Prepare and execute the update query for song
    pstmt = con->prepareStatement("UPDATE user SET song = ? WHERE id = ?");
    pstmt->setString(1, song); // Bind the new song to the query
    pstmt->setString(2, id); // Bind the user ID to the query
    pstmt->executeQuery(); // Execute the query

    printf("▶Successfully updated.\n"); // Notify the user of success
}
void friends()
{
    // Prepare SQL query to fetch all users except the current user
    pstmt = con->prepareStatement("SELECT name, status, song, birth, phone FROM user WHERE id != ?;");
    pstmt->setString(1, id); // Set the parameter to exclude the current user
    result = pstmt->executeQuery(); // Execute the query

    while (result->next())
    {
        // Display friend details
        cout << "-----------------------------" << endl;
        cout << "▷Name : " << result->getString("name") << endl;
        cout << "▷Current Vibe : "
             << (result->getString("status").empty() ? "No status available." : result->getString("status")) << endl;
        cout << "▷Song : "
             << (result->getString("song").empty() ? "No song shared." : result->getString("song")) << endl;
        cout << "▷Birth : " << result->getString("birth") << endl;
        cout << "▷Number : " << result->getString("phone") << endl;
    }
}

void searchBirth()
{
    string startDay, endDay;
    cout << "Please enter the first 4 digits (e.g., 0201): ";
    cin >> startDay;
    cout << "Please enter the last 4 digits (e.g., 0405): ";
    cin >> endDay;

    // Prepare SQL query to search users with birthdays between the specified range
    pstmt = con->prepareStatement("SELECT name, birth, phone FROM user \
                                   WHERE DATE_FORMAT(birth, '%m%d') BETWEEN ? AND ? \
                                   AND id != ?;");
    pstmt->setString(1, startDay); // Set the starting day
    pstmt->setString(2, endDay);   // Set the ending day
    pstmt->setString(3, id);       // Exclude the current user
    result = pstmt->executeQuery(); // Execute the query

    if (!result->next())
    {
        cout << "No search results found." << endl;
    }
    else
    {
        do
        {
            // Display search results for users' birthdays
            cout << "-----------------------------------------------" << endl;
            cout << "▷Name : " << result->getString("name") << endl;
            cout << "▷Birth : " << result->getString("birth") << endl;
            cout << "▷Number : " << result->getString("phone") << endl;
        } while (result->next());
    }
}

void search_content_Message()
{
    string content;
    cout << ">>Search by contents: ";
    cin >> content;

    // Prepare SQL query to search chat messages containing specific content
    pstmt = con->prepareStatement("SELECT chatname, time, recv FROM chatting \
                                   WHERE recv LIKE ?");
    pstmt->setString(1, "%" + content + "%"); // Use wildcards to perform a partial match
    result = pstmt->executeQuery(); // Execute the query

    if (!result->next())
    {
        cout << "No search results found." << endl;
    }
    else
    {
        do
        {
            // Display chat messages matching the content
            cout << "--------------------------------------------------" << endl;
            cout << "▷From : " << result->getString("chatname")
                 << " ▷Time : " << result->getString("time") << endl;
            cout << "▷" << result->getString("recv") << endl;
        } while (result->next());
    }
}

void search_day_Message()
{
    string startDay, endDay;
    cout << "Please enter the first 4 digits (e.g., 0201): ";
    cin >> startDay;
    cout << "Please enter the last 4 digits (e.g., 0405): ";
    cin >> endDay;

    string startMonthDay = "2023" + startDay; // Append year to start day
    string endMonthDay = "2023" + endDay;    // Append year to end day

    // Prepare SQL query to search chat messages sent within the specified date range
    pstmt = con->prepareStatement("SELECT chatname, time, recv FROM chatting \
                                   WHERE time BETWEEN ? AND ?");
    pstmt->setString(1, startMonthDay); // Set the start date
    pstmt->setString(2, endMonthDay);   // Set the end date
    result = pstmt->executeQuery(); // Execute the query

    if (!result->next())
    {
        cout << "No search results found." << endl;
    }
    else
    {
        do
        {
            // Display chat messages within the specified date range
            cout << "--------------------------------------------------" << endl;
            cout << "▷From : " << result->getString("chatname")
                 << " ▷Time : " << result->getString("time") << endl;
            cout << "▷" << result->getString("recv") << endl;
        } while (result->next());
    }
}
    void modifyPw()
    {
        cout << ">>Please enter your current password. : ";
        string existPw = "";
        char ch = ' ';
        while (ch != 13)
        {
            ch = _getch();
            if (ch == 13) break;
            if (ch == 8) {
                if (!existPw.empty()) { // if there's input string
                    existPw.pop_back(); // delete last letter
                    cout << "\b \b"; // output whitespace by moving the cursor to the left, return the cursor to the original position.                }
            }
            else {
                existPw.push_back(ch);
                cout << '*'; // replace to * then print
            }
        }
        cout << endl;

        pstmt = con->prepareStatement("SELECT pw FROM user WHERE id=?");
        pstmt->setString(1, id);
        result = pstmt->executeQuery();
        if (result->next())
        {
            while (1)
            {
                string new_pw = "";
                cout << ">>Please enter a new pw : ";
                char ch = ' ';
                while (ch != 13) { // End if user press Enter key
                    ch = _getch();
                    if (ch == 13) break;  // End if user press Enter key
                    else if (ch == 8) { //  if backspace key
                        if (!new_pw.empty()) {  // delete last letter
                            new_pw.pop_back();  // output whitespace by moving the cursor to the left, return the cursor to the original position.
                            cout << "\b \b";
                        }
                    }
                    else {
                        new_pw.push_back(ch);
                        cout << '*';
                    }
                }
                cout << endl;

                string renew_pw = "";
                cout << endl << ">>Please enter again. : ";
                char rech = ' ';
                while (rech != 13) {
                    rech = _getch();
                    if (rech == 13) break;
                    else if (rech == 8) {
                        if (!renew_pw.empty()) {
                            renew_pw.pop_back();
                            cout << "\b \b";
                        }
                    }
                    else {
                        renew_pw.push_back(rech);
                        cout << '*';
                    }

                }
                cout << endl;

                if (new_pw == renew_pw)
                {
                    pstmt = con->prepareStatement("UPDATE user SET pw = ? WHERE id = ?");
                    pstmt->setString(1, new_pw);
                    pstmt->setString(2, id);
                    pstmt->executeQuery();
                    printf("▶Your pw has been changed successfully.\n");
                    break;
                }
                else cout << "▶Wrong pw." << endl;
            }

        }
        else  cout << "▶Wrong pw." << endl;

    }
    int deleteUser()
    {
        cout << ">>Please enter your password. : ";
        string existPw = "";
        char ch = ' ';
        while (ch != 13) {
            ch = _getch();
            if (ch == 13) break;
            if (ch == 8) {
                if (!existPw.empty()) {
                    existPw.pop_back();
                    cout << "\b \b";
                }
            }
            else {
                existPw.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;

        pstmt = con->prepareStatement("SELECT pw FROM user \
            WHERE id=?");
        pstmt->setString(1, id);
        result = pstmt->executeQuery();

        if (result->next())
        { // If there are any query results
            string db_pw = result->getString(1); //  PW brought from DB
            if (db_pw == existPw)
            {
                cout << "Are you sure you want to delete? Once deleted, it cannot be undone. 1. Continue  2. Quit  : ";
                char lastCheck;
                cin >> lastCheck;
                if (lastCheck == '1')
                {
                    pstmt = con->prepareStatement("DELETE FROM user WHERE id = ?");
                    pstmt->setString(1, id);
                    result = pstmt->executeQuery();
                    cout << "▶Thank you for your support. Please visit us again!" << endl;
                    return 1;
                }
                else if (lastCheck == '2')
                {
                    cout << "Wise choice" << endl;
                    return 2;
                }
                else cout << "▶Invalid input." << endl;

            }
            else cout << "▶Wrong pw." << endl;
        }
        else cout << "▶Wrong pw." << endl;
    }
    
    //Retrieves the name of a user by their id from the database.
    string getName()
    {
        string getName = "";
        pstmt = con->prepareStatement("SELECT name FROM user \
            WHERE id=?");
        pstmt->setString(1, id);
        result = pstmt->executeQuery();
        if (result->next())
        {
            getName = result->getString(1);
        }
        return getName;
    }
    void beforeChat()
    {
        pstmt = con->prepareStatement("SELECT chatname, time, recv FROM chatting ORDER BY time DESC LIMIT 5");
        result = pstmt->executeQuery();

        string chatname[5];
        string time[5];
        string recv[5];
        string msg[5];
        int num = 0;
        while (result->next()) // Fetch messages from the result set
        {
            chatname[num] = result->getString(1);
            time[num] = result->getString(2);
            recv[num] = result->getString(3);
            num++;
        }
        // Print messages in reverse order (most recent first)
        for (int i = 4; i >= 0; i--) {  // Limit the number of messages to 5
            msg[i] += "--------------------------------------------------";
            msg[i] += "\n▷From : " + chatname[i] + " " + "▷time : " + time[i] + "\n";
            msg[i] += "▷Content : " + recv[i] + "\n";
            msg[i] += "--------------------------------------------------\n";
            cout << msg[i] << endl;
        }
    }
};

int main()
{
    SQL sql;
    bool loginSuccess = false;

    //Implementing start page
    while (!loginSuccess)
    {
        startMenu(); //start page
        char startIn = 0;
        cout << "▶ ";
        cin >> startIn;
        switch (startIn)
        {
        case '1': //1. Log in
            login();
            if (sql.login() == 1) {
                loginSuccess = true;
                break;
            }
            continue;
        case '2': //2. Find id
            searchId();
            sql.searchId();
            continue;

        case '3': //3. Find pw
            searchPw();
            sql.searchPw();
            continue;

        case '4': //4. Register
            createUser();
            sql.crateUser();
            continue;
        case '0': //0. Shut down
            return -1;
        default: //5 ~ 9. Re-enter
            cout << "▶Invalid input. Please try again." << endl;
            continue;
        }

    }

    //Implementing main page
    while (1)
    {
        mainMenu(); //main page
        int mainIn = 0;
        cout << "▶ ";
        cin >> mainIn;

        //Implementing my info
        if (mainIn == 1)
        {
            myMenu();
            bool backButton = false;
            while (!backButton)
            {
                char informationIn = 0;
                cout << "▶ ";
                cin >> informationIn;
                switch (informationIn)
                {
                case '1':
                    sql.myProfile();
                    break;
                case '2':
                    sql.updateStatus(); //setting status message
                    continue;
                case '3':
                    sql.updateSong(); //setting the song
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

        //Implementing friends
        else if (mainIn == 2)
        {
            friends(); //Friends page
            bool backButton = false;
            while (!backButton)
            {
                char friendsIn = 0;
                cout << "▶ ";
                cin >> friendsIn;
                switch (friendsIn)
                {
                case '1':
                    sql.friends(); //Friends list
                    continue;
                case '2':
                    sql.searchBirth(); //
                    continue;
                case '0':
                    backButton = true;
                    break;
                default:
                    cout << "▶Invalid input. Please try again. << endl;
                    continue;
                }
            }
        }

        //Implementing chat room page
        else if (mainIn == 3)
        {
            chatting(); //friend page
            bool backButton = false;
            int code = 0; // initializing
            while (!backButton)
            {
                char chattingIn = 0;
                cout << "▶Invalid input. Please try again. ";
                cin >> chattingIn;
                switch (chattingIn)
                {
                case '1':
                    sql.beforeChat();
                    WSADATA wsa;
                    code = WSAStartup(MAKEWORD(2, 2), &wsa); // initializing the var
                    if (!code)
                    {
                        cout << "< Entering the chat room. >" << endl;
                        my_nick = sql.getName();
                        closesocket(client_sock);
                        client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

                        SOCKADDR_IN client_addr = {};
                        client_addr.sin_family = AF_INET;
                        client_addr.sin_port = htons(7777);
                        InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);

                        while (1)
                        {
                            if (!connect(client_sock, (SOCKADDR*)&client_addr, sizeof(client_addr))) {
                                cout << "※If impersonation is suspected, please stop the conversation immediately, as someone might be posing as an acquaintance to request money." << endl;
                                send(client_sock, my_nick.c_str(), my_nick.length(), 0);
                                break;
                            }
                            cout << "Connecting..." << endl;
                        }
                        std::thread th2(chat_recv);
                        while (1)
                        {
                            string text;
                            std::getline(cin, text);

                            const char* buffer = text.c_str(); // Type casting from strin to char*
                            send(client_sock, buffer, strlen(buffer), 0);
                            if (text == "/Exit")
                            {
                                closesocket(client_sock);
                                backButton = true;
                                break;
                            }
                        }
                        th2.join();
                    }
                    WSACleanup();
                    break;
                case '2':
                    sql.search_content_Message();
                    continue;
                case '3':
                    sql.search_day_Message();
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

        // Implementing the settings page
else if (mainIn == 4)
{
    setting(); // Call the function to display the settings menu
    bool backButton = false; // Flag to determine when to return to the previous menu
    while (!backButton)
    {
        char settingIn = 0;
        cout << "▶ "; // Prompt the user to make a choice
        cin >> settingIn;
        switch (settingIn)
        {
        case '1':
            sql.modifyPw(); // Call the method to change the password
            continue; // Continue to show the settings menu after modifying the password

        case '2':
            if (sql.deleteUser() == 1) // Call the method to delete the user; check if successful
                return -1; // Exit the program if the user is successfully deleted
            else
                backButton = true; // Otherwise, return to the previous menu
            break;

        case '0':
            backButton = true; // Return to the previous menu
            break;

        default:
            cout << "▶Invalid input. Please try again." << endl; // Handle invalid input
        }
    }
}

// Exit button
else if (mainIn == 0)
{
    cout << "Quitting the program." << endl; // Inform the user that the program is exiting
    return 0; // Exit the program
}
else
{
    cout << "Invalid input. Please try again." << endl; // Handle invalid input for the main menu
}

// Clean up and release resources before exiting the program
delete result;
delete pstmt;
delete con;
delete stmt;
return 0;

