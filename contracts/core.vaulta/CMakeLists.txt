add_contract(core.vaulta core.vaulta ${CMAKE_CURRENT_SOURCE_DIR}/src/core.vaulta.cpp)

target_include_directories(core.vaulta
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(core.vaulta
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/core.vaulta.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/core.vaulta.contracts.md @ONLY )

target_compile_options( core.vaulta PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )
