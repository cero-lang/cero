file(GLOB_RECURSE sources CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cc *.hh)

add_executable(cero
  ${sources}
)

add_dependencies(cero
  cero-ast
  cero-lexer
  cero-parse
)

target_compile_features(cero
PRIVATE
  cxx_std_20
)

target_compile_options(cero
PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W3 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

target_link_libraries(cero
PUBLIC
  cero-ast
  cero-lexer
  cero-parse

PRIVATE
  magic_enum
)
