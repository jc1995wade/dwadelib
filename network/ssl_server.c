#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define MAXBUF 1024 * 100
#define SERVER_CERT     "cert.pem"
#define SERVER_KEY      "key.pem"
const char *rootKey = "123456";

/*自动填入密码*/
int PemPasswdCb(char *buf, int size, int rwflag, void *password)
{
    strncpy(buf, (char *)(password), size);
    buf[size - 1] = '\0';
    return(strlen(buf));
}

/*openssl库的初始化*/
void LibSslInit()
{
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
}


/*启动服务器监听*/
int StartListenServer(unsigned int portnum)
{
    struct sockaddr_in my_addr;
    int listenfd = -1;
    int reuse = 1;
    int lisnum = 5;
    int ret  = -1;
    listenfd = socket(PF_INET, SOCK_STREAM, 0);

    if (listenfd == -1) {
        perror("socket");
        exit(1);
    }
    ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    if (ret < 0) {
        printf("setsockopet error\n");
        return -1;
    }

    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(portnum);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(listenfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr));
    if (ret == -1) {
        perror("bind");
        exit(1);
    }

    ret = listen(listenfd, lisnum);
    if (ret == -1) {
        perror("listen");
        exit(1);
    }

    return listenfd;
}

/*加载配置文件*/
void SslLoadConf(SSL_CTX *ctx)
{
    /*加载公钥证书*/
    if (SSL_CTX_use_certificate_file(ctx, SERVER_CERT, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    /*设置私钥*/
    if (SSL_CTX_use_PrivateKey_file(ctx, SERVER_KEY, SSL_FILETYPE_PEM) <= 0) {
        printf("use private key fail.\n");
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    if (!SSL_CTX_check_private_key(ctx)) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
}

/*设置密码从程序里面读取*/
void SslSetPassWd(SSL_CTX *ctx)
{
    SSL_CTX_set_default_passwd_cb_userdata(ctx, (void *)rootKey);
    SSL_CTX_set_default_passwd_cb(ctx, PemPasswdCb);
}

/*构造响应消息*/
int BuildSendBuf(char *buf, int maxBufLen)
{
    int bufLen = 0;
    char fileBuf[1024] = {0};
    int i;

    for (i = 0; i < 1024; i++)
    {
        fileBuf[i] = '1';
    }

    bufLen += snprintf(buf, maxBufLen, "%s", "HTTP/1.1 200 OK\r\n Server: Apache/1.1\r\n Content-Length:1024\r\n Content-Type:text/plain\r\n");
    bufLen += snprintf(buf + bufLen, maxBufLen, "%s", fileBuf);

    return bufLen;
}

/*处理SSL连接*/
int HandleSslConnect(SSL_CTX *ctx, int clientfd)
{
    SSL *ssl = NULL;
    char buf[MAXBUF + 1];
    int i = 0;
    int bufLen = 0;
    int ret = -1;
    int len;

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, clientfd);
    if (SSL_accept(ssl) == -1) {
        perror("ssl accept");
        ret = -1;
        goto finish;
    }

    bzero(buf, MAXBUF + 1);
    len = SSL_read(ssl, buf, MAXBUF);
    if (len <= 0) {
        printf("read msg failed");
                    ret = -1;
        goto finish;
    }
    printf("recv buf is %s.\n", buf);

    bufLen = BuildSendBuf(buf, MAXBUF);

    len = SSL_write(ssl, buf, bufLen);
    if (len <= 0) {
        printf("msg '%s' send failed %d！\n", buf);
        ret = -1;
        goto finish;
    }
    printf("msg '%s' send success，%d byte！\n", buf, len);
    ret = 0;
finish:
    SSL_shutdown(ssl);
    SSL_free(ssl);

    return ret;
}

int main(int argc, char **argv)
{
    int listenfd = -1;
    int clientfd = -1;
    int ret = -1;
    socklen_t len;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    unsigned int myport;
    SSL_CTX *ctx;

    if (argv[1]) {
        myport = atoi(argv[1]);
    } else {
        myport = 8888;
    }

    LibSslInit();
    ctx = SSL_CTX_new(SSLv23_server_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    SslSetPassWd(ctx);
    SslLoadConf(ctx);

    listenfd = StartListenServer(myport);

    while (1) {
        len = sizeof(struct sockaddr);
        clientfd = accept(listenfd, (struct sockaddr *) &their_addr, &len);
        if (clientfd == -1) {
            perror("accept");
            continue;
        } 
        printf("server: got connection from %s, port %d, socket %d\n",inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), clientfd);
        ret = HandleSslConnect(ctx, clientfd);
        if (ret != 0)
        {
           printf("handle ssl connect failed close clientfd %d.\n", clientfd);
        }
finish:
        close(clientfd);
    }

    close(listenfd);
    SSL_CTX_free(ctx);
    return 0;
}