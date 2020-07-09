#include <iostream>
#include <stdio.h>
#include "Felica.h"
#include "logo.h"
#include "mc_led.h"
#include "oslib.h"
#include "tm1680.h"
#include "toolslib.h"

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

// converts character array 
// to string and returns it 
string convertToString(char* a, int size)
{ 
    int i; 
    string s = ""; 
    for (i = 0; a[i] != 0; i++) { 
        s = s + a[i]; 
    } 
    return s; 
}

int SendToPrinter(void)
{
	int com2fd;
	int ret;
	int i;
	unsigned char pBuffer[1024]={0};
	unsigned char wbuffer[1024]={0};
	char dispbuff[25]={0};
	
	com2fd = OSUART_Init("/dev/ttymxc1","9600,n,8,1");
	if(com2fd == 0)
	{
		printf("open com2 err!\n");
	}
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(10, DISP_LEFT,"Start the com2 test:");
	// print_start("Start the com2 test");
	memset(wbuffer, 'k', 1023);
	
	OSUART_TxStr(com2fd, wbuffer, 1023);
	memset(pBuffer, 0, sizeof(pBuffer));
	for(i = 0; i < 20; i++)
	{
		ret = OSUART_RxStr(com2fd,pBuffer,sizeof(pBuffer),100);
		if(ret>0)
    	{
			ret = memcmp(pBuffer, wbuffer, 1023);
			if(ret == 0)
			{
				memset(pBuffer, 0, sizeof(pBuffer));
				ret = OSUART_RxStr(com2fd,pBuffer,sizeof(pBuffer),500);
				if(ret > 0)
				{
					    ret = Sys_SetRealTime(pBuffer);
    					printf("try to reset time to %s[%d]\n",pBuffer,ret);
						sprintf(dispbuff,"%.4s-%.2s-%.2s %.2s:%.2s:%.2s\n",
            			&pBuffer[0],&pBuffer[4],&pBuffer[6],
            			&pBuffer[8],&pBuffer[10],&pBuffer[12]);
						LCD_Printf(70, DISP_FONT6X8|DISP_RIGHT,dispbuff);
				}
				LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test com2 success");
				// print_greenok("test com2 success");
				Sys_BeepMs(100);	
				//OSTIMER_DelayMs(2000);
				OSUART_Flush(com2fd);
				return 0;
			}
		}
	}
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test com2 error");
	// print_rederr("test com2 error");
	//OSTIMER_DelayMs(3000);
	OSUART_Flush(com2fd);
	return 1;
}

int main(int argc, char **argv)
{
    std::cout << "Hello Validator!" << std::endl;
    // HelloTcp();

    // QR Reader Initialization
    int qrfd1,qrfd2,ret;
	unsigned char TmpBuff[1024];
    qrfd1 = QRCode_Open(0);
    qrfd2 = QRCode_Open(1);
	unsigned long tick_last = 0,tick_end;
	unsigned int succ_num = 0;
	unsigned int tick_start = 0;
	
	printf("\n******** Start to test QR code ********>>\n");
    printf("scan the QR code>>\r\n");


    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        cout << "Error on initializing the socket!";
        return 1;
    }

    // Create a hint structure for the server we're connecting with
    int port = 8080;
    string ipAddress = "192.168.233.98";
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
    
    // Connect to the server on the socket
    int connectRes = connect(sock, (sockaddr *)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        return 1;
    }
    
    // While loop:
    char buf[4096];
    string userInput;
    string userInput1 = "";
    do
    {
        // Enter lines of text
        // cout << "> ";
        // getline(cin, userInput);
        // userInput = "akbar";

        // wait untill qr reads something
        do {
            ret = QRCode_RxStr(qrfd1, TmpBuff, 1024, 100);
            if(ret <= 0)
                ret = QRCode_RxStr(qrfd2, TmpBuff, 1024, 100);

            if(ret > 0)
            {
                tick_end = OSTIMER_GetTickCount() ;
                if(tick_start == 0)
                    tick_start = tick_end;
                printf("\nscan the QR code:[length %2d] : [%s]\r\n",ret,TmpBuff);
                succ_num ++;
                
                if(tick_last)
                    printf("[time interval %ld ms] time interval%d ms(%d)\n",tick_end - tick_last,
                    (tick_end - tick_start)/succ_num,succ_num);
                else
                    printf("Start to test the scan time interval\n");
                tick_last = tick_end;
                Sys_BeepMs(100);
            }
            continue;
        } while (ret < 0);


        // JSON generation
        userInput = "";
        // userInput += "\n\r{\n\r";
        // userInput += "    \"method\" : \"PerformTransaction\"\n\r";
        // userInput += "    \"qrcode\" : \"";
        userInput += convertToString(TmpBuff, sizeof(TmpBuff) / sizeof(char));
        // userInput += "\"\n\r}\n\r";

        // cout << "this is converted:" << userInput << "And this is original:" << TmpBuff << endl;

        // Send to server
        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        
        if (sendRes == -1)
        {
            cout << "Could not send to server! Whoops!\r\n";
            continue;
        }

        // Wait for response
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            cout << "There was an error getting response from server\r\n";
        }
        else
        {
            // Display response
            cout << "SERVER> " << string(buf, bytesReceived) << "\r\n";
        }

    } while (true);
    // Close the socket
    close(sock);
    // QR Reader DeInit
    QRCode_Close(qrfd1);
    QRCode_Close(qrfd2);

    return 0;
}

// export LD_LIBRARY_PATH=/opt/lib:$LD_LIBRARY_PATH