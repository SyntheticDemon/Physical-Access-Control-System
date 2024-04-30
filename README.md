# Cyber Physical Systems - (Realtime Embedded Systems) Report

## Code Structure and Implementational Details

### GUI Client
A GUI Client is used for the admin to get a view of the events published and entry accesses granted
### Key functions 
Certain functions are used to setup event routing between the server and the client which you can see below
![alt text](images/application_functions.png)
The below class is used for setting up a bidirectional connection between the server and the client , it emits signals which are used to handle the UI Changes in the widget application
![alt text](images/bidirectional_connection.png) 
Finally , connecting signals and slots, and using the already developed code gives us a fully functional application
![alt text](images/1.png)
![alt text](images/2.png)
### RFID Server
This server consists of two main components
1. The websocket server
2. The HTTP Server 
The business / logic layer is all maintained in the top parent class of Server[.cpp]

![alt text](images/server_header.png)

which serves as a container for the web socket server (Used mainly to communicate with)
#### Initialization
Initialization requires a user.json adjacent to RFIDServer.exe when launching, users and their RFIDs-passwords are read from this file
a ```loadJsonFromFile ``` method is used for instantiation of user data

#### Logic 
##### Authentication and Websocket functionalities 
A handle websocket request method has been implemented which is extendible, this helps to add new functionalities on the fly without handling multiple unrelated usecases in the same function 
![alt text](images/image-1.png)
Websocket server only supports one GUI client and discards the old connection upon a new request.
For authentication of users, any incorrect username password combinations will be met with a disconnection and a failed authentication response, enforcing the GUI Client to connect again
When websocket receives a new request, it emits a signal to the server class ( We avoided sharing logical resources by using signals here), server handles and uses the web socket attribute field to send the response over to the user 
![alt text](images/image-2.png)
##### RFID checks 
The server exposes a REST endpoint for checking RFIDs against the database for the arduino board access, 
this endpoint URL : 
``` /checkRFID ```
whose implementation is showed below 
![alt text](images/image-3.png)
On successful RFID verifications,a history log is sent over to the active GUI connection, this ensures all live details are sent over the connection so the GUI has the latest updates in realtime, 
Also when a new client is connected, history logs related to the past gate openings which are stored on a file in the disk, are sent serially over the network to the user 
Code from history implementation which iterates over the internal historyArray list to send arrays
![History Implementation](images/image-5.png)
![alt text](images/image-6.png)
![alt text](images/image-7.png)
![alt text](images/image-8.png)
#### User Details 
The last passing user on the gate will appear on the gate, the user details is in the form of a history log and is emitted as shown in the code below 
implemented using the ```formUserDetails()``` function

# Output
The output of the client server is shown below
## User Details
![alt text](images/image-details.png)
## History output
![alt text](images/image-history.png)
## Right Panel disabled
![alt text](images/image-panel.png)
## Postman results
### Not Found 
![alt text](images/image-found.png)
### Found
![alt text](images/image-notfound.png)
