// ======================================================================
// \title  PingReceiverImpl.cpp
// \author tim
// \brief  cpp file for PingReceiver component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#include <Fw/FPrimeBasicTypes.hpp>
#include <Ref/PingReceiver/PingReceiverComponentImpl.hpp>

namespace Ref {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

PingReceiverComponentImpl ::PingReceiverComponentImpl(const char* const compName)
    : PingReceiverComponentBase(compName),
      m_inhibitPings(false),
      m_pingsRecvd(0),
      m_pingCeiling(0xFFFFFFFF),
      m_ceilingExceeded(false) {}

PingReceiverComponentImpl ::~PingReceiverComponentImpl() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void PingReceiverComponentImpl ::PingIn_handler(const FwIndexType portNum, U32 key) {
    // this->log_DIAGNOSTIC_PR_PingReceived(key);
    this->m_pingsRecvd++;
    this->tlmWrite_PR_NumPings(this->m_pingsRecvd);

    if ((this->m_pingsRecvd > this->m_pingCeiling) && (not this->m_ceilingExceeded)) {
        this->log_WARNING_HI_PR_PingsExceeded(this->m_pingsRecvd, this->m_pingCeiling);
        this->m_ceilingExceeded = true;
    }

    if (not this->m_inhibitPings) {
        PingOut_out(0, key);
    }
}

void PingReceiverComponentImpl::PR_StopPings_cmdHandler(FwOpcodeType opCode, /*!< The opcode*/
                                                        U32 cmdSeq           /*!< The command sequence number*/
) {
    this->m_inhibitPings = true;
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void PingReceiverComponentImpl::PR_SetPingCeiling_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, U32 ceiling) {
    this->m_pingCeiling = ceiling;
    this->tlmWrite_PR_PingCeiling(this->m_pingCeiling);
    if (this->m_pingsRecvd <= this->m_pingCeiling) {
        this->m_ceilingExceeded = false;
    }
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // end namespace Ref
