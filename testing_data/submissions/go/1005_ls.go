package main

import (
    "fmt"
    "os"
)

func main() {
    files, err := os.ReadDir(".")
    if err != nil {
        return
    }
    for _, file := range files {
        fmt.Fprintf(os.Stderr, "%s\n", file.Name())
    }
}