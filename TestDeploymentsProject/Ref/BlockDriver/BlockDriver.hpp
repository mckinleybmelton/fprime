#ifndef REF_BLOCK_DRIVER_IMPL_HPP
#define REF_BLOCK_DRIVER_IMPL_HPP

#include <Ref/BlockDriver/BlockDriverComponentAc.hpp>

#include <atomic>

namespace Ref {

class BlockDriver final : public BlockDriverComponentBase {
  public:
    // Only called by derived class
    BlockDriver(const char* compName);

    ~BlockDriver();

  private:
    // downcalls for input ports
    void BufferIn_handler(FwIndexType portNum, Drv::DataBuffer& buffer) override;
    void Sched_handler(FwIndexType portNum, U32 context) override;
    void SchedInternal_internalInterfaceHandler() override;
    void SchedInternal_overflowHook() override;
    //! Handler implementation for PingIn
    //!
    void PingIn_handler(const FwIndexType portNum, /*!< The port number*/
                        U32 key                    /*!< Value to return to pinger*/
                        ) override;

    //! Set cycle ceiling for BD_Cycles (REQ-BD-001)
    void BD_SetCycleCeiling_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, U32 ceiling) override;

    // cycle count
    U32 m_cycles;
    U32 m_cycleCeiling;        //!< cycle ceiling for BD_Cycles (REQ-BD-001/002)
    bool m_cycleCeilingArmed;  //!< true after BD_SetCycleCeiling (REQ-BD-003)
    // true while a SchedInternal message is on the queue
    std::atomic<bool> m_schedQueued;
};
}  // namespace Ref

#endif
