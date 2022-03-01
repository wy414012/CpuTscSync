# CpuTscSync

- **该库仅供自维护机型使用，追求稳定，而不是探究新方法诞生**
- **非主分支模式**
- **克隆于1.03版本**
- **不对测试版系统进行深入的支持与探索。**
- **对构建包名进行了与团队版本的区分，修改为`as.Yaming.CpuTscSync`**
## 注意 ！！！
- 只想追求方法而不是稳定运行的这里不适合你
- 只是为了追求新方法的也不适合你
- 已有代码能解决的问题，这里绝对不会去写新的代码！！！
## 手动构建说明
- 克隆仓库`git clone https://github.com/wy414012/CpuTscSync.git`
- 进入代码库`cd CpuTscSync`
- `xcodebuild -jobs 1 -configuration Debug`构建测试版本
- `xcodebuild -jobs 1 -configuration Release`构建稳定版

## 注意x79系列使用
- 该系列cpu请使用库中驱动并且配合OC`UEFI-->Quirks-->TscSyncTimeout`使用，初始值`500000`上调每次值为10万增加，自行测试合适的值。
