#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <termios.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>
#include <resolv.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define MAXBUF 1024

void ShowCerts(SSL * ssl)
{
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL) {
        printf("Digital cert info:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Cert: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Owner: %s\n", line);
        free(line);
        X509_free(cert);
    } else
        printf("No cert info\n");
}

int callhome_ssl_connect(int sockfd)
{
    int len;
    struct sockaddr_in dest;
    char buffer[MAXBUF + 1];
    SSL_CTX *ctx;
    SSL *ssl;


    /* SSL 库初始化，參�? ssl-server.c 代码 */
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(TLSv1_2_client_method());
    if (ctx == NULL) 
	{
        ERR_print_errors_fp(stdout);
        exit(-1);
    }



    /* 基于 ctx 产生一�?新的 SSL */
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);
    /* 建立 SSL 连接 */
    if (SSL_connect(ssl) == -1)
        ERR_print_errors_fp(stderr);
    else {
        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
        ShowCerts(ssl);
    }
    /* 接收对方发过来的消息，最多接�? MAXBUF �?字节 */
    bzero(buffer, MAXBUF + 1);
    /* 接收服务器来的消�? */
    len = SSL_read(ssl, buffer, MAXBUF);
    if (len > 0)
        printf("recv message '%s' ok.total size:'%d'\n",
               buffer, len);
    else {
        printf("recv message error!error code:'%d',error info:'%s'\n",
             errno, strerror(errno));
        goto finish;
    }
    bzero(buffer, MAXBUF + 1);
    strcpy(buffer, "from client->server");
    /* 发消�?给服务器 */
    len = SSL_write(ssl, buffer, strlen(buffer));
    if (len < 0)
        printf("send message '%s' error!error code:'%d',error info:'%s'\n",
             buffer, errno, strerror(errno));
    else
        printf("send message '%s' ok,total size:'%d'\n",buffer, len);

  finish:
    /* 关闭连接 */
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sockfd);
    SSL_CTX_free(ctx);
    return 0;
}

//使用多线程解决并�?
//pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
//为什么选择多线程解决并发问�?,而不选择多进�?
void serial_port_init(int fd);
void *pthread_fun(void *args)
{
    int *cidp = (int *)args;
    int cid = *cidp;
    char buf[128] = "";
    int count = -1;

    callhome_ssl_connect(cid);
    //5.发送或接收数据:send(),write()/recv(),read();
    while (strncmp(buf, "quit", 4))
    {
        read(cid, buf, sizeof(buf));
        printf("接收�?:%s\n", buf);
        write(cid, buf, strlen(buf));
        memset(buf, 0, sizeof(buf));
		sleep(1);
    }
    //6.关闭套接�?:close();
    shutdown(cid, SHUT_RDWR);
    close(cid);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("usage : ./a.out ip port\n");
        return -1;
    }

    int sid = socket(AF_INET, SOCK_STREAM, 0);
    if (sid < 0)
    {
        perror("socket error");
        return -1;
    }

    //2.绑定到本机ip和�??口号bind();
    struct sockaddr_in saddr = {0};
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(atoi(argv[2]));
    saddr.sin_addr.s_addr = inet_addr(argv[1]);
    int reuseaddr = 1;
    if (setsockopt(sid, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) < 0) //设置地址复用
    {
        perror("reuseaddr error");
        close(sid);
        return -1;
    }

    //�?口�?�用
    if (bind(sid, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    {
        perror("bind error: port binded");
        close(sid);
        return -1;
    }

    printf("listen sid = %d\n", sid);
    //3.把�?�接字�?�置为监�?状态listen();
    if (listen(sid, 10) < 0)
    {
        perror("listen error");
        close(sid);
        return -1;
    }

    //4.等待客户�?连接请求:accept();
    int cid = -1;
    pthread_t tid;

    //设置线程分�??
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    //当有客户�?连接时，会有两个线程:主线程�?�理客户�?连接请求，并创建一�?子线程与网络客户�?交互,子线�?:负责与网络�?�户�?交互
    while (1)
    {
        cid = accept(sid, NULL, NULL);
        printf("accept cid=%d\n", cid);
        if (cid < 0)
        {
            perror("accept error");
            continue;
        }

        pthread_create(&tid, &attr, pthread_fun, &cid); //创建子线�?
        sleep(1000);
    }
    close(sid);
}
