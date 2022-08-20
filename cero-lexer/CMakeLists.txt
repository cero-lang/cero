file(GLOB_RECURSE sources CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cc *.hh)

add_executable(cero-lexer
  ${sources}
)

target_compile_features(cero-lexer
PRIVATE
  cxx_std_20
)

target_compile_options(cero-lexer
PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W3 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

target_link_libraries(cero-lexer
PRIVATE
  magic_enum
)
