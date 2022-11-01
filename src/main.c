#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <inttypes.h>
#include <stdlib.h>

int main() {
    int status;
    struct sockaddr_in sa = {
        .sin_family = AF_INET,
        .sin_port = htons(80)
    };

    inet_pton(AF_INET, "127.0.0.1", &(sa.sin_addr));

    int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    status = bind(socket_fd, (struct sockaddr*) &sa, sizeof(sa));

    if (status < 0) {
        printf("Failed to bind\n");
        return 1;
    }

    while (1) {
        printf("Listening at port %" PRIu16 "\n", ntohs(sa.sin_port));
        status = listen(socket_fd, 10);
        if (status < 0) {
            printf("Failed to listen\n");
            break;
        }

        struct sockaddr sa_incoming = {0};
        socklen_t len;

        int new_fd = accept(socket_fd, &sa_incoming, &len);

        if (new_fd < 0) {
            printf("Failed to accpet\n");
            break;
        }

        char *buffer = malloc(sizeof(char) * 20);
        inet_ntop(sa_incoming.sa_family, sa_incoming.sa_data, buffer, 20);
        printf("%s\n", buffer);
        free(buffer);
    }
}