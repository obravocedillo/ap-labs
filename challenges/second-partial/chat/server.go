// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"flag"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel
var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
)


type user struct {
	userName string
	ip string
	userChannel chan string
	since string
	//require to kick user
	userConn net.Conn
}

//Current user
var currentAdmin user
//Array with al conected users
var connectedUsers []user
var currentUsers int

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

func kick(userName string, channel chan string){
	for index := range connectedUsers{
		if connectedUsers[index].userName == userName{
			fmt.Println("irc-server < user [" + connectedUsers[index].userName + "] was kicked out of the channel")
			connectedUsers[index].userChannel <- "irc-server < you have been kicked out of the channel"
			//Conection is closed in order to kick user
			connectedUsers[index].userConn.Close()
		}
	}
}

func listUsers(channel chan string){
	channel <- "List of users: "
	for index := range connectedUsers{
		//List all users connected
		channel <- "username: " + connectedUsers[index].userName + " Logging time: " + connectedUsers[index].since	
	}
}

func userDetail(userName string, channel chan string) user{
	var selectedUser user
	for index := range connectedUsers{
		//return selected user
		if connectedUsers[index].userName == userName{
			return connectedUsers[index]
		}
	}
	return selectedUser
}

func sendMessage(userReceiving string, userSending string, message string, userSendingChanel chan string){	
		for index := range connectedUsers{
		//return selected user
		if connectedUsers[index].userName == userReceiving{
			connectedUsers[index].userChannel <- "irc-server < message from " + userSending + " message:  " + message
			userSendingChanel <- "irc-server < message sent to " + userReceiving
		}
	}
}

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)
	//Scan items in connection, in order to get name
	input := bufio.NewScanner(conn)
	input.Scan()
	//Create user from current connection
	who := user{
		userName: input.Text(),
		userChannel: ch,
		//Get adress from conection as String
		ip: conn.RemoteAddr().String(),
		since: time.Now().String(),
		userConn: conn,
	}
	//If no users, first user will be admin
	if(currentUsers == 0){
		currentAdmin = who;
		//print admin message
		fmt.Println("irc-server > [" + who.userName + "] was promoted to channel ADMIN")
		ch <- "Congrats, you are the first user"
		ch <- "You are the new IRC Server ADMIN"
	}
	//Add number to current users
	currentUsers ++;
	//Add user to conected users
	connectedUsers = append(connectedUsers, who)
	fmt.Println("irc-server > New connected user [" + who.userName + "]")
	ch <- "irc-server > Welcome to the simple IRC Server " + who.userName

	messages <- "irc.server > New connected user [" + who.userName + "]"
	entering <- ch
	for input.Scan() {
		currentMessage := input.Text()
		//we will split the message to find if the user wants to use a command
		formatedMessage := strings.Split(currentMessage," ")
		if formatedMessage[0] == "/kick" {
			if(currentAdmin.userName == who.userName){
				ch <- "irc-server < you cant kick the ADMIN of the channel"
			}else{
				kick(formatedMessage[1], who.userChannel)
				messages <- "irc-server < [" + formatedMessage[1] + "] has been kicked from de channel" 
			}
		}else if formatedMessage[0] == "/user" {
			userInfo := userDetail(formatedMessage[1], who.userChannel)
			who.userChannel <- "irc-server > username:  " + userInfo.userName + " IP: " + userInfo.ip + " Connected since: " + userInfo.since
		}else if formatedMessage[0] == "/time" {
			zone, _ := time.Now().Zone()
			ch <- "irc.server > Local Time: " + zone + " " + time.Now().String()
		}else if formatedMessage[0] == "/users" {
			listUsers(who.userChannel)
		}else if formatedMessage[0] == "/msg" {
			formatedSingleMessage := strings.SplitN(currentMessage," ",3)
			sendMessage(formatedMessage[1],who.userName, formatedSingleMessage[2], who.userChannel)
		}else{
			messages <- who.userName + "> " + currentMessage
		}
	}
	// NOTE: ignoring potential errors from input.Err()

	leaving <- ch
	messages <- who.userName + " has left"
	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {
	currentUsers = 0;
	hostName := flag.String("host","","Host name")
	portNumber := flag.String("port","","Port number")
	flag.Parse()

	listener, err := net.Listen("tcp", *hostName + ":" + *portNumber)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("irc-server > Simple IRC Server started at " + *hostName + ":" + *portNumber)
	fmt.Println("Ready for receiving new clients")
	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main
