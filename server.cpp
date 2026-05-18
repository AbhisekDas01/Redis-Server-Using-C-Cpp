#include <stdio.h> //standard i/O header
#include <sys/socket.h> 

int main() {

            //socket(domain , conn-type , protocol)
    int fd = socket(AF_INET , SOCK_STREAM , 0);
    
    /*
        1.AF_INET is for IPv4. Use AF_INET6 for IPv6 or dual-stack sockets.
        2.SOCK_STREAM is for TCP. Use SOCK_DGRAM for UDP.
        3.The 3rd argument is 0 and useless for our purposes.
     */

}