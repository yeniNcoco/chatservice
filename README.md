# Chatting Service Project

## Objectives
- Implementing a simple chat service with C++.
- Conduct a refactoring process with an object-oriented version.
- Add mini-game that I made in C++ class.

## Purpose of the project
- I've took basic c++ programing during 1st semester.
- I've learned about socket networking in Advanced Java class.
- I wanted to make a chat service program, that can integrate knowledge I've learned. 
- Mostly, What I've done during the classes was just making a short piece code. but I'm eager to learn the actual code used in real-field applications.
- It was hard for me to start from the bottom. So I started by understand the reference code then started refactorizing the code.
- I had to study other subjects so I made enough spots for them, and built code using a spare time.


## Dues of project goals

|due|goal|
|--|--|
|10.01.2024 ~ 10.15.2024|selecting a topic of project and chose a reference|
|10.15.2024 ~ 10.31.2024|Understand every single code in reference|
|11.01.2024~ 11.07.2024| Review the overall code by writing comments line by line|
|11.08.2024 ~ 11.15.2024|Change Winsocket version -> Unix, check it runs|
|11.16.2024~ 11.30.2024| Refactoring a code into obj-oriented style|
|12.01.2024 ~ 12.15.2024| Add mini games|


## Things considered to be a high level of difficulty in the project (Things I need to overcome)
|Things to do |difficulties|
|--|--|
|understanding the reference code|Since I have never seen real working code it would be hard|
|change Winsocket version into Unix version|I need to use unfamlier library |
|refactoring into obj-oriented code|still beginer level in obj style it would be difficult for me|
|mini game|Altouogh mini games are already made it would be hard to combining with complicated code|



## chat service overview
### Server 
:Consruct a chatting sever and open TCP communicate socket when client enters, to manege a chat service. Then by linking a user information with database, add login and user identification functions.

### Client 
:A subject participating in a chat server can chats with other clients  by get connected to the chat server so a single user can have more than one connection.

## To Be 
- By referring to the UI of apps such as what’s app or snapchat, I was thinking about creating a chat app that is CLI but has a good user experience. I also don't want to miss out on the aesthetic element.
- When refactoring, I used sockets to think about the content that should be included in each function of the socket and how it will be expressed, and think about ways to make the function name or configuration easier to read.

## Features
- Logging in, finding ID, finding password, and singing up are be performed in the start screen.
- Perform My info, friends, and chatting fuctions in the main page.
- using a while loop to make it bidirectional.
- I wanted to revive the mini project I did in C++ class, so I included it as a mini game in the chat service.
