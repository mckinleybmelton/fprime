####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
#
####
register_fprime_module(
  AUTOCODER_INPUTS
    "${CMAKE_CURRENT_LIST_DIR}/PingReceiver.fpp"
  SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/PingReceiverComponentImpl.cpp"
  HEADERS
    "${CMAKE_CURRENT_LIST_DIR}/PingReceiver.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/PingReceiverComponentImpl.hpp"
)

### Unit Tests ###
register_fprime_ut(
  AUTOCODER_INPUTS
    "${CMAKE_CURRENT_LIST_DIR}/PingReceiver.fpp"
  SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/test/ut/PingReceiverTester.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test/ut/PingReceiverTestMain.cpp"
  UT_AUTO_HELPERS
)
