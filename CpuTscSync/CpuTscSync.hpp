//
//  CpuTscSync.hpp
//  CpuTscSync
//
//  版权所有 © 2020 lvs1974。 版权所有。
//

#ifndef kern_cputs_hpp
#define kern_cputs_hpp

#include <Headers/kern_patcher.hpp>
#include <IOKit/IOService.h>

class CpuTscSyncPlugin {
public:
	void init();
	
	bool kernel_routed = false;

private:
	/**
	 *  原始资源加载回调的蹦床
	 */
	mach_vm_address_t org_xcpm_urgency             {0};
	
	/**
	 *  挂钩函数
	 */
	static void xcpm_urgency(int urgency, uint64_t rt_period, uint64_t rt_deadline);
	
	/**
	 *  内核补丁
	 *
	 *  @参数 patcher KernelPatcher 实例
	 */
	void processKernel(KernelPatcher &patcher);
};

#endif /* kern_cputs_hpp */
