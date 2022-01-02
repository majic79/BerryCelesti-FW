#include "bootloader.h"
#include "Hardware.h"

// TODO: Bootloader for code... sample below from AN2634 documentation

static bool is_boot_requested(void)
{
    /* Check for boot request from firmware */
    if (USERROW.USERROW31 == 0xEB) {
        /* Clear boot request*/
        USERROW.USERROW31 = 0xff;
        _PROTECTED_WRITE_SPM(NVMCTRL.CTRLA, NVMCTRL_CMD_PAGEERASEWRITE_gc);
        while(NVMCTRL.STATUS & NVMCTRL_EEBUSY_bm);
    }
    /* Check if SW1 (PC5) is low */
    else if(VPORTC.IN & PIN5_bm) {
        return false;
    }
    
    return true;
}

void enter_bootloader(void)
{
    /* Write boot request */
    USERROW.USERROW31 = 0xEB;
    _PROTECTED_WRITE_SPM(NVMCTRL.CTRLA, NVMCTRL_CMD_PAGEERASEWRITE_gc);
    while(NVMCTRL.STATUS & NVMCTRL_EEBUSY_bm);
    /* Issue system reset */
    _PROTECTED_WRITE(RSTCTRL.SWRR, RSTCTRL_SWRE_bm);
}