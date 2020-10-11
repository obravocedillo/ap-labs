// ClockWall uses the same template from clock2
package main

import (
	"log"
	"net"
	"time"
	"os"
	"fmt"
	"strings"
)

func handleConnWall(c net.Conn, location string) {
	defer c.Close()
	for {
		//get time from location, 10 bytes are read from clock2, couldnt use string for type byte[]
		//It could also be read from a buffer
		locationHour := make([]byte, 10)
		_, errHour := c.Read(locationHour)
		if errHour != nil {
			log.Fatal("Error reading hour from the server")
			break
		}
		//Print the hour we got
		//Couldnt get the time zone name from a city
		fmt.Printf("%s : %s", location, locationHour)
		time.Sleep(1 * time.Second)
	}
}

func main() {
	//more arguments than needed
	if len(os.Args[1:]) < 1{
		log.Fatal("Less arguments than needed")
	}
	
	for i:=1;i<len(os.Args[1:])+1;i++{
		//Reading every clock passed in the command line and getting the information nedeed
		clockInformation := strings.Split(os.Args[i], "=")
		//debug fmt.Println(clockInformation[1])
		conn, err := net.Dial("tcp",clockInformation[1])
		if err != nil{
			log.Fatal(err)
		}

		go handleConnWall(conn, clockInformation[0])
	}

	//Equivalent to while(true) in go
	for{

	}
}
