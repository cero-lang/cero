file(GLOB_RECURSE sources CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cc *.hh)

add_executable(cero-ast
  ${sources}
)

target_compile_features(cero-ast
PRIVATE
  cxx_std_20
)

target_compile_options(cero-ast
PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W3 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
