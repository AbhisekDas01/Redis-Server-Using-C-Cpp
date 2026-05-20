#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cerrno> //global variable the kernel use to store the last error

const size_t k_max_msg = 4096; //specify maximum message length


static void die(const char *msg) {
    int err = errno;
    std::cerr << "[" << err << "] " << msg << std::endl; /*File Print Format: Unlike standard printf (which always prints to the screen), fprintf allows you to specify exactly where the text should be sent (to a file, a stream, or the screen).*/
    abort();
}

static void msg(const char *msg) {
    std::cerr << msg << std::endl;
}

static int32_t readFull(int fd , char *buf , size_t n) {

    while (n > 0){
        ssize_t rv = read(fd , buf , n);

        if(rv <= 0) {
            if(errno == EINTR) { //If we were interrupted by a signal, do NOT exit!
                continue;
            }
            return -1; // -1 error, or  0 unexpected EOF
        }

        assert((size_t)rv <= n); /*If the condition is TRUE: The program assumes everything is fine and quietly moves on to the next line of code. There is zero disruption.

        If the condition is FALSE: The program instantly halts, prints the exact file name and line number to the screen, and crashes right there on the spot.*/
        n -= (size_t)rv;
        buf += rv; //move pointer to read from new slots
        
    }
    return 0; 
}

static int32_t writeFull(int fd ,const char *buf , size_t n) {
    while(n > 0) {

        ssize_t rv = write(fd , buf , n);

        if(rv <= 0) {
            return -1;
        }

        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv; //move pointer forward to write new places;

    }
    return 0;
}

static int32_t query(int fd , const char *text) {

    //format the message to be sent 
    uint32_t len = (uint32_t)strlen(text);
    if(len > k_max_msg) {
        return -1;
    }

    //send message 
    char wbuf[4+k_max_msg];

    //fill the length of the message in the first 4 bytes
    memcpy(wbuf , &len , 4);
    memcpy(&wbuf[4] , text , len); //fill the message from the next index

    int32_t err = writeFull(fd , wbuf , 4+len);
    if(err) {
        return err;
    }

    //read the recived msg
    char rbuf[4+k_max_msg];
    errno = 0;
    err = readFull(fd , rbuf , 4); //extract the length of the recieved message from the 1st 4 bytes
    if(err) {
        msg(errno == 0 ? "EOF" : "read() error");
        return err;
    }
    memcpy(&len , rbuf , 4); //copty the length from rbuf to len 

    if(len > k_max_msg) {
        msg("too long");
        return -1;
    }

    //read the message 
    err = readFull(fd , &rbuf[4] , len);
    if(err) {
        msg("Read() error");
        return err;
    }

    //print the result
    std::cout << "The Server says: " << std::string_view(&rbuf[4] , len) << std::endl;

    return 0;
}


int main(){
    
    int fd = socket(AF_INET , SOCK_STREAM , 0);
    if(fd < 0) {
        die("Socket()");
    }

    //binding
    struct sockaddr_in addr = {}; //initiazlise a sockadder_in sturcture
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234); //htons converts the big endian  representation to little endian
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK); //INADDR_LOOPBACK = 127.0.0.1(localhost)

    int rv = connect(fd , (const struct sockaddr *) & addr , sizeof(addr));
    if(rv) {
        die("Connect()");
    }

    //send multiple  message
    int32_t i = 0;
    while(i < 10) {

        char message[8] = "Hello ";
        message[6] = (char)('0' + i++);

        int32_t err = query(fd , message);

        if(err) {
            goto L_DONE;
        }
    }
    
    L_DONE:
         close(fd);
         return 0;
}