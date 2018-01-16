#include "IGoodbyeService.h"

namespace android {
class BpGoodbyeService: public BpInterface<IGoodbyeService>
{
    public:
	BpGoodbyeService(const sp<IBinder>& impl):BpInterface<IGoodbyeService>(impl) {}

	void saygoodbye(void) {
		Parcel data, reply;
		data.writeInt32(0);
		data.writeString16(String16("IGoodbyeService"));
		remote()->transact(GOODBYE_SVR_CMD_SAYGOODBYE, data, &reply);
	}

	int saygoodbye_to(const char *name) {
		Parcel data, reply;
		data.writeInt32(0);
		data.writeString16(String16("IGoodbyeService"));
		data.writeString16(String16(name));
		remote()->transact(GOODBYE_SVR_CMD_SAYGOODBYE_TO, data, &reply);

		int exception = reply.readInt32();
		if(exception) {
			return -1;
		} else {
			return reply.readInt32();
		}
	}

	int get_fd(void) {
		Parcel data, reply;
		int exception;

		data.writeInt32(0);
		data.writeString16(String16("IGoodbyeService"));
		remote()->transact(GOODBYE_SVR_CMD_GET_FD, data, &reply);

		exception = reply.readInt32();
		if(exception) {
			return -1;
		} else {
			int rawFd = reply.readInt32();
			return dup(rawFd);
		}
	
	}
};

IMPLEMENT_META_INTERFACE(GoodbyeService, "android.media.IGoodbyeService");


}
