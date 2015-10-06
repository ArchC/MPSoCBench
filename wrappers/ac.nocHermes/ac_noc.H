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

#ifndef __ac_noc_H
#define __ac_noc_H

#include <list>
#include <systemc.h>
// ArchC TLM
#include "ac_tlm_protocol.H"
// Hermes NoC    // adicionar ../../is/nocHermes/ para gerar dicionario
#include "chl_and_if/outFromRouterIf.h"
#include "chl_and_if/intoRouterIf.h"
#include "basics/doorDefs.h"
#include "../../defines.h"
#include "ac_tlm2_port.H"

// using statements
using namespace std;
using tlm::tlm_blocking_transport_if;

class ac_noc: 
    public sc_module,
    public ac_tlm2_blocking_transport_if 
{
public:
    // ac_tlm_protocol port
    sc_export< ac_tlm2_blocking_transport_if > target_export;
    // Hermes NoC ports
    sc_port< outFromRouterIf > outPort;
    sc_port< intoRouterIf > inPort;

    SC_HAS_PROCESS(ac_noc);

    ac_noc(sc_module_name _name, FLITTYPE _address, uint32_t _mem_address);

    void addAddress(FLITTYPE nocAddress, uint32_t start_address, uint32_t endaddress);

    //void doRead();
    //void doWrite();

    // ac_tlm_rsp_status writem( const uint32_t & , const uint32_t & );
    // ac_tlm_rsp_status readm( const uint32_t & , uint32_t & );
    

    //ac_tlm_rsp transport( const ac_tlm_req &request );
    void b_transport(ac_tlm2_payload &, sc_core::sc_time &);

    class addressMap{
    public:
        FLITTYPE nocAddress;
        uint32_t startaddress;
        uint32_t endaddress;
    };

private:
    sc_event doReadEvt, doWriteEvt;
    bool ready;
    FLITTYPE address, targetAddress, targetData;
    uint32_t mem_address;
    list<addressMap> addresses;    

    void sendFlit(FLITTYPE payload);
    void sendReadPacket(FLITTYPE nocaddr, FLITTYPE memaddr);
    void sendWritePacket(FLITTYPE nocaddr, FLITTYPE memaddr, FLITTYPE data);
    FLITTYPE receiveFlit();
    FLITTYPE receiveReadPacket();
    addressMap decodeAddress(FLITTYPE memaddr);


}; // end module ac_noc

#endif
