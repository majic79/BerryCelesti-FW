// This file is part of BerryCelesti-FW.
//
// BerryCelesti-FW is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// BerryCelesti-FW is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// BerryCelesti-FW. If not, see <https://www.gnu.org/licenses/>.
//
// Copyright (C) 2022 Chris Marr - Nerdsmith UK (www.nerdsmith.co.uk)

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