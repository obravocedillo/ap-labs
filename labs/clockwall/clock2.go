// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"io"
	"log"
	"net"
	"time"
	"os"
	"fmt"
)

func handleConn(c net.Conn, timeZone string) {
	defer c.Close()
	for {
		//get time from location
		_, errTime := time.LoadLocation(timeZone)
		if errTime != nil {
			log.Fatal(errTime)
			break
		}
		_, err := io.WriteString(c, time.Now().Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	//more arguments than needed
	if len(os.Args[1:]) < 2{
		log.Fatal("Less arguments than needed")
	}
	//time zone the user selected
	tz := os.Getenv("TZ")
	port := "localhost:"+os.Args[2]
	fmt.Println("Port used: " + port)
	listener, err := net.Listen("tcp", port)
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn, tz) // handle connections concurrently
	}
}
