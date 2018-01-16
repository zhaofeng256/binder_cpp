#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <cutils/properties.h>
#include <utils/Log.h>
#include <unistd.h>

#include "IHelloService.h"
#include "IGoodbyeService.h"

#define SOCKET_BUFFER_SIZE (32768U)

using namespace android;

class Mythread : public Thread {
	private:
		int fd;
public:
		Mythread();
		Mythread(int fd) :fd(fd) {};

		bool threadLoop() {
			char buf[500];
			int len;
			int cnt = 0;

			while(1) {
				len = read(fd, buf, 500);
				buf[len] = '\0';
				ALOGI("%s", buf);

				len = sprintf(buf, "Hello, test client, cnt = %d", ++cnt);
				write(fd, buf, len);
			}

			return true;
		}
};


int main(int argc, char **argv)
{

	int sockets[2];
	socketpair(AF_UNIX, SOCK_SEQPACKET, 0, sockets);


	int buffersize = SOCKET_BUFFER_SIZE;
	setsockopt(sockets[0], SOL_SOCKET, SO_SNDBUF, &buffersize, sizeof(buffersize));
	setsockopt(sockets[0], SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize));
	setsockopt(sockets[1], SOL_SOCKET, SO_SNDBUF, &buffersize, sizeof(buffersize));
	setsockopt(sockets[1], SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize));

	sp<Mythread> th = new Mythread(sockets[0]);
	th->run();

	sp<ProcessState> proc(ProcessState::self());
	sp<IServiceManager> sm = defaultServiceManager();

	sm->addService(String16("hello"), new BnHelloService(sockets[1]));
	sm->addService(String16("goodbye"), new BnGoodbyeService());
	
	ProcessState::self()->startThreadPool();
	IPCThreadState::self()->joinThreadPool();

	return 0;

}
