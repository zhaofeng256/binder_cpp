#ifndef ANDROID_IGOODBYESERVICE_H
#define ANDROID_IGOODBYESERVICE_H


#include <utils/Errors.h>
#include <utils/KeyedVector.h>
#include <utils/RefBase.h>
#include <utils/String8.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>


#define GOODBYE_SVR_CMD_SAYGOODBYE 1
#define GOODBYE_SVR_CMD_SAYGOODBYE_TO 2
#define GOODBYE_SVR_CMD_GET_FD 3

namespace android {

class IGoodbyeService: public IInterface
{
	public:
        DECLARE_META_INTERFACE(GoodbyeService);
        virtual void saygoodbye(void) = 0;
        virtual int saygoodbye_to(const char *name) = 0;
        virtual int get_fd(void) = 0;

};


class BnGoodbyeService: public BnInterface<IGoodbyeService>
{
	private:
		int fd;
	public:
		status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags = 0);
		void saygoodbye();
		int saygoodbye_to(const char *name);
		int get_fd(void);

		BnGoodbyeService();
		BnGoodbyeService(int fd);
};

}

#endif
