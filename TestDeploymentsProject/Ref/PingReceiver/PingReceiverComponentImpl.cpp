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
      m_pingCeiling(0),
      m_ceilingExceeded(false) {}

PingReceiverComponentImpl ::~PingReceiverComponentImpl() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void PingReceiverComponentImpl ::PingIn_handler(const FwIndexType portNum, U32 key) {
    // this->log_DIAGNOSTIC_PR_PingReceived(key);
    const U32 pings = this->m_pingsRecvd++;
    this->tlmWrite_PR_NumPings(pings);
    if ((pings > this->m_pingCeiling) && !this->m_ceilingExceeded) {
        this->m_ceilingExceeded = true;
        this->log_WARNING_HI_PR_PingCeilingExceeded(pings, this->m_pingCeiling);
    }
    if (not this->m_inhibitPings) {
        this->PingOut_out(0, key);
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
    this->tlmWrite_PR_PingCeiling(ceiling);
    const bool reported = (this->m_pingsRecvd > 0);
    if (!reported || ((this->m_pingsRecvd - 1) <= ceiling)) {
        this->m_ceilingExceeded = false;
    }
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // end namespace Ref
