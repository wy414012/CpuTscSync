/* 为x79主板优化的设置*/

#include <IOKit/IOService.h>
#include <IOKit/IOLib.h>
#include <i386/proc_reg.h>

//reg define
#define MSR_IA32_TSC                    0x00000010

//extern function defined in mp.c from xnu
extern "C" void  mp_rendezvous_no_intrs(void (*action_func)(void*), void *arg);

class CpuTSCSync : public IOService
{
    typedef IOService super;
    OSDeclareDefaultStructors(CpuTSCSync)

private:
	void doTSC(void);

public:
    virtual IOService* probe(IOService* provider, SInt32* score);
    virtual bool start(IOService* provider);
    virtual void stop(IOService* provider);
    virtual IOReturn setPowerState(unsigned long powerStateOrdinal, IOService* whatDevice);
};
