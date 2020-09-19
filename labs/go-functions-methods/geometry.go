// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import 	(
			"math"
			"math/rand"
			"fmt"
			"os"
			"strconv"
		)

type Point struct{ x, y float64 }


func onSegment(p Point, q Point, r Point) bool{ 
    if (q.x <= math.Max(p.x, r.x) && q.x >= math.Min(p.x, r.x) && q.y <= math.Max(p.y, r.y) && q.y >= math.Min(p.y, r.y)){
		return true
	}else{
		return false
	}
} 

func orientation(p Point, q Point, r Point) int{ 
    val := (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y)
  
    if (val == 0){
		return 0
	}else if (val > 0){
		return 1
	}else {
		return 2
	}
} 

//fix for intersection
func doIntersect( p1 Point, q1 Point, p2 Point, q2 Point) bool{ 
    o1 := orientation(p1, q1, p2) 
    o2 := orientation(p1, q1, q2) 
    o3 := orientation(p2, q2, p1) 
    o4 := orientation(p2, q2, q1)
  
    if (o1 != o2 && o3 != o4){
		return true
	}
       
  
    if (o1 == 0 && onSegment(p1, p2, q1)){
		return true
	} 
  
    if (o2 == 0 && onSegment(p1, q2, q1)){
		return true
	} 
  
    if (o3 == 0 && onSegment(p2, p1, q2)){
		return true
	} 
  
    if (o4 == 0 && onSegment(p2, q1, q2)){
		return true
	} 
  
    return false
} 

//generate random points
func GeneratePoints(number int) []Point{
	points := make([]Point, number)
	for i := 0; i<number; i+=2{
		//generates a random from -100 to 100 for x and y
		tempX := -100 + rand.Float64() * (200) 
		tempY := -100 + rand.Float64() * (200)
		tempX2 := -100 + rand.Float64() * (200) 
		tempY2 := -100 + rand.Float64() * (200)
		if(i>=2){
			for{
				tempIntersect := false
				for j := 0; j<i; j+=2{
					if(doIntersect(Point{tempX,tempY}, Point{tempX2,tempY2}, Point{points[j].x,points[j].y}, Point{points[j+1].x,points[j+1].y}) == true){
						tempIntersect = true;
					}
				}
				if(tempIntersect == false){
					break
				}
				tempX = -100 + rand.Float64() * (200) 
				tempY = -100 + rand.Float64() * (200)
				tempX2 = -100 + rand.Float64() * (200) 
				tempY2 = -100 + rand.Float64() * (200)
			}
		}
		points[i] = Point{tempX,tempY}
		points[i+1] = Point{tempX2,tempY2}
	}
	return points
}

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
		}
	}
	return sum
}

func main() {
	arguments := os.Args[1:]
	vertices, _ := strconv.Atoi(arguments[0])
	fmt.Printf("Generating a [%v] sides figure\n",arguments[0])
	fmt.Printf("Figure vertices\n")
	points := GeneratePoints(vertices)
	perimeter := 0.0
	for i := 0; i<len(points); i++{
		fmt.Printf("( %v,  %v)\n",points[i].x,points[i].y)
	}
	fmt.Printf("Figure perimeter\n")
	for j := 0; j<len(points); j++{
		if(j+1 == len(points)){
			perimeter += Distance(points[j], points[0])
			fmt.Printf("%v ", Distance(points[j], points[0]))
		}else{
			perimeter += Distance(points[j], points[j+1])
			fmt.Printf("%v + ", Distance(points[j], points[j+1]))
		}
		
	}
	fmt.Printf("= %v\n",perimeter)
	


	fmt.Printf("\n")
}

//!-path
