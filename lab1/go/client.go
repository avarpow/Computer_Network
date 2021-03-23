package main

import (
    "os"
    "fmt"
    "net"
    "time"
)

func main() {
    conn, err := net.Dial("udp", "172.18.198.217:30011")
    defer conn.Close()
    if err != nil {
        os.Exit(1)  
    }
    for i:=0; i<100;i++ {
        t := fmt.Sprintf("%v",time.Now().UnixNano() / 1e6);
        s := fmt.Sprintf("UDP test id:%d,sendtime=%vms",i,t)
        fmt.Printf("发送第%v条消息:",i)
        fmt.Printf(s+"\n")
        conn.Write([]byte(s)) 
        time.Sleep(time.Duration(1)*time.Millisecond)
    }
   // UDP test id:99,sendtime=1517194800
    fmt.Println("发送信息完成")

}