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

    //! Handler implementation for command BD_SET_CYCLE_CEILING
    void BD_SET_CYCLE_CEILING_cmdHandler(FwOpcodeType opCode, /*!< The opcode*/
                                         U32 cmdSeq,          /*!< The command sequence number*/
                                         U32 ceiling          /*!< Cycle count ceiling*/
                                         ) override;

    // cycle count
    U32 m_cycles;
    // configured BD_Cycles ceiling
    U32 m_cycleCeiling;
    // true after BD_Cycles has exceeded the ceiling until re-armed
    bool m_ceilingExceeded;
    // true while a SchedInternal message is on the queue
    std::atomic<bool> m_schedQueued;
};
}  // namespace Ref

#endif
