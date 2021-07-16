package main

import (
	"fmt"
	"net"
	"regexp"
	"time"
	"strconv"
)

// UDP Server端
func main() {
    reg1 := regexp.MustCompile(`.*id:(.*?),.*sendtime=(.*?)ms`)
    if reg1 == nil {
        fmt.Println("regexp err")
        return
    }	
	listen, err := net.ListenUDP("udp", &net.UDPAddr{
		IP:   net.IPv4(0, 0, 0, 0),
		Port: 30011,
	})
	if err != nil {
		fmt.Println("Listen failed, err: ", err)
		return
	}
	cont:=0
	defer listen.Close()
	fmt.Println("服务器启动", err)

	for {
		var data [1024]byte
		_, _,err := listen.ReadFromUDP(data[:]) // 接收数据
		t := time.Now().UnixNano() / 1e6
		if err != nil {
			fmt.Println("read udp failed, err: ", err)
			continue
		}
		result1 := reg1.FindStringSubmatch(string(data[:]))
		fmt.Printf("收到第%d条消息：%s\n",cont, string(data[:]))
		cont=cont+1
		sen, _ := strconv.ParseInt(result1[2], 10, 64)
		fmt.Printf("该条消息的发送-接收时间差为:%vms\n", sen-t)
		if result1[1]=="99" {
			break
		}
	}
	fmt.Printf("接收到的数据包数量：%d\n丢包率：%.3f%%\n",cont,float32(100-cont));
	fmt.Printf("服务端运行结束\n");

}