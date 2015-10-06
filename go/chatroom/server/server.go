package server

import (
	"fmt"
	"net"
	"os"
	"strconv"
	"strings"
	//"../webService"
)

type IpcServer struct {
	ln       net.Listener
	clientDB map[string]*IpcClient
}

type IpcClient struct {
	nickname string
	conn     net.Conn
	channel  chan string
	rQuit    chan bool
	wQuit    chan bool
}

func (server *IpcServer) Init(addr string, protocol string) {
	ln, err := net.Listen(protocol, addr)
	if err != nil {
		fmt.Println("error listen:", err)
	}
	server.ln = ln

	clientDB := make(map[string]*IpcClient)
	server.clientDB = clientDB
}

func (server *IpcServer) NewIpcClient(nickname string, conn net.Conn) *IpcClient {
	channel := make(chan string)
	rQuit := make(chan bool)
	wQuit := make(chan bool)
	return &IpcClient{nickname, conn, channel, rQuit, wQuit}
}

func (server *IpcServer) Login(conn net.Conn) *IpcClient {
	for {
		respMsg := "请输入您的昵称: "
		conn.Write([]byte(respMsg))
		reqMsg := make([]byte, 100)
		_, err := conn.Read(reqMsg[0:])
		CheckError(err)

		nickname := Trim(string(reqMsg))

		_, existed := server.clientDB[nickname]
		if existed {
			respMsg = "此昵称已存在，亲换一个重试吧～\n"
			conn.Write([]byte(respMsg))
		} else {
			client := server.NewIpcClient(nickname, conn)
			server.clientDB[nickname] = client

			server.HandleChat(client, client.nickname+"已上线\n")
			respMsg = "欢迎加入聊天室，当前在线人数为 " + strconv.Itoa(len(server.clientDB)) + " 人\n"
			conn.Write([]byte(respMsg))
			return client
		}
	}
}

func (server *IpcServer) Logout(client *IpcClient) bool {
	msg := client.nickname + "已下线\n"
	server.HandleChat(client, msg)
	delete(server.clientDB, client.nickname)
	client.conn.Close()
	client.rQuit <- true
	client.wQuit <- true
	fmt.Println("delete")
	fmt.Println("num:")
	fmt.Println(len(server.clientDB))
	return true
	//释放 client？
}

/*
* 处理命令指令
 */
func (server *IpcServer) HandleCmd(client *IpcClient, msg []string) {

	switch {
	case msg[0] == "quit":
		server.Logout(client)
	/*
		case msg[0] == "天气"
			webService.getByArea(msg[1])
	*/
	default:
		//
	}

}

/*
* 处理聊天内容
 */
func (server *IpcServer) HandleChat(client *IpcClient, msg string) {
	for nickname, cln := range server.clientDB {
		if nickname != client.nickname {
			cln.channel <- msg
		}
	}
}

/*
*接受客户端消息
 */
func (server *IpcServer) Receive(client *IpcClient) {
	for {

		select {

		case <-client.rQuit:
			break

		default:
			reqMsg := make([]byte, 100)

			_, err := client.conn.Read(reqMsg[0:])
			CheckError(err)

			msg := Trim(string(rmZero(reqMsg)))

			if msg[0] == '/' {
				msgArr := strings.Split(msg[1:], " ")
				server.HandleCmd(client, msgArr)
			} else {
				msg = client.nickname + ":" + msg + "\n"
				server.HandleChat(client, msg)
			}
		}
	}
}

/*
*发送消息到客户端
 */
func (server *IpcServer) Send(client *IpcClient) {
	for {

		select {
		case str := <-client.channel:
			msg := []byte(str)
			client.conn.Write(msg)

		case <-client.wQuit:
			break
		}

	}
}

/*
*处理每个客户端事务
 */
func (server *IpcServer) Handle(conn net.Conn) {
	client := server.Login(conn)
	go server.Receive(client)
	go server.Send(client)
}

/*
*服务器启动
 */
func (server *IpcServer) Start() {

	addr := ":5011"
	protocol := "tcp"
	server.Init(addr, protocol)
	for {
		conn, err := server.ln.Accept()
		if err != nil {
			fmt.Println("Get conn error:", err)
			os.Exit(1)
		}
		go server.Handle(conn)
	}
}

/*
*服务器关闭
 */
func (server *IpcServer) Stop() {
	server.ln.Close()
}

/*
 一些常用工具方法，最好将其放到单独的工具库
*/
func CheckError(err error) {
	if err != nil {
		fmt.Println("Error: ", err)
	}
}

func Trim(s string) string {
	return strings.Replace(s, "\r\n", "", -1)
}

func rmZero(chs []byte) []byte {
	i := len(chs) - 1
	for ; i >= 0; i-- {
		if chs[i] != 0 {
			break
		}
	}
	return chs[0 : i+1]
}
