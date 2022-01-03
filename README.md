BerryCelesti-FW is the Firmware component (software driving hardware) of a
specific Hat for a Raspberry Pi - more details on Pi HAT's can be found on
the Raspberry Pi website

BerryCelesti-FW is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later 
version.

BerryCelesti-FW is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along with
BerryCelesti-FW. If not, see <https://www.gnu.org/licenses/>.

Copyright (C) 2022 Chris Marr - Nerdsmith UK (www.nerdsmith.co.uk)

## Notes

For UART (debug output):

  * Disable bluetooth

https://www.mups.co.uk/post/2016/10/telescope-serial-control-and-pi-hat/

  * Raspi-config:
    * Disable serial port shell
    * Disable bluetooth module (how?)

  * systemctl disable hciuart

