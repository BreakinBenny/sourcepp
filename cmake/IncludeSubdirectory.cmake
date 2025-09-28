include_guard(GLOBAL)

function(include_subdirectory SUBDIR)
    cmake_parse_arguments(PARSE_ARGV 1 OPTIONS "" "" "PROPAGATE")
    cmake_path(GET SUBDIR FILENAME SUBDIR_FILENAME)
    include("${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/_${SUBDIR_FILENAME}.cmake")
    return(PROPAGATE ${OPTIONS_PROPAGATE})
endfunction()
