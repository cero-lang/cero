file(GLOB_RECURSE Cero CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cc *.hh)

add_executable(Cero
  ${Cero}
)

target_compile_features(Cero
PRIVATE
  cxx_std_20
)

target_compile_options(Cero
PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
