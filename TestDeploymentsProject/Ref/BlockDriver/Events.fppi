@ BD_Cycles exceeded the configured cycle ceiling (REQ-BD-003)
event BD_CycleCeilingExceeded(
                                cycles: U32
                                ceiling: U32
                              ) \
  severity warning high \
  id 0 \
  format "BD_Cycles {} exceeded cycle ceiling {}"
