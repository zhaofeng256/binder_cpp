#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <cutils/properties.h>
#include <utils/Log.h>
#include <unistd.h>

#include "IHelloService.h"
#include "IGoodbyeService.h"

using namespace android;

int main(int argc, char **argv)
{
	int cnt;
	if(argc < 2) {
		ALOGI("Usage:");
		ALOGI("%s <readfile>\n", argv[0]);
		ALOGI("%s <hello|goodbye>", argv[0]);
		ALOGI("%s <hello|goodbye> [name]", argv[0]);
		return -1;
	}

	sp<ProcessState> proc(ProcessState::self());
	sp<IServiceManager> sm = defaultServiceManager();
	
	if(strcmp(argv[1], "hello") == 0) {
		sp<IBinder> binder = sm->getService(String16("hello"));
		if (NULL == binder.get()) {
			ALOGE("fail to get hello service");
			return -1;
		}

		sp<IHelloService> service = interface_cast<IHelloService>(binder);

		if(argc < 3) {
			service->sayhello();
			ALOGI("client say hello");
		} else {
		
			cnt = service->sayhello_to(argv[2]);
			ALOGI("client say hello to %s, ret = %d", argv[2], cnt);
		}
	}
	else if(strcmp(argv[1], "goodbye") == 0) {
		sp<IBinder> binder = sm->getService(String16("goodbye"));
		if(NULL == binder.get()) {
			ALOGE("fail to get goodbye service");
			return -1;
		}

		sp<IGoodbyeService> service = interface_cast<IGoodbyeService>(binder);

		if(argc < 3) {
			service->saygoodbye();
			ALOGI("client say goodbye");
		} else {
		
			cnt = service->saygoodbye_to(argv[2]);
			ALOGI("client say goodbye to %s, ret = %d", argv[2], cnt);
		}
	}
	else if(strcmp(argv[1], "readfile") == 0) {
		sp<IBinder> binder = sm->getService(String16("hello"));
		if(NULL == binder.get()) {
			ALOGE("fail to get hello service");
			return -1;
		}

		sp<IHelloService> service = interface_cast<IHelloService>(binder);
		int fd = service->get_fd();
		ALOGI("client get_fd = %d", fd);

		char buf[500];
		int len;
		int cnt = 0;


		while(1) {
			len = sprintf(buf, "Hello, test_server, cnt = %d", cnt++);
			write(fd, buf, len);

			len = read(fd, buf, 500);
			buf[len] = '\0';
			ALOGI("%s", buf);
			sleep(5);

		}
	}

	return 0;
}

