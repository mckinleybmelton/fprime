module Ref {

  @ A rate group active component with input and output ping ports
  active component PingReceiver {

    # ----------------------------------------------------------------------
    # General Ports
    # ----------------------------------------------------------------------

    @ The ping input port
    async input port PingIn: Svc.Ping drop

    @ The ping input port
    output port PingOut: Svc.Ping

    # ----------------------------------------------------------------------
    # Special ports
    # ----------------------------------------------------------------------

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

    @ Time get port
    time get port Time

    @ Telemetry port
    telemetry port Tlm

    # ----------------------------------------------------------------------
    # Commands
    # ----------------------------------------------------------------------

    @ Command to disable ping response
    async command PR_StopPings \
      opcode 0

    @ Set the maximum allowed PR_NumPings before a warning
    async command PR_SetPingCeiling(
                                     ceiling: U32 @< Ping count ceiling
                                   ) \
      opcode 1

    # ----------------------------------------------------------------------
    # Events
    # ----------------------------------------------------------------------

    @ Disabled ping responses
    event PR_PingsDisabled \
      severity activity high \
      id 0 \
      format "PingReceiver ping responses disabled"

    @ Got ping
    event PR_PingReceived(
                           code: U32 @< Ping code
                         ) \
      severity diagnostic \
      id 1 \
      format "PingReceiver pinged with code {}"

    @ PR_NumPings exceeded the configured ceiling
    event PR_PingCeilingExceeded(
                                  numPings: U32 @< Current PR_NumPings value
                                  ceiling: U32 @< Active ceiling
                                ) \
      severity warning high \
      id 2 \
      format "PR_NumPings {} exceeded ping ceiling {}"

    # ----------------------------------------------------------------------
    # Telemetry
    # ----------------------------------------------------------------------

    @ Number of pings received
    telemetry PR_NumPings: U32 id 0

    @ Active ping ceiling
    telemetry PR_PingCeiling: U32 id 1

  }

}
