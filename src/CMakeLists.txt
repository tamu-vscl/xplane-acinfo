project(acinfo)

set(SOURCES
  acinfo.c
)

set(HEADERS
)


add_library(${PROJECT_NAME} SHARED
  ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
  ${XPLM_LIB}
  ${XPWidgets_LIB}
  m
  GL
  GLU
  bsd
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
