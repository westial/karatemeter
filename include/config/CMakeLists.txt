set(HEADER_FILES
        displayconfig.h
        impactconfig.h
        typeconfig.h
        )

add_library(config STATIC ${HEADER_FILES})
