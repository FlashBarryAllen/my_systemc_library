#ifndef INITIATOR_H
#define INITIATOR_H

#include "utilities.h"
#include "tlm_utils/simple_initiator_socket.h"

struct Initiator: sc_module
{
    tlm_utils::simple_initiator_socket<Initiator> socket;

    SC_CTOR(Initiator) : socket("socket") {
      socket.register_nb_transport_bw(this, &Initiator::nb_transport_bw);

      SC_METHOD(request);
    }

    void request()
    {
        tlm::tlm_generic_payload* trans;
        tlm::tlm_phase phase;
        sc_time delay;

        next_trigger(1, SC_NS);

        // Generate a sequence of random transactions
        //for (int i = 0; i < 1000000; i++) {
        int i = 0;
        {
            int adr = rand();
            tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);
            if (cmd == tlm::TLM_WRITE_COMMAND) {
                data[i % 16] = rand() % 10;
            }

            little_end::chi_extension* extension_pointer;
            extension_pointer = new little_end::chi_extension();
            extension_pointer->srcAddr = 0x00 + m_val;
            extension_pointer->dstAddr = 0xFF + m_val;

            // Grab a new transaction from the memory manager
            trans = m_mm.allocate();
            trans->acquire();

            // Set all attributes except byte_enable_length and extensions (unused)
            trans->set_command( cmd );
            trans->set_address( adr );
            trans->set_data_ptr( reinterpret_cast<unsigned char*>(&data[i % 16]) );
            trans->set_data_length( 4 );
            trans->set_streaming_width( 4 ); // = data_length to indicate no streaming
            trans->set_byte_enable_ptr( 0 ); // 0 indicates unused
            trans->set_dmi_allowed( false ); // Mandatory initial value
            trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value
            trans->set_extension(extension_pointer);

            phase = tlm::BEGIN_REQ;

            // Timing annotation models processing time of initiator prior to call
            delay = sc_time(rand_ps(), SC_PS);

            fout << hex << " " << name() << " snd, cmd=" << (cmd ? 'W' : 'R') << ", adr = " << adr 
                << ", data=" << data[i % 16] << " at time " << sc_time_stamp() << std::endl;

            // Non-blocking transport call on the forward path
            tlm::tlm_sync_enum status;
            status = socket->nb_transport_fw( *trans, phase, delay );
        }
        m_val++;
    }

    virtual tlm::tlm_sync_enum nb_transport_bw( tlm::tlm_generic_payload& trans,
                                                tlm::tlm_phase& phase, sc_time& delay ) {
        tlm::tlm_command cmd = trans.get_command();
        int              adr = trans.get_address();
        int*             ptr = reinterpret_cast<int*>( trans.get_data_ptr() );
        
        //little_end::chi_extension* extension_pointer = nullptr;
        //trans.get_extension(extension_pointer);
        //cout << extension_pointer->srcAddr << ", " << extension_pointer->dstAddr << endl;
        //delete extension_pointer;

        fout << hex <<  " " << name() << " rvc, cmd=" << (cmd ? 'W' : 'R') << ", adr = " << adr 
            << ", data=" << *ptr << " at time " << sc_time_stamp() << std::endl;
        
        trans.release();
        return tlm::TLM_COMPLETED;
    }

    mm   m_mm;
    int  data[16];
    int  m_val;
};

#endif
