package main

import(	"fmt" 
		"strings"
	  )	

func wordCount(word string) map[string]int {
	slice := make(map[string]int)
	singleWords := strings.Fields(word)
	for i := 0; i < len(singleWords); i++{
		slice[singleWords[i]] += 1
	}
	return slice
}


func main() {
	fmt.Printf("%v\n",wordCount("hola papu"))
}
