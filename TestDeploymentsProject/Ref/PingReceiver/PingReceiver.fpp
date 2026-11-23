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

    @ Command to set the ping ceiling
    async command PR_SetPingCeiling(
                                     ceiling: U32 @< Ping ceiling
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

    @ Ping ceiling was set
    event PR_PingCeilingSet(
                             ceiling: U32 @< Ping ceiling
                           ) \
      severity activity high \
      id 2 \
      format "Ping ceiling set to {}"

    @ PR_NumPings exceeded the ping ceiling
    event PR_PingCeilingExceeded(
                                  pings: U32 @< Reported PR_NumPings
                                  ceiling: U32 @< Ping ceiling
                                ) \
      severity warning high \
      id 3 \
      format "PR_NumPings {} exceeded ceiling {}"

    # ----------------------------------------------------------------------
    # Telemetry
    # ----------------------------------------------------------------------

    @ Number of pings received
    telemetry PR_NumPings: U32 id 0

    @ Active ping ceiling
    telemetry PR_PingCeiling: U32 id 1 update on change

  }

}
