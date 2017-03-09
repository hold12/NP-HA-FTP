#include <iostream>
#include "FTPClient.h"

using namespace std;

int main() {
	char ip[] = "130.226.195.126";
	char *ip_pointer = ip; //Casting ip-address to a char-pointer
	FTPClient c;

	//Establishing connection
	c.Connect(21, ip_pointer);
	c.RecvMsg();
	c.SendMsg("HELLO\r\n", 7);
	c.RecvMsg();

	//Logging in
	c.SendMsg("USER anonymous\r\n", 16);
	c.RecvMsg();
	c.SendMsg("PASS s165232@dtu.dk\r\n", 21);
	c.RecvMsg();

	//Entering Passive Mode
	c.SendMsg("PASV\r\n", 6);
	int a1, a2, a3, a4, p1, p2;
	sscanf_s(c.RecvMsg(), "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d).\r\n", &a1, &a2, &a3, &a4, &p1, &p2);
	int dataPort = (p1 * 256) + p2;

	//Opening data connection for 1st file
	FTPClient data;
	data.Connect(dataPort, ip_pointer);
	c.SendMsg("RETR file.txt\r\n", 15); //small file of 12 bytes
	c.RecvMsg();
	c.RecvMsg();
	data.SaveFile("file.txt");
	data.CloseCon();

	//Entering Passive Mode again
	c.SendMsg("PASV\r\n", 6);
	sscanf_s(c.RecvMsg(), "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d).\r\n", &a1, &a2, &a3, &a4, &p1, &p2);
	dataPort = (p1 * 256) + p2;

	//Opening new data connection for 2nd file
	data.Connect(dataPort, ip_pointer);
	c.SendMsg("CWD /pub/62501/HOWTOs/NAT-HOWTO\r\n", 33);//bigger file of 1359 bytes
	c.RecvMsg();
	c.SendMsg("RETR NAT-HOWTO-3.html\r\n", 23);
	c.RecvMsg();
	c.RecvMsg();
	data.SaveFile("NAT-HOWTO-3.html");
	cin.get();
	c.CloseCon();
	return 0;
}