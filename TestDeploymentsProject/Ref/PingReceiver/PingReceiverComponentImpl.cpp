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
#include <limits>

namespace Ref {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

PingReceiverComponentImpl ::PingReceiverComponentImpl(const char* const compName)
    : PingReceiverComponentBase(compName),
      m_inhibitPings(false),
      m_pingsRecvd(0),
      m_pingCeiling(std::numeric_limits<U32>::max()),
      m_ceilingExceeded(false) {}

PingReceiverComponentImpl ::~PingReceiverComponentImpl() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void PingReceiverComponentImpl ::PingIn_handler(const FwIndexType portNum, U32 key) {
    // Preserve existing PR_NumPings post-increment publish semantics.
    const U32 numPings = this->m_pingsRecvd;
    this->tlmWrite_PR_NumPings(this->m_pingsRecvd++);
    this->checkPingCeiling(numPings);
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

    // Last published PR_NumPings value (pre-increment write in PingIn_handler).
    const U32 currentNumPings = (this->m_pingsRecvd > 0) ? (this->m_pingsRecvd - 1) : 0;
    this->m_ceilingExceeded = false;
    this->checkPingCeiling(currentNumPings);

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void PingReceiverComponentImpl::checkPingCeiling(U32 numPings) {
    if ((numPings > this->m_pingCeiling) && (not this->m_ceilingExceeded)) {
        this->log_WARNING_HI_PR_PingCeilingExceeded(numPings, this->m_pingCeiling);
        this->m_ceilingExceeded = true;
    }
}

}  // end namespace Ref
