/*  用这个做你想做的。
  这项工作源于 Turbo 和
  戴尔所有者cosmo1t 。
 *
 */

#include <IOKit/IOService.h>
#include <IOKit/pwr_mgt/IOPMPowerSource.h>

class TSCSync : public IOService
{
    typedef IOService super;
    OSDeclareDefaultStructors(TSCSync)

    /**
     *  Power state name indexes
     */
    enum PowerState {
        PowerStateOff,
        PowerStateOn,
        PowerStateMax
    };

    /**
     *  Power states we monitor
     */
    IOPMPowerState powerStates[PowerStateMax]  {
        {kIOPMPowerStateVersion1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {kIOPMPowerStateVersion1, kIOPMPowerOn | kIOPMDeviceUsable, kIOPMPowerOn, kIOPMPowerOn, 0, 0, 0, 0, 0, 0, 0, 0}
    };

public:
    /**
     *  Decide on whether to load or not by checking the processor compatibility.
     *
     *  @param provider  parent IOService object
     *  @param score     probing score
     *
     *  @return self if we could load anyhow
     */
    virtual IOService* probe(IOService* provider, SInt32* score) override;
    
    /**
     *  Add VirtualSMC listening notification.
     *
     *  @param provider  parent IOService object
     *
     *  @return true on success
     */
    bool start(IOService *provider) override;
    
    /**
     *  Update power state with the new one, here we catch sleep/wake/boot/shutdown calls
     *  New power state could be the reason for keystore to be saved to NVRAM, for example
     *
     *  @param state      power state index (must be below PowerStateMax)
     *  @param whatDevice power state device
     */
    IOReturn setPowerState(unsigned long state, IOService *whatDevice) override;
};
