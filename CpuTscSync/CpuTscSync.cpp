//
//  CpuTscSync.cpp
//  CpuTscSync
//
//  版权所有 © 2020 lvs1974。
//

#include <Headers/kern_api.hpp>

#include "CpuTscSync.hpp"
#include "TSCSync.hpp"

static CpuTscSyncPlugin *callbackCpuf = nullptr;

void CpuTscSyncPlugin::init()
{
	callbackCpuf = this;
	
	lilu.onPatcherLoadForce(
		[](void *user, KernelPatcher &patcher) {
			static_cast<CpuTscSyncPlugin *>(user)->processKernel(patcher);
		}, this);
}

void CpuTscSyncPlugin::xcpm_urgency(int urgency, uint64_t rt_period, uint64_t rt_deadline)
{
	if (!TSCSync::isTscSynced())
	{
		SYSLOG("cputs", "当 TSC 可能不同步时调用 xcpm_urgency，同步它");
        return;
	}
	
	FunctionCast(xcpm_urgency, callbackCpuf->org_xcpm_urgency)(urgency, rt_period, rt_deadline);
}

void CpuTscSyncPlugin::processKernel(KernelPatcher &patcher)
{
	if (!kernel_routed)
	{
		KernelPatcher::RouteRequest request {"_xcpm_urgency", xcpm_urgency, org_xcpm_urgency};
		if (!patcher.routeMultiple(KernelPatcher::KernelID, &request, 1))
			SYSLOG("cputs", "patcher.routeMultiple for %s is failed with error %d", request.symbol, patcher.getError());
		kernel_routed = true;
	}

	// 忽略其他处理器的所有错误
	patcher.clearError();
}
