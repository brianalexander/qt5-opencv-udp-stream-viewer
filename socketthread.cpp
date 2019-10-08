#include "socketthread.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "opencv2/opencv.hpp"

#define PORT 12345
#define PACK_SIZE 4096
#define BUF_LEN 65540

void SocketThread::run()
{
    int sockfd;
    char *tempBuf = new char[PACK_SIZE];

    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // fill servaddr & tempBuf with zeros
    memset(&servaddr, 0, sizeof(servaddr));
    memset(tempBuf, 0, sizeof(tempBuf));

    servaddr.sin_family = AF_INET; // ipv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // bind the socket with the server address
    int result;
    result = bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    if (result < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int n = 0;
    int numPacks = 8;
    int recvMsgSize;

    while (true)
    {
        // the do-while below is in charge of finding a packet that is a single int
        // We then get that int, which presents the number of packets needed to get
        // the complete image.
        // If it gets data or misses data, it will continue to throw away messages
        // until it finds another int, which is the start of a new image.
        do
        {
            recvMsgSize = recvfrom(sockfd, tempBuf, BUF_LEN, 0, nullptr, nullptr);
        } while (recvMsgSize > sizeof(int));

        // treat tempBuf as an int array and get the first element
        numPacks = ((int *)tempBuf)[0];

        // create a byte array that is the size of the incomming image
        char *buffer = new char[numPacks * PACK_SIZE];

        for (int i = 0; i < numPacks; i++)
        {
            n += recvfrom(sockfd, &(buffer[i * PACK_SIZE]), PACK_SIZE, MSG_WAITALL, nullptr, nullptr);
        }

        //display bytes recieved and reset count to 0
        printf("bytes recieved : %i\n", n);
        n = 0;

        // Decode the image
        cv::Mat rawData = cv::Mat(1, PACK_SIZE * numPacks, CV_8UC1, buffer);
        cv::Mat frame = cv::imdecode(rawData, cv::IMREAD_COLOR);
        if (frame.size().width == 0)
        {
            std::cerr << "decode failure" << std::endl;
            continue;
        }

        // release memory back to the heap
        free(buffer);

        emit frameReady(frame);
    }

}
