project (Karatemeter)

include_directories(${PROJECT_SOURCE_DIR}/../include)
include_directories(${PROJECT_SOURCE_DIR}/../include/sparring)
include_directories(${PROJECT_SOURCE_DIR}/../include/minlib)
include_directories(${PROJECT_SOURCE_DIR}/../include/impact)
include_directories(${PROJECT_SOURCE_DIR}/../include/display)
include_directories(${PROJECT_SOURCE_DIR}/../include/usecase)
include_directories(${PROJECT_SOURCE_DIR}/../include/base/config)

# Add here all libraries in src
# And link them in the runnable CMakeLists file
add_library(AudibleSparring sparring/AudibleSparring.c)
add_library(Accelerometer3AxesImpact impact/Accelerometer3AxesImpact.c)
add_library(ArrayMaxQueue minlib/CachedMaxQueue.c)
add_library(ArrayPeak impact/ArrayPeak.c)
add_library(MonochromeDisplay display/MonochromeDisplay.c)
add_library(MeasureAndDisplayByModeUseCase usecase/MeasureAndDisplayByModeUseCase.c)
