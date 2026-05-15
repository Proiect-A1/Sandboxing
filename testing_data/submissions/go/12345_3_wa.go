package main

import "fmt"

func main() {
    var x int
    if _, err := fmt.Scan(&x); err == nil {
        fmt.Println(x + 1)
    }
}