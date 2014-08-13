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

#include "ac_noc.H"

using tlm::tlm_command;
using tlm::TLM_READ_COMMAND;
using tlm::TLM_WRITE_COMMAND;


void ac_noc::addAddress(FLITTYPE nocAddress, uint32_t startaddress, uint32_t endaddress){
    if(startaddress>=endaddress){
        cout << "SEVERE: memory declared with start address greater or equal to end address." << endl;
        cout << "SEVERE: start=" << hex << startaddress <<" end="<<hex<<endaddress<<endl;
        exit(1);
    }
    addressMap amap;
    amap.nocAddress = nocAddress;
    amap.startaddress = startaddress;
    amap.endaddress = endaddress;
    list<addressMap>::iterator it;
    for(it=addresses.begin(); it != addresses.end(); ++it){
        if (((*it).endaddress >= startaddress) &&
            ((*it).startaddress <= endaddress)){
            cout << "WARNING: overlapping address space!" << endl;
            cout << "WARNING: noc node address="<<hex<<(*it).nocAddress<<endl;
            cout << "WARNING: declared start="<<hex<<startaddress<<" end="<<hex<<endaddress<<endl;
            cout << "WARNING: current  start="<<hex<<(*it).startaddress<<" end="<<hex<<(*it).endaddress<<endl;
        }

    }
    addresses.push_back(amap);

}

ac_noc::ac_noc(sc_module_name _name, FLITTYPE _address, uint32_t _mem_address): 
    sc_module(_name), 
    address(_address),
    mem_address(_mem_address),
    target_export("iport") 
{
    target_export(*this);

    cout << "...ac_noc> instanciado - "<< _name << endl;
    
    //SC_THREAD(doRead);
    //sensitive << doReadEvt;
    //SC_THREAD(doWrite);
    //sensitive << doWriteEvt;
}

void ac_noc::sendFlit(FLITTYPE payload){

    //printf("\t ac_noc.cpp - sendFlit \n");

	bool answer=false;
     
     while(!answer){
         outPort->try2SendFlit(payload, &answer);
         if (!answer) wait(1,SC_NS);
     }
}

void ac_noc::sendReadPacket(FLITTYPE nocaddr, FLITTYPE memaddr){
    
    //printf("\t ac_noc.cpp - sendReadPacket \n");

    nocaddr.range(15,8) = address(7,0);
    sendFlit(nocaddr);
    sendFlit(1);
    sendFlit(memaddr);
}

void ac_noc::sendWritePacket(FLITTYPE nocaddr, FLITTYPE memaddr, FLITTYPE data){
    
    //printf("\t ac_noc.cpp - sendWritePacket \n");

    nocaddr.range(15,8) = address(7,0);
    sendFlit(nocaddr);
    sendFlit(2);
    sendFlit(memaddr);
    sendFlit(data);
}

FLITTYPE ac_noc::receiveFlit() {
    
    //printf("\t ac_noc.cpp - receiveFlit \n");

    FLITTYPE payload;
    //Receive header flit
    inPort->haveNewFlit(&payload);
    inPort->flitAccepted();
    return payload;
}

FLITTYPE ac_noc::receiveReadPacket(){
    
    //printf("\t ac_noc.cpp - receiveReadPacket \n");

    FLITTYPE nocaddr, size, data;
    nocaddr = receiveFlit();
    size    = receiveFlit();
    data    = receiveFlit();
    while(size.to_uint()>1){
        cout<<"WARNING: "<<name()<<" received a packet of size different than one."<<endl;
        receiveFlit();
        size = size.to_uint()-1;
    }
    return data;
}

ac_noc::addressMap ac_noc::decodeAddress(FLITTYPE memaddr){
    FLITTYPE nocaddr;
    list<addressMap>::iterator it;
    for(it=addresses.begin(); it != addresses.end(); ++it){
        if (((*it).startaddress <= memaddr) &&
            ((*it).endaddress >= memaddr)){
            return (*it);
        }            
    }
    cout<<"SEVERE: wrapper ("<<name()<<") was requested to access a memory address (0x"\
        <<hex<<memaddr<<") that is not mapped to any NoC address. Maybe forgot to add"\
        <<" an address mapping (with addAddress())?"<<endl;
    for(it=addresses.begin(); it != addresses.end(); ++it){
        cout<<"NoC (0x"<<hex<<(*it).nocAddress<<") mapped to 0x"\
            <<hex<<(*it).startaddress<<"-0x"<<hex<<(*it).endaddress<<endl;
    }
    
    exit(1);
}


//void ac_noc::testRun(){
    // list<addressMap>::iterator it;
    // cout<<"Dump of address map:"<<endl;
    // for(it=addresses.begin(); it != addresses.end(); ++it){
    //     cout<<"0x"<<hex<<(*it).startaddress<<"-0x"<<hex<<(*it).endaddress\
    //         <<"=Noc(0x"<<hex<<(*it).nocAddress<<endl;
    // }
//     FLITTYPE nocaddr, memaddr, data, check;
//     addressMap addr;
//     data    = 0xABCDEF98;
//     for(memaddr = 0x00000000; memaddr<=0x000002FF; memaddr++){        
//         data = rand();
//         cout<<"cycle "<<hex<<memaddr<<endl;
//         cout<<"write"<<endl;
//         sendWritePacket(nocaddr, memaddr-addr.startaddress, data);       
//         cout<<"readreq"<<endl;
//         cout<<"check"<<endl;
//         if(check!=data){
//             cout << "DATA DIFFER! Check: "<<hex<<check<<" Data: "<<hex<<data<<endl;
//         } else {
//             cout << "checked "<<hex<<data<<end0x30000000,0x7FFFFFFFl;
//         }
//     }
//     cout<<"Test end."<<endl;
// }



void ac_noc::b_transport(ac_tlm2_payload &payload, sc_core::sc_time &time_info)
{

    FLITTYPE nocaddr, memaddr;
	uint32_t addr_m = (uint32_t) payload.get_address();

    addressMap addr = decodeAddress(addr_m);

	nocaddr = addr.nocAddress;      
	memaddr = addr_m-addr.startaddress;

	unsigned char* data_pointer = payload.get_data_ptr();

	unsigned int len = payload.get_data_length();
	tlm_command command = payload.get_command();

    //cout << "comando: " << command << "len: " << len << endl;

	payload.set_response_status(tlm::TLM_OK_RESPONSE);    
   
	uint8_t data8;
	uint16_t data16;
	uint32_t data32;

	switch(command) {

		case TLM_READ_COMMAND :     // Packet is a READ one

			sendReadPacket(nocaddr, memaddr);
			//response.data = receiveReadPacket();

			 *((uint32_t*)data_pointer) = (uint32_t) receiveReadPacket();

			

		break; 

		case TLM_WRITE_COMMAND:     // Packet is a WRITE
	        	
			sendWritePacket(nocaddr, memaddr, (*(uint32_t*)data_pointer));       
		break;

		default:
			printf("AC_NOC TRANSPORT --> COMMAND %d IS NOT IMPLEMENTED", command);
      		break; 

	}

}


/*ac_tlm_rsp ac_noc::transport( const ac_tlm_req &request ) {


    ac_tlm_rsp response;


    addressMap addr;
    FLITTYPE nocaddr, memaddr;
    addr = decodeAddress(request.addr);

    nocaddr = addr.nocAddress;

    memaddr = request.addr-addr.startaddress;

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

