#include <libkern/version.h>
#include "CpuTSCSync.h"

OSDefineMetaClassAndStructors(CpuTSCSync, IOService)

// Stamp the TSC
extern "C"
void stamp_tsc(void* tscp)
{
    wrmsr64(MSR_IA32_TSC, *static_cast<uint64_t*>(tscp));
}

// Define power states
static IOPMPowerState powerStates[2] =
{
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, kIOPMPowerOn, kIOPMPowerOn, kIOPMPowerOn, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// Probe method
IOService* CpuTSCSync::probe(IOService* provider, SInt32* score)
{
    if (!super::probe(provider, score)) {
        return nullptr;
    }

    if (!provider) {
        return nullptr;
    }

    // Only attach to last CPU
    uint16_t threadCount = rdmsr64(MSR_CORE_THREAD_COUNT);
    OSNumber* cpuNumber = OSDynamicCast(OSNumber, provider->getProperty("IOCPUNumber"));
    if (!cpuNumber || cpuNumber->unsigned16BitValue() != threadCount - 1) {
        return nullptr;
    }

    return this;
}

// Set power state
IOReturn CpuTSCSync::setPowerState(unsigned long powerStateOrdinal, IOService* whatDevice)
{
    if (powerStateOrdinal) {
        this->doTSC();
    }

    return IOPMAckImplied;
}

// Stop method
void CpuTSCSync::stop(IOService* provider)
{
    PMstop();
    super::stop(provider);
}

// Start method
bool CpuTSCSync::start(IOService* provider)
{
    if (!super::start(provider)) {
        return false;
    }

    // Announce version
    extern kmod_info_t kmod_info;
    IOLog("CpuTSCSync: Version %s starting on OSX Darwin %d.%d.\n", kmod_info.version, version_major, version_minor);

    // Place version/build info in IO registry properties
    char buf[128];
    snprintf(buf, sizeof(buf), "%s", kmod_info.version);
    setProperty("build, Version:", buf);

#ifdef DEBUG
    setProperty("Build-Debug:", buf);
#else
    setProperty("Build-Release:", buf);
#endif

    // Initialize power management
    PMinit();
    registerPowerDriver(this, powerStates, 2);
    provider->joinPMtree(this);

    return true;
}

// Update TSC for all processors
void CpuTSCSync::doTSC()
{
    uint64_t tsc = rdtsc64();
    IOLog("CpuTSCSync: current TSC from rdtsc64() is %lld. Rendezvouing..\n", tsc);

    mp_rendezvous_no_intrs(stamp_tsc, &tsc);
}
