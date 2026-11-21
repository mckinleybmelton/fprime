#include <Fw/FPrimeBasicTypes.hpp>
#include <Fw/Types/Assert.hpp>
#include <Ref/BlockDriver/BlockDriver.hpp>

namespace Ref {

BlockDriver::BlockDriver(const char* compName)
    : BlockDriverComponentBase(compName),
      m_cycles(0),
      m_cycleCeiling(0),
      m_cycleCeilingArmed(false),
      m_schedQueued(false) {}

BlockDriver::~BlockDriver() {}

void BlockDriver::BufferIn_handler(FwIndexType portNum, Drv::DataBuffer& buffer) {
    // just a pass-through
    this->BufferOut_out(0, buffer);
}

void BlockDriver::Sched_handler(FwIndexType portNum, U32 context) {
    bool expected = false;
    if (this->m_schedQueued.compare_exchange_strong(expected, true)) {
        this->SchedInternal_internalInterfaceInvoke();
    }
}

void BlockDriver::SchedInternal_overflowHook() {
    this->m_schedQueued = false;
}

void BlockDriver::SchedInternal_internalInterfaceHandler() {
    this->m_schedQueued = false;
    const U32 cycles = this->m_cycles++;
    this->tlmWrite_BD_Cycles(cycles);

    // REQ-BD-002: report active ceiling while armed
    if (this->m_cycleCeilingArmed) {
        this->tlmWrite_BD_CycleCeiling(this->m_cycleCeiling);
    }

    // REQ-BD-003: warn when BD_Cycles exceeds the armed cycle ceiling
    if (this->m_cycleCeilingArmed && (cycles > this->m_cycleCeiling)) {
        this->log_WARNING_HI_BD_CycleCeilingExceeded(cycles, this->m_cycleCeiling);
    }
}

void BlockDriver::PingIn_handler(const FwIndexType portNum, U32 key) {
    // call ping output port
    this->PingOut_out(0, key);
}

void BlockDriver::BD_SetCycleCeiling_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, U32 ceiling) {
    // REQ-BD-001: store and arm the operator-configured cycle ceiling
    this->m_cycleCeiling = ceiling;
    this->m_cycleCeilingArmed = true;
    this->tlmWrite_BD_CycleCeiling(this->m_cycleCeiling);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // namespace Ref
