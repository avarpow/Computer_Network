package main

import (
	"fmt"
	//"net"
	"regexp"
	//"time"
)

// UDP Server端
func main() {
    reg1 := regexp.MustCompile(`.*id:(.*?),.*sendtime=(.*?)ms`)
    if reg1 == nil {
        fmt.Println("regexp err")
        return
    }	

		var data []byte = []byte("UDP test id:99,sendtime=1616467431843ms")
		var str = string(data[:])
		fmt.Printf("data:%s\n", str)
		result1 := reg1.FindStringSubmatch(str)
		fmt.Printf("regex:%v\n", result1[:])
		fmt.Printf("data:%v\n", str)

}