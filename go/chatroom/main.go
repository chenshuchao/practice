package main

import (
	"./server"
)

func main() {
	newServer := &server.IpcServer{}
	newServer.Start()
	newServer.Stop()
}
