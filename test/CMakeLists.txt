project (Karatemeter)

# include project source
include_directories(${PROJECT_SOURCE_DIR}/../include)
include_directories(${PROJECT_SOURCE_DIR}/../include/sparring)
include_directories(${PROJECT_SOURCE_DIR}/../include/minlib)
include_directories(${PROJECT_SOURCE_DIR}/../include/impact)
include_directories(${PROJECT_SOURCE_DIR}/../include/display)
include_directories(${PROJECT_SOURCE_DIR}/../include/usecase)
include_directories(${PROJECT_SOURCE_DIR}/../include/base/config)

# include mocks only for test code

# include CppUTest headers
include_directories(SYSTEM $ENV{CPPUTEST_HOME})

# add cpputest library
add_library(imp_cpputest STATIC IMPORTED)
set_property(TARGET imp_cpputest PROPERTY IMPORTED_LOCATION /usr/local/lib/libCppUTest.a)

# build test libraries
add_library(SparringTest SparringTest.cpp)
add_library(ImpactTest MonoImpactTest.cpp)
add_library(PeakTest PeakTest.cpp)
add_library(DisplayTest DisplayTest.cpp)
add_library(MaxQueueTest MaxQueueTest.cpp)
add_library(MeasureAndDisplayByModeUseCaseTest MeasureAndDisplayByModeUseCaseTest.cpp)

# add mocking library

# add main executable
add_executable(RunAllTests RunAllTests.cpp)

# link libraries
target_link_libraries(RunAllTests
        imp_cpputest
        SparringTest
        ImpactTest
        PeakTest
        DisplayTest
        MaxQueueTest
        MeasureAndDisplayByModeUseCaseTest
        MeasureAndDisplayByModeUseCase
        AudibleSparring
        MonoAccelerometerImpact
        ArrayPeak
        ArrayMaxQueue
        MonochromeDisplay
)
