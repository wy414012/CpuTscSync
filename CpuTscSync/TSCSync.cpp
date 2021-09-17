#include <Headers/kern_util.hpp>
#include "TSCSync.hpp"

OSDefineMetaClassAndStructors(TSCSync, IOService)

_Atomic(bool) TSCSync::tsc_synced = false;

//签名 tsc
extern "C" void stamp_tsc(void *tscp)
{
	wrmsr64(MSR_IA32_TSC, *reinterpret_cast<uint64_t*>(tscp));
}

IOService* TSCSync::probe(IOService* provider, SInt32* score)
{
    if (!super::probe(provider, score)) return NULL;
    if (!provider) return NULL;

    // 只连接到最后一个 CPU
    uint16_t threadCount = rdmsr64(MSR_CORE_THREAD_COUNT);
    OSNumber* cpuNumber = OSDynamicCast(OSNumber, provider->getProperty("IOCPUNumber"));
    if (!cpuNumber || cpuNumber->unsigned16BitValue() != threadCount-1)
        return NULL;

    return this;/*执行*/
}

static IOPMPowerState powerStates[2] =
{
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, kIOPMPowerOn, kIOPMPowerOn, kIOPMPowerOn, 0, 0, 0, 0, 0, 0, 0, 0 }
};

IOReturn TSCSync::setPowerState(unsigned long powerStateOrdinal, IOService *whatDevice )
{
	tsc_synced = false;

    if (powerStateOrdinal)
        doTSC();
	
    return IOPMAckImplied;
}

void TSCSync::stop(IOService *provider)
{
    PMstop();
    super::stop(provider);
}

bool TSCSync::start(IOService *provider)
{
	if (!super::start(provider)) { return false; }

    // 声明版本
    extern kmod_info_t kmod_info;
    SYSLOG("cputs", "Version %s starting on OS X Darwin %d.%d.\n", kmod_info.version, version_major, version_minor);

    // 将版本/构建信息放在 ioreg 属性 Build 和 Version
    char buf[128];
    snprintf(buf, sizeof(buf), "%s %s", kmod_info.name, kmod_info.version);
    setProperty("Version", buf);
#ifdef DEBUG
    setProperty("Build", "Debug");
#else
    setProperty("Build", "Release");
#endif

    PMinit();
    registerPowerDriver(this, powerStates, 2);
    provider->joinPMtree(this);
	return true;
}

// 更新所有处理器的 MSR.
void TSCSync::doTSC()
{
	uint64_t tsc = rdtsc64();
	SYSLOG("cputs", "current tsc from rdtsc64() is %lld. Rendezvouing..\n", tsc);
	
	// 调用将在所有内核/处理器上调用此“操作”的内核函数
	mp_rendezvous_no_intrs(stamp_tsc, &tsc);
	tsc_synced = true;
}
