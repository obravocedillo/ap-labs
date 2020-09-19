package main

import "golang.org/x/tour/pic"

func Pic(dx, dy int) [][]uint8 {
	slice := make([][]uint8, dy)
	for i := 0; i < dy; i++ {
		slice[i] = make([]uint8, dx);
	}

	for j := 0; j < dy; j++{
		//position i in slice (aray of uint8)
		for k := 0; k < len(slice[j]); k++{
			//j ^ k proposed by golang tour page
			slice[j][k] = uint8(j) ^ uint8(k)
		}
	}

    return slice
}

func main() {
	pic.Show(Pic)
}
