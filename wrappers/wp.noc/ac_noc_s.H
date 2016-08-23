/**
 * @file      ac_noc.H
 * @author    balbertini
 * @author    The ArchC Team
 *            http://www.archc.org/
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 * @date      Wed, 03 Aug 2011 14:33:42 -0300
 * @version   0.1
 * @brief     A wrapper between ac_tlm and Hermes NoC.
 *
 */

#ifndef __ac_noc_dma_H
#define __ac_noc_dma_H

#include <list>
#include <systemc.h>
#include "ac_tlm_protocol.H"
#include "ac_tlm2_port.H"
#include "chl_and_if/outFromRouterIf.h"
#include "chl_and_if/intoRouterIf.h"
#include "basics/doorDefs.h"

// using statements
using namespace std;

// using tlm::tlm_blocking_transport_if;

class ac_noc_s : public sc_module {
public:
  //    sc_export< ac_tlm_transport_if > target_export;
  sc_export<ac_tlm2_blocking_transport_if> slave_port;

  // Hermes NoC ports
  sc_port<outFromRouterIf> outPort;
  sc_port<intoRouterIf> inPort;

  SC_HAS_PROCESS(ac_noc_s);

  ac_noc_s(sc_module_name _name, FLITTYPE _address, uint32_t _size);

  void addAddress(FLITTYPE nocAddress, uint32_t start_address,
                  uint32_t endaddress);

  void runIP();
  // ac_tlm_rsp transport( const ac_tlm_req &request );

private:
  sc_event doReadEvt, doWriteEvt;
  bool ready;
  FLITTYPE address, targetAddress, targetData;
  class addressMap {
  public:
    FLITTYPE nocAddress;
    uint32_t startaddress;
    uint32_t endaddress;
  };
  list<addressMap> addresses;

  void sendFlit(FLITTYPE payload);
  void sendReadPacket(FLITTYPE nocaddr, FLITTYPE memaddr);
  void sendWritePacket(FLITTYPE nocaddr, FLITTYPE memaddr, FLITTYPE data);
  FLITTYPE receiveFlit();
  FLITTYPE receiveReadPacket();
  addressMap decodeAddress(FLITTYPE memaddr);

  // slave
  void sendAnswer2ReadPacket(FLITTYPE nocaddr, FLITTYPE data);
  FLITTYPE receiveFlitSlave();

  uint32_t size;
};

#endif
