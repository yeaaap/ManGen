file(GLOB TEST_SCRIPTS "${CMAKE_CURRENT_SOURCE_DIR}/test_*.sh")

foreach(TEST_SCRIPT ${TEST_SCRIPTS})
    get_filename_component(TEST_NAME ${TEST_SCRIPT} NAME_WE)

    add_test(
            NAME ${TEST_NAME}
            COMMAND bash ${TEST_SCRIPT}
    )
endforeach()
