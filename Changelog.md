CpuTscSync Changelog
===================
### v1.5.3
- 格式化代码缩进，使其更易于阅读和理解；
- 使用`nullptr`替代`NULL`，`nullptr`是`C++11`中引入的新关键字，作为新的空指针常量，好处是类型安全；
- 使用强制类型转换`static_cast`替代`reinterpret_cast`，`static_cast`在类型转换时进行类型检查，更加安全。
### v1.5.1
- 将`VoodooTSCSync驱动重构到该库`适配兼容性

### v1.0.6
- 开始采用团队版新方法用于同步10.7-11.6.x之间的TSC（用于评估验证）
- 由于e526xx系列cpu不存在adjus方法所以暂时屏蔽掉该方法

### v1.05
- 将包名正式更改
- 添加中文注释
- 规范文件名
- 测试更新xcode
### v1.04
- 分支重构解决之前版本的兼容问题
- 移除了`MSR_IA32_TSC_ADJUST` (03Bh)方法 因为大部分8代之前的主板不存在该区域
- 测试了在低于6核心12线程的CPU上 macOS12 工作良好


#### 原始v1.0.4
- Added constants for macOS 12 support
- Added macOS 12 compatibility for CPUs with `MSR_IA32_TSC_ADJUST` (03Bh)

#### v1.0.3
- Added MacKernelSDK with Xcode 12 compatibility

#### v1.0.2
- Compatibility for macOS 11
- Use atomic variable tsc_synced 

#### v1.0.1
- Minor improvements (logging, original xcpm_urgency is called after syncing)

#### v1.0.0
- Initial release
