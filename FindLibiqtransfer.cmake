if(NOT LIBIQTRANSFER_FOUND)
    # pkg_check_modules (LIBIQTRANSFER_PKG LIBIQTRANSFER)


        find_path(LIBIQTRANSFER_INCLUDE_DIRS NAMES libiqtransfer.h
                PATHS
                /usr/include
                /usr/local/include
                )

        find_library(LIBIQTRANSFER_LIBRARIES NAMES iqtransfer
                PATHS
                /usr/lib
                /usr/local/lib
                )



    if(LIBIQTRANSFER_INCLUDE_DIRS AND LIBIQTRANSFER_LIBRARIES)
        set(LIBIQTRANSFER_FOUND TRUE CACHE INTERNAL "LIBIQTRANSFER found")
        message(STATUS "Found LIBIQTRANSFER: ${LIBIQTRANSFER_INCLUDE_DIRS}, ${LIBIQTRANSFER_LIBRARIES}")
    else(LIBIQTRANSFER_INCLUDE_DIRS AND LIBIQTRANSFER_LIBRARIES)
        set(LIBIQTRANSFER_FOUND FALSE CACHE INTERNAL "LIBIQTRANSFER found")
        message(STATUS "LIBIQTRANSFER not found.")
    endif(LIBIQTRANSFER_INCLUDE_DIRS AND LIBIQTRANSFER_LIBRARIES)

    #mark_as_advanced(LIBIQTRANSFER_LIBRARIES LIBIQTRANSFER_INCLUDE_DIRS)

endif(NOT LIBIQTRANSFER_FOUND)