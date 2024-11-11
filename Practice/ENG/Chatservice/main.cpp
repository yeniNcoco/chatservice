//
//  main.cpp
//  Chatservice
//
//  Created by 김예은 on 9/28/24.
//

#define MAX_SIZE 1024 //declare a const
#define MAX_CLIENT 3 //maximum number of clients: 3

struct SOCKET_INFO { //structure to define the information of a connected socket
    SOCKET sck = 0; // ctrl + click, unsigned int pointer type
    string user =""; //user  : the name of client
};

std::vector<SOCKET_INFO> sck_list; //  Declare an array to store the connected client sockets
SOCKET_INFO server_sock; // Declare a variable to store the server socket information
int client_count = 0; //  Variable to count the number of clients currently connected

void server_init() //activating server socket
{
    server_sock.sck = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN server_addr = {};
    // Variable to configure socket address
    server_addr.sin_family = AF_INET;
    // type of socket is Internet
    server_addr.sin_port = htons(7777); //:define 7777 among 123.0.0.1:7777------
    // Set the server port
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Defines the host part of 123.0.0.1:----
    // INADDR_ANY allows the use of either 127.0.0.1 or localhost. Both are allowed with INADDR_ANY.
    bind(server_sock.sck, (sockaddr*)&server_addr, sizeof(server_addr));
    // Bind the configured socket information to the socket
    listen(server_sock.sck, SOMAXCONN);
     // Set the socket to wait for incoming connections

    server_sock.user = "server";

    void add_client() {
    SOCKADDR_IN addr = {};
    int addrsize = sizeof(addr);
    char buf[MAX_SIZE] = { };

    ZeroMemory(&addr, addrsize);
    // Initialize the memory area of addr to 0

    SOCKET_INFO new_client = {};

    new_client.sck = accept(server_sock.sck, (sockaddr*)&addr, &addrsize);
    recv(new_client.sck, buf, MAX_SIZE, 0);
    // Winsock2's recv function. Receives the nickname sent by the client.
    new_client.user = string(buf);

    string msg = "▶" + new_client.user + " has joined.";
    cout << msg << endl;
    sck_list.push_back(new_client);
    // Add the new client to the sck_list array that stores client information
    print_clients();

    std::thread th(recv_msg, client_count);
    // Create a state where it continuously receives messages from other users
    th.detach();
    client_count++;

    cout << "▷Current number of connected clients: " << client_count << endl;
    send_msg(msg.c_str());
    // c_str: Converts the string type to const char*
}
void send_msg(const char* msg)
{
    for (int i = 0; i < client_count; i++)
    {
        send(sck_list[i].sck, msg, MAX_SIZE, 0);
        // Send the message to all connected clients
    }
}
void send_msg_notMe(const char* msg, int sender_idx)
{
    for (int i = 0; i < client_count; i++) {
        if (i != sender_idx) {
            send(sck_list[i].sck, msg, MAX_SIZE, 0);
        }
    }
}
void sendWhisper(int position, string sbuf, int idx)
{
    int cur_position = position + 1; // Move to the position after the initial space
    position = sbuf.find(" ", cur_position); // Find the next space (after the receiver's name)
    int len = position - cur_position; // Calculate the length of the receiver's name
    string receiver = sbuf.substr(cur_position, len); // Extract the receiver's name from the string
    cur_position = position + 1; // Move to the position after the receiver's name
    string dm = sbuf.substr(cur_position); // Extract the whisper message (direct message)
    string msg = "※Whispered [" + sck_list[idx].user + "] : " + dm; // Format the message as a whisper from the sender
    for (int i = 0; i < client_count; i++)
    {
        if (receiver.compare(sck_list[i].user) == 0) // Check if the current client's username matches the receiver
            send(sck_list[i].sck, msg.c_str(), MAX_SIZE, 0); // Send the whisper message to the matching client
    }
}

void recv_msg(int idx)
{
    char buf[MAX_SIZE] = { };
    string msg = "";

    while (1) {
        ZeroMemory(&buf, MAX_SIZE);

        if (recv(sck_list[idx].sck, buf, MAX_SIZE, 0) > 0) {
         // if no error occured then return the number of byte. if it is bigger than 0, it means message has arrived.
            string whisper(buf);
            int position = whisper.find(" ", 0);
            int message = position - 0;
            string flag = whisper.substr(0, message);
            if(string(buf) == "/end")
            {
                msg = "▶" + sck_list[idx].user + " left the room.";
                cout << msg << endl;
                send_msg(msg.c_str());
                del_client(idx);
                return;
            }
            else if (flag.compare("/whisper") == 0)
            {
                sendWhisper(position, whisper, idx);
            }
            else {

                pstmt = con->prepareStatement("INSERT INTO chatting(chatname, time, recv) VALUE(?, NOW(),  ?)");
                pstmt->setString(1, sck_list[idx].user);
                pstmt->setString(2, whisper);
                pstmt->execute();

                pstmt = con->prepareStatement("SELECT chatname, time, recv FROM chatting ORDER BY time DESC LIMIT 1");
                result = pstmt->executeQuery();
                if (result->next())
                {
                    string chatname = result->getString(1);
                    string time = result->getString(2);
                    string recv = result->getString(3);
                    msg += "--------------------------------------------------";
                    msg += "\n▷from : " + chatname + "  " + "▷ : " + time + "\n";
                    msg += "▷context : " + recv + "\n";
                    msg += "--------------------------------------------------\n";
                    cout << msg << endl;
                    send_msg_notMe(msg.c_str(), idx);
                }
            }
        }
        else { //Unless regard it as left signal and send leaving message
            msg = "[notice] " + sck_list[idx].user + " left the room.";
            cout << msg << endl;
            send_msg(msg.c_str());
            del_client(idx); // delete client
            return;
        }
    }
}
void del_client(int idx) {
    std::thread th(add_client);
    closesocket(sck_list[idx].sck);
    client_count--;
    cout << "▷Concurrent user : " << client_count << "people" << endl;
    sck_list.erase(sck_list.begin() + idx);
    th.join();
}

int main()
{
    system("color 06");
    startSql();
    mainMenu();
    WSADATA wsa;

    // function that initializes Winsock. MAKEWORD(2, 2) means that we are going to use Winsock 2.2 version.
    // 0 if it succeed, unless reuturn other value.
    // returning 0 means it is ready to use Winsock.
    
    int code = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (!code)
    {
        server_init(); //connecting sever

        std::thread th1[MAX_CLIENT];

        for(int i = 0; i < MAX_CLIENT; i++)
        {
        //Constructing thread as many people to make each client communicate at the same time
            th1[i] = std::thread(add_client);
        }

        while (1)
        { // Use infinte loop to make server send messages. Unless we can only send once.
            string text, msg = "";

            std::getline(cin, text);
            const char* buf = text.c_str();
            msg = server_sock.user + " : " + buf;
            send_msg(msg.c_str());
        }

        for (int i = 0; i < MAX_CLIENT; i++)
        {
            th1[i].join();
            //Keep main function working until thread process ends
        }

        closesocket(server_sock.sck);
    }
    else cout << "End program. (Error code : " << code << ")";

    WSACleanup();

    delete result;
    delete pstmt;
    delete con;
    delete stmt;

    return 0;
}

while (1)
                {
                    string new_pw = "";
                    cout << ">>enter the new password : ";
                    char ch = ' ';
                    while (ch != 13) { // End if user press Enter key
                        ch = _getch();
                        if (ch == 13) break; //
                        if (ch == 8) { // Backspace key
                            if (!new_pw.empty()) { // if there's input string
                                new_pw.pop_back(); // delete last letter
                                cout << "\b \b"; // output whitespace by moving the cursor to the left, return the cursor to the original position.
                            }
                        }
                        else {
                            new_pw.push_back(ch);
                            cout << '*'; // replace to * then print
                        }
                    }
                    cout << endl;

                    string renew_pw = "";
                    cout << ">>Please enter again. : ";
                    char rech = ' ';
                    while (rech != 13) { // End if user press Enter key
                        rech = _getch();
                        if (rech == 13) break; // End if user press Enter key
                        if (rech == 8) { //  if backspace key
                            if (!renew_pw.empty()) { // if there's any input string
                                renew_pw.pop_back(); // delete last letter
                                cout << "\b \b"; // output whitespace by moving the cursor to the left, return the cursor to the original position.
                            }
                            }
                        }
                        else {
                            renew_pw.push_back(rech);
                            cout << '*'; //  replace to * then print
                        }
                    }
                    cout << endl;

                    if (new_pw == renew_pw)
                    {
                        pstmt = con->prepareStatement("UPDATE user SET pw = ? WHERE id = ?");
                        pstmt->setString(1, new_pw);
                        pstmt->setString(2, id);
                        pstmt->executeQuery();
                        printf("▶Your password has been changed\n");
                        Sleep(500);
                        break;
                    }
                    else {
                        cout << "▶Password doesn't match." << endl;
                        Sleep(500);
                    }
                }
                #define MAX_SIZE 1024

SOCKET client_sock;
string my_nick;
int chat_recv() {
    char buf[MAX_SIZE] = { };
    string msg;

    while (1) {
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {
            textColor(7, 0);
            msg = buf;
            std::stringstream ss(msg);  //make string into stream
            string user;
            ss >> user;
            if (user != my_nick) cout << buf << endl;
        }
        else {
            cout << "Server Off" << endl;
            return -1;
        }
    }
}
 WSADATA wsa;
                    code = WSAStartup(MAKEWORD(2, 2), &wsa); // initialize variable
                    if (!code)
                    {
                        cout << "< Enter the chatroom. >" << endl;
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
                                cout << "※They may impersonate an acquaintance and ask for money, so if you suspect theft, please stop chatting.." << endl;
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

                            const char* buffer = text.c_str(); // transform string type into char*
                            send(client_sock, buffer, strlen(buffer), 0);
                            if (text == "/end")
                            {
                                closesocket(client_sock);
                                backButton = true;
                                break;
                            }
                        }
                        th2.join();
                    }
                    WSACleanup();
