/**
 * @file      ac_noc.cpp
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

#include "ac_noc_s.H"

void ac_noc_s::addAddress(FLITTYPE nocAddress, uint32_t startaddress,
                          uint32_t endaddress) {
  if (startaddress >= endaddress) {
    cout << "SEVERE: memory declared with start address greater or equal to "
            "end address." << endl;
    cout << "SEVERE: start=" << hex << startaddress << " end=" << hex
         << endaddress << endl;
    exit(1);
  }
  addressMap amap;
  amap.nocAddress = nocAddress;
  amap.startaddress = startaddress;
  amap.endaddress = endaddress;
  list<addressMap>::iterator it;
  for (it = addresses.begin(); it != addresses.end(); ++it) {
    if (((*it).endaddress >= startaddress) &&
        ((*it).startaddress <= endaddress)) {
      cout << "WARNING: overlapping address space!" << endl;
      cout << "WARNING: noc node address=" << hex << (*it).nocAddress << endl;
      cout << "WARNING: declared start=" << hex << startaddress
           << " end=" << hex << endaddress << endl;
      cout << "WARNING: current  start=" << hex << (*it).startaddress
           << " end=" << hex << (*it).endaddress << endl;
    }
  }
  addresses.push_back(amap);
}

ac_noc_s::ac_noc_s(sc_module_name _name, FLITTYPE _address, uint32_t _size)
    : sc_module(_name), address(_address), size(_size) {
  cout << ".ac_noc_s> instanciado - " << _name << endl;
  SC_THREAD(runIP);

} //,    target_export("target_port"),    slave_port("slave_port", 16777471U)

void ac_noc_s::sendFlit(FLITTYPE payload) {
  bool answer = false;
  while (!answer) {
    outPort->try2SendFlit(payload, &answer);
    // cout <<name()<<hex<<address<<" -> sent     0x" << hex << payload << endl;
    if (!answer)
      wait(1, SC_NS);
  }
}
// escravo
void ac_noc_s::sendAnswer2ReadPacket(FLITTYPE nocaddr, FLITTYPE data) {
  nocaddr.range(15, 8) = address(7, 0);
  sendFlit(nocaddr);
  sendFlit(1);
  sendFlit(data);
}

void ac_noc_s::sendReadPacket(FLITTYPE nocaddr, FLITTYPE memaddr) {
  nocaddr.range(15, 8) = address(7, 0);
  sendFlit(nocaddr);
  sendFlit(1);
  sendFlit(memaddr);
}

void ac_noc_s::sendWritePacket(FLITTYPE nocaddr, FLITTYPE memaddr,
                               FLITTYPE data) {
  nocaddr.range(15, 8) = address(7, 0);
  sendFlit(nocaddr);
  sendFlit(2);
  sendFlit(memaddr);
  sendFlit(data);
}
// escravo
FLITTYPE ac_noc_s::receiveFlitSlave() {
  FLITTYPE payload;
  wait(1, SC_NS);
  inPort->haveNewFlit(&payload);
  // cout <<name()<<hex<<address<<" <- received 0x" << hex << payload << endl;
  inPort->flitAccepted();
  return payload;
}

FLITTYPE ac_noc_s::receiveFlit() {
  FLITTYPE payload;
  // Receive header flit
  inPort->haveNewFlit(&payload);
  // cout <<name()<<hex<<address<<" <- received 0x" << hex << payload << endl;
  inPort->flitAccepted();
  return payload;
}

FLITTYPE ac_noc_s::receiveReadPacket() {
  FLITTYPE nocaddr, size, data;
  nocaddr = receiveFlit();
  size = receiveFlit();
  data = receiveFlit();
  while (size.to_uint() > 1) {
    cout << "WARNING: " << name()
         << " received a packet of size different than one." << endl;
    receiveFlit();
    size = size.to_uint() - 1;
  }
  return data;
}

ac_noc_s::addressMap ac_noc_s::decodeAddress(FLITTYPE memaddr) {
  FLITTYPE nocaddr;
  list<addressMap>::iterator it;
  for (it = addresses.begin(); it != addresses.end(); ++it) {
    if (((*it).startaddress <= memaddr) && ((*it).endaddress >= memaddr)) {
      return (*it);
    }
  }
  cout << "SEVERE: wrapper (" << name()
       << ") was requested to access a memory address (0x" << hex << memaddr
       << ") that is not mapped to any NoC address. Maybe forgot to add"
       << " an address mapping (with addAddress())?" << endl;
  for (it = addresses.begin(); it != addresses.end(); ++it) {
    cout << "NoC (0x" << hex << (*it).nocAddress << ") mapped to 0x" << hex
         << (*it).startaddress << "-0x" << hex << (*it).endaddress << endl;
  }

  exit(1);
}

/*ac_tlm_rsp ac_noc_s::transport( const ac_tlm_req &request ) {
    ac_tlm_rsp response;
    cout << "Transport do ac_noc_s" << endl;
    //cout << "Endereço do request: " << request.addr << endl;
    addressMap addr;
    FLITTYPE nocaddr, memaddr;

    addr = decodeAddress(request.addr);
    nocaddr = addr.nocAddress;
    memaddr = request.addr-addr.startaddress + 4; //alteração no endereço da
requisição para gravar no endereço correto
    response.status = SUCCESS;

    switch( request.type ) {
    case READ :
        sendReadPacket(nocaddr, memaddr);
        response.data = receiveReadPacket();
    break;
    case WRITE:
        sendWritePacket(nocaddr, memaddr, request.data);
    break;
    default :
        response.status = ERROR;
        break;
    }
    return response;
}*/
// escravo
void ac_noc_s::runIP() {

  sc_core::sc_time time_info;

  FLITTYPE nocaddr, memaddr, data;
  sc_uint<FLITSIZE> size;

  uint32_t ad2, da2;

  ac_tlm2_payload payload;

  unsigned char p[64];

  while (true) {

    nocaddr = receiveFlitSlave();
    memaddr = receiveFlitSlave();

    size = memaddr.to_uint();

    if (size > 0) {

      memaddr = receiveFlitSlave();
      ad2 = memaddr;

      if (size == 1) { // it is a READ operation

        payload.set_address(ad2);
        // req.addr = ad2;
        payload.set_command(tlm::TLM_READ_COMMAND);
        // req.type = READ;
        payload.set_data_length(sizeof(uint32_t));
        payload.set_data_ptr(p);
        slave_port->b_transport(payload, time_info);
        // rsp = slave_port->transport(req);
        da2 = *((uint32_t *)p);
        // da2 = rsp.data;

        // cout<<"Mem ("<<name()<<") read " <<ad2 << " =0x"<<hex<<da2<<endl;
        sendAnswer2ReadPacket(nocaddr >> 8, da2);

      } else if (size == 2) { // it is a WRITE operation

        inPort->haveNewFlit(&data);

        da2 = data.to_uint();

        inPort->flitAccepted();

        payload.set_data_length(sizeof(uint32_t));

        payload.set_address(ad2);
        // req.addr = ad2;
        payload.set_command(tlm::TLM_WRITE_COMMAND);
        // req.type = WRITE;

        (*(uint32_t *)p) = da2;
        // ((uint32_t*)p)[0]=da2;

        payload.set_data_ptr(p);

        // req.data = da2;

        // cout<<"Mem ("<<name()<<") write " << ad2 << " =0x"<<hex<<da2<<endl;
        slave_port->b_transport(payload, time_info);

      } else {
        while (size.to_uint() > 1) {
          inPort->haveNewFlit(&data);
          inPort->flitAccepted();
          size = size.to_uint() - 1;
        }
      }
    }
  }
}
