#ifndef TARGET_H
#define TARGET_H

#include "utilities.h"
#include "tlm_utils/simple_target_socket.h"
#include "peq.h"

struct Target: sc_module
{
    tlm_utils::simple_target_socket<Target> socket;
    little_end::peq<tlm::tlm_generic_payload> m_peq;
    sc_in_clk m_clk;

    SC_CTOR(Target) : socket("socket"), m_peq("peq")
    {
        socket.register_nb_transport_fw(this, &Target::nb_transport_fw);

        SC_METHOD(response);
        sensitive << m_clk.pos();
    }

    void response() {
        tlm::tlm_generic_payload* payload = nullptr;
        sc_time delay = SC_ZERO_TIME;
        tlm::tlm_phase phase = tlm::BEGIN_REQ;

        while ((payload = m_peq.get_next_transaction()) != nullptr) {
            tlm::tlm_command cmd = payload->get_command();
            int              adr = payload->get_address();
            unsigned int     len = payload->get_data_length();
            unsigned char*   byt = payload->get_byte_enable_ptr();
            unsigned int     wid = payload->get_streaming_width();
            int*             dat = reinterpret_cast<int*>( payload->get_data_ptr() );

            little_end::chi_extension * extension_pointer = nullptr;
            payload->get_extension(extension_pointer);

            cout << extension_pointer->srcAddr << ", " << extension_pointer->dstAddr << endl;

            extension_pointer->srcAddr++;
            extension_pointer->dstAddr++;

            delete extension_pointer;

            fout << hex << " " << name() << " snd, cmd=" << (cmd ? 'W' : 'R') << ", adr = " << adr 
            << ", data=" << *dat << " at time " << sc_time_stamp() << std::endl;

            socket->nb_transport_bw( *payload, phase, delay );
        }
    }

    virtual tlm::tlm_sync_enum nb_transport_fw( tlm::tlm_generic_payload& trans,
                                                tlm::tlm_phase& phase, sc_time& delay ) {
        tlm::tlm_command cmd = trans.get_command();
        int              adr = trans.get_address();
        unsigned int     len = trans.get_data_length();
        unsigned char*   byt = trans.get_byte_enable_ptr();
        unsigned int     wid = trans.get_streaming_width();
        int*             dat = reinterpret_cast<int*>( trans.get_data_ptr() );
        int              cnt = 1 + rand() % 1;

        fout << hex << " " << name() << " rcv, cmd=" << (cmd ? 'W' : 'R') << ", adr = " << adr 
           << ", data=" << *dat << " at time " << sc_time_stamp() 
           << ". It needs to delay " << cnt << " periods." << std::endl;

        m_peq.delay(trans, cnt);

        return tlm::TLM_COMPLETED;
    }
  };

#endif
