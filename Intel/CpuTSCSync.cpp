#include <libkern/version.h>
#include "CpuTSCSync.h"

OSDefineMetaClassAndStructors(CpuTSCSync, IOService)

//stamp the tsc
extern "C" void stamp_tsc(void *tscp)
{
	wrmsr64(MSR_IA32_TSC, *reinterpret_cast<uint64_t*>(tscp));
}

IOService* CpuTSCSync::probe(IOService* provider, SInt32* score)
{
    if (!super::probe(provider, score)) return NULL;
    if (!provider) return NULL;

    // only attach to last CPU
    uint16_t threadCount = rdmsr64(MSR_CORE_THREAD_COUNT);
    OSNumber* cpuNumber = OSDynamicCast(OSNumber, provider->getProperty("IOCPUNumber"));
    if (!cpuNumber || cpuNumber->unsigned16BitValue() != threadCount-1)
        return NULL;

    return this;
}

static IOPMPowerState powerStates[2] =
{
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, kIOPMPowerOn, kIOPMPowerOn, kIOPMPowerOn, 0, 0, 0, 0, 0, 0, 0, 0 }
};

IOReturn CpuTSCSync::setPowerState(unsigned long powerStateOrdinal, IOService *whatDevice )
{
    if (powerStateOrdinal)
        this->doTSC();

    return IOPMAckImplied;
}

void CpuTSCSync::stop(IOService *provider)
{
    PMstop();
    super::stop(provider);
}

bool CpuTSCSync::start(IOService *provider)
{
	if (!super::start(provider)) { return false; }

    // announce version
    extern kmod_info_t kmod_info;
    IOLog("CpuTSCSync: Version %s starting on OS X Darwin %d.%d.\n", kmod_info.version, version_major, version_minor);

    // place version/build info in ioreg properties RM,Build and RM,Version
    char buf[128];
    snprintf(buf, sizeof(buf), "%s", kmod_info.version);
    setProperty("build,Version:", buf);
#ifdef DEBUG
    setProperty("Build-Debug:", buf);
#else
    setProperty("Build-Release:", buf);
#endif

    PMinit();
    registerPowerDriver(this, powerStates, 2);
    provider->joinPMtree(this);
	return true;
}

// 更新全部处理器的MSR
void CpuTSCSync::doTSC()
{
	uint64_t tsc = rdtsc64();
    IOLog("CpuTSCSync: current tsc from rdtsc64() is %lld. Rendezvouing..\n", tsc);
	
	// 更新所有处理器TSC
	mp_rendezvous_no_intrs(stamp_tsc, &tsc);
}
