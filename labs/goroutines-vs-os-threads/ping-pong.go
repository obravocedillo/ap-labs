package main

import (
	"fmt"
	"time"
)

var commsPerSecond = 0

func ping(receiver chan int, sender chan int){
	for{
		//sends message to pong channel
		receiver <- 10
		//Add one to communications
		commsPerSecond += 1
		//Whait until message arrives
		<-sender
	}
		
	
}

func pong(receiver chan int, sender chan int){
	for{
		//Pong receives message
		<-sender
		//Pong sends message back to ping
		receiver <- 10
		//Add one to communications
		commsPerSecond += 1
	}
}

func main() {
	chPing := make(chan int)
	chPong := make(chan int)
	//Start go routine 1
	go ping(chPong,chPing)
	//Start go routine 2
	go pong(chPing, chPong)
	//Sleep for one second
	time.Sleep(1 * time.Second)
	//Prints the number of communications
	fmt.Println("Communications Per Second : ", commsPerSecond)
	//Orfanes go routines die after main returns
}
