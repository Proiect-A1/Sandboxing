package main

import "fmt"

func main() {
    var x int
    if _, err := fmt.Scan(&x); err == nil {
        size := x * 10 * 1024 * 1024
        if size < 100 {
            size = 100
        }
        v := make([]byte, size)
        for i := 0; i < size; i += 100 {
            v[i] = byte(i % 256)
        }
        fmt.Println(v[size/2])
    }
}