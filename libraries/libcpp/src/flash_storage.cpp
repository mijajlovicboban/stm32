/*
 * Copyright (c) 2013, Michael E. Ferguson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <flash_storage.hpp>

namespace flash_storage
{

bool write(uint32_t addr, uint16_t * data, const uint16_t data_len)
{
  int sector;

  /* Check bounds */
  if ((addr >= (SECTOR11_START_ADDR + SECTOR_SIZE_128KB)) ||
      (addr < SECTOR0_START_ADDR))
    return false;

  /* Need to compute sector to erase */
  if (addr >= SECTOR5_START_ADDR)
    sector = 5 + ((addr - SECTOR5_START_ADDR)/SECTOR_SIZE_128KB);
  else if (addr >= SECTOR4_START_ADDR)
    sector = 4;
  else
    sector = (addr - SECTOR0_START_ADDR)/SECTOR_SIZE_16KB;

  /* Unlock flash */
  FLASH_Unlock();
  FLASH_EraseSector(sector * 0x08, VoltageRange_3);

  /* Write data */
  for (int i = 0; i < data_len; i += 2)
  {
    FLASH_ProgramHalfWord(addr, *data);
    data++; addr += 2; 
  }

  return true;
}

bool read(uint32_t addr, uint16_t * data, const uint16_t data_len)
{
  for (int i = 0; i < data_len; i += 2)
  {
    *data = *(volatile uint16_t*)(addr + i);
    data++;
  }
  return true;
}

}  // end namespace flash_storage