#ifndef CHI_EXTENSION_H
#define CHI_EXTENSION_H

#include <systemc>
#include <tlm>
namespace little_end {

struct chi_extension:public tlm::tlm_extension<chi_extension>
{
    chi_extension(){};

    virtual tlm_extension_base* clone() const
    {
        chi_extension *ext = new chi_extension;
        ext->srcAddr = srcAddr;
        ext->dstAddr = dstAddr;
        return ext;
    }

    virtual void copy_from(tlm_extension_base const &ext)
    {
        srcAddr = static_cast<chi_extension const &>(ext).srcAddr;
        dstAddr = static_cast<chi_extension const &>(ext).dstAddr;
    }
    
    int srcAddr;
    int dstAddr;
};

}
#endif