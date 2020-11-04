package main

import (
	"fmt"
	"time"
	"runtime"
	"log"
)

var maxStages int
//Create first pipe global variable
var firstPipe chan int
var m runtime.MemStats
			
//Create pipelines equal to the number of n
func createPipelines(n int) (<-chan int){
	//Create channel for first pipe
	firstPipe = make(chan int)
	finalPipe := firstPipe
	for i := 0; i < n; i++ {
		runtime.ReadMemStats(&m)
		maxStages ++
		//Previous and next pipe of current i pipe
		beforePipe := finalPipe
		afterPipe := make(chan int)
		go func(){
			//Allow sending data to pipe
			for data := range beforePipe {
				afterPipe <- data
			}
		}()
		//Final pipe of the for loop
		finalPipe = afterPipe
	}
	return finalPipe
}

func main(){
	lastPipe := createPipelines(9999)
	startingTime := time.Now()
	//Send message to first pipe
	firstPipe <- 1
	//Whait until last pipe receives the message
	<-lastPipe
	endTime := time.Now()
	//Gets the difference from 2 times
	transitTime := endTime.Sub(startingTime)
	log.Printf("\nTotalAlloc = %v\nSys = %v\n\n", m.TotalAlloc/1024, m.Sys/1024)
	fmt.Println("Maximum number of pipeline stages: ", maxStages)
	fmt.Println("Total time to traverse the entire pipeline: ", transitTime)
	fmt.Println("I decreased the number of pipelines to have a slower running time, my virtual box could use up to 99999 pipes")
}
