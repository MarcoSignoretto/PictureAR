file(GLOB_RECURSE CvLab_SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
include_directories( ${PROJECT_SOURCE_DIR}/include )

add_executable(${CvLab_NAME} ${CvLab_SRC_FILES})

target_link_libraries( ${CvLab_NAME} ${OpenCV_LIBS}  )

INSTALL( TARGETS ${CvLab_NAME} DESTINATION bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )


## =====  Webcam target ===== ##
add_custom_target(run
    COMMAND ${CvLab_NAME}
    DEPENDS ${CvLab_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(run_D
    COMMAND ${CvLab_NAME} -D
    DEPENDS ${CvLab_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

## ===== Video targets ====== ##
#Video 1
add_custom_target(run_video1
    COMMAND ${CvLab_NAME} -s video -f data/test_video1.mp4
    DEPENDS ${CvLab_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(run_video1_D
    COMMAND ${CvLab_NAME} -D -s video -f data/test_video1.mp4
    DEPENDS ${CvLab_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
#Video 2
add_custom_target(run_video2
    COMMAND ${CvLab_NAME} -s video -f data/test_video2.mp4
    DEPENDS ${CvLab_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(run_video2_D
    COMMAND ${CvLab_NAME} -D -s video -f data/test_video2.mp4
    DEPENDS ${CvLab_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

## ===== Image targets ===== ##

#Image 1
add_custom_target(run_image1_D
    COMMAND ${CvLab_NAME} -D -s image -f data/test1.jpg
    DEPENDS ${CvLab_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(run_image1
    COMMAND ${CvLab_NAME} -s image -f data/test1.jpg
    DEPENDS ${CvLab_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)


#Image 2
add_custom_target(run_image2_D
    COMMAND ${CvLab_NAME} -D -s image -f data/test2.jpg
    DEPENDS ${CvLab_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(run_image2
    COMMAND ${CvLab_NAME} -s image -f data/test2.jpg
    DEPENDS ${CvLab_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
#Image 3
add_custom_target(run_image3_D
    COMMAND ${CvLab_NAME} -D -s image -f data/test3.jpg
    DEPENDS ${CvLab_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(run_image3
    COMMAND ${CvLab_NAME} -s image -f data/test3.jpg
    DEPENDS ${CvLab_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
#Image 4
add_custom_target(run_image4_D
    COMMAND ${CvLab_NAME} -D -s image -f data/test4.jpg
    DEPENDS ${CvLab_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(run_image4
    COMMAND ${CvLab_NAME} -s image -f data/test4.jpg
    DEPENDS ${CvLab_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)



