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
      m_monitoringEnabled(false),
      m_exceedanceLatched(false) {}

PingReceiverComponentImpl ::~PingReceiverComponentImpl() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void PingReceiverComponentImpl ::PingIn_handler(const FwIndexType portNum, U32 key) {
    // this->log_DIAGNOSTIC_PR_PingReceived(key);
    const U32 reported = this->m_pingsRecvd;
    this->tlmWrite_PR_NumPings(reported);
    this->m_pingsRecvd++;
    if (this->m_monitoringEnabled) {
        if (reported > this->m_pingCeiling) {
            if (not this->m_exceedanceLatched) {
                this->log_WARNING_HI_PR_PingCeilingExceeded(reported, this->m_pingCeiling);
                this->m_exceedanceLatched = true;
            }
        } else {
            this->m_exceedanceLatched = false;
        }
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
    this->m_monitoringEnabled = true;
    this->m_exceedanceLatched = false;
    this->tlmWrite_PR_PingCeiling(ceiling);
    this->log_ACTIVITY_HI_PR_PingCeilingSet(ceiling);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // end namespace Ref
