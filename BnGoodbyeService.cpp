#define LOG_TAG "GoodbyeService"
#include "IGoodbyeService.h"

namespace android {
	BnGoodbyeService::BnGoodbyeService() {}
	BnGoodbyeService::BnGoodbyeService(int fd) {this->fd = fd;}
status_t BnGoodbyeService::onTransact(uint32_t code, const Parcel& data, Parcel *reply, uint32_t flags)
{
	switch(code) {
		case GOODBYE_SVR_CMD_SAYGOODBYE:
		{
			saygoodbye();
			reply->writeInt32(0);
			return NO_ERROR;
		}break;

		case GOODBYE_SVR_CMD_SAYGOODBYE_TO:
		{
			int32_t policy = data.readInt32();
			String16 name_svc = data.readString16();
			String16 name16 = data.readString16();
			String8 name8(name16);

			int cnt = saygoodbye_to(name8.string());
			reply->writeInt32(0);
			reply->writeInt32(cnt);
			return NO_ERROR;
		}break;

		case GOODBYE_SVR_CMD_GET_FD:
		{
			int fd = this->get_fd();
			reply->writeInt32(0);
			reply->writeDupFileDescriptor(fd);
			return NO_ERROR;
		}break;

	
		default:
		return BBinder::onTransact(code, data, reply, flags);
	}

}

void BnGoodbyeService::saygoodbye(void)
{
	static int cnt = 0;
	ALOGI("saygoodbye : %d\n", ++cnt);

}

int BnGoodbyeService::saygoodbye_to(const char *name)
{
	static int cnt = 0;
	ALOGI("saygoodbye_to %s, cnt = %d", name, ++cnt);
	return cnt;
}

int BnGoodbyeService::get_fd(void)
{
	return this->fd;
}
}
