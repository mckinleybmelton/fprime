module Ref {

  @ An example block driver component with data buffers and interrupts
  active component BlockDriver {

    # ----------------------------------------------------------------------
    # General ports
    # ----------------------------------------------------------------------

    import Drv.Tick

    @ The rate group scheduler input
    sync input port Sched: Svc.Sched

    @ Internal port that performs the Sched work on the component thread
    internal port SchedInternal hook

    @ The input data buffer port
    async input port BufferIn: Drv.DataBuffer

    @ The output data buffer port
    output port BufferOut: Drv.DataBuffer

    @ Input ping port
    async input port PingIn: Svc.Ping drop

    @ Output ping port
    output port PingOut: Svc.Ping

    # ----------------------------------------------------------------------
    # Special ports
    # ----------------------------------------------------------------------

    @ Time get port
    time get port Time

    @ Telemetry port
    telemetry port Tlm

    @ Command receive port
    command recv port CmdDisp

    @ Command registration port
    command reg port CmdReg

    @ Command response port
    command resp port CmdStatus

    @ Event port
    event port Log

    @ Text event port
    text event port LogText

    # ----------------------------------------------------------------------
    # Commands
    # ----------------------------------------------------------------------

    @ Set the BD_Cycles ceiling
    async command BD_SET_CYCLE_CEILING(
                                         ceiling: U32 @< Cycle count ceiling
                                       ) \
      opcode 0

    # ----------------------------------------------------------------------
    # Events
    # ----------------------------------------------------------------------

    @ BD_Cycles exceeded the configured ceiling
    event BD_CycleCeilingExceeded(
                                   cycles: U32 @< Reported BD_Cycles value
                                   ceiling: U32 @< Active ceiling
                                 ) \
      severity warning high \
      id 0 \
      format "BD_Cycles {} exceeded ceiling {}"

    # ----------------------------------------------------------------------
    # Telemetry
    # ----------------------------------------------------------------------

    include "Tlm.fppi"

  }

}
