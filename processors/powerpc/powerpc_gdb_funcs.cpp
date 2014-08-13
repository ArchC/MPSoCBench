/**
 * @file      powerpc_gdb_funcs.cpp
 * @author    Bruno Corsi dos Santos
 *
 *            The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br
 *
 * @version   1.0
 * @date      Mon, 19 Jun 2006 15:50:48 -0300
 * 
 * @brief     The ArchC POWERPC functional model.
 * 
 * @attention Copyright (C) 2002-2006 --- The ArchC Team
 * 
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "powerpc.H"

using namespace powerpc_parms;

int powerpc::nRegs(void) {
  return 104;
}


ac_word powerpc::reg_read( int reg ) {
  unsigned int n;

  if ( ( reg >= 0 ) && ( reg < 32 ) )
    return GPR.read( reg );
  else {
    switch (reg) {
    
      case 96:
	n=ac_pc;
      break;

      /* case ??:
        n=MSR.read();
	break; */

      case 98:
        n=CR.read();
      break;

      case 99:
        n=LR.read();
      break;

      case 100:
        n=CTR.read();
      break;

      case 101:
        n=XER.read();
      break;

      default:
        return 0;
      break;

    }
    
    return(n);
    
  }
}


void powerpc::reg_write( int reg, ac_word value ) {

  /* general purpose registers */
  if ( ( reg >= 0 ) && ( reg < 32 ) )
    GPR.write(reg,value);
  else {
    switch (reg) {

    case 96:
      ac_pc=value;
    break;

    /*    case ??:
      MSR.write(value);
    break;
    */

    case 98:
      CR.write(value);
    break;

    case 99:
      LR.write(value);
    break;

    case 100:
      CTR.write(value);
    break;

    case 101:
      XER.write(value);
    break;

    default:
      /* No completely implemented register */
      break;

    }
  }
}


unsigned char powerpc::mem_read( unsigned int address ) {
  return IM->read_byte( address );
}


void powerpc::mem_write( unsigned int address, unsigned char byte ) {
  IM->write_byte( address, byte );
}

