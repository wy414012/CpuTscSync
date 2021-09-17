/*  用这个做你想做的。
  这项工作源于 Turbo 和
  戴尔所有者cosmo1t 。
 *
 */

#include <stdatomic.h>

#include <IOKit/IOService.h>
#include <IOKit/IOLib.h>
#include <i386/proc_reg.h>

//注册定义
#define MSR_IA32_TSC                    0x00000010

//xnu 的 mp.c 中定义的 extern 函数
extern "C" void  mp_rendezvous_no_intrs(void (*action_func)(void*), void *arg);

class TSCSync : public IOService
{
    typedef IOService super;
    OSDeclareDefaultStructors(TSCSync)

private:
	static void doTSC(void);

public:
    static bool isTscSynced() { return tsc_synced; };

    virtual IOService* probe(IOService* provider, SInt32* score) override;
    virtual bool start(IOService* provider) override;
    virtual void stop(IOService* provider) override;
    virtual IOReturn setPowerState(unsigned long powerStateOrdinal, IOService* whatDevice) override;
    
protected:
    static _Atomic(bool) tsc_synced;
};
