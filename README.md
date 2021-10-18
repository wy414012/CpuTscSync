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
- 克隆SDK`git clone https://github.com/wy414012/MacKernelSDK.git`
- 放入Lilu_Debug_版本
- `xcodebuild -jobs 1 -configuration Debug`构建测试版本
- `xcodebuild -jobs 1 -configuration Release`构建稳定版
## warn! ! !
- **I just want to pursue a method instead of stable operation. This is not for you.**
- **Just for pursuing new methods is not suitable for you.**
- **Problems that can be solved with existing code. Never write new code.! ! !**

#### Credits
- [Apple](https://www.apple.com) for macOS  
- [vit9696](https://github.com/vit9696) for [Lilu.kext](https://github.com/vit9696/Lilu)
- [Voodoo Projects Team](http://forge.voodooprojects.org/p/voodootscsync/) for initial idea and implementation
- [RehabMan](https://github.com/RehabMan/VoodooTSCSync) for improved implementation
- [lvs1974](https://applelife.ru/members/lvs1974.53809/) for writing the software and maintaining it
