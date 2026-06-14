add_library(uText STATIC)
set_target_properties(uText PROPERTIES LINKER_LANGUAGE CXX)

target_sources(uText PUBLIC FILE_SET HEADERS FILES
     include/uText.h
     include/unicode.h
)
target_sources(uText PRIVATE src/uText.cpp)
target_include_directories(uText PUBLIC include)
