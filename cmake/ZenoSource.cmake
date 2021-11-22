zeno_glob_recurse(source zeno *.h *.cpp)
target_sources(zeno PRIVATE ${source})
target_include_directories(zeno PUBLIC zeno)
target_include_directories(zeno PUBLIC 3rdparty/include)

#find_package(fmt REQUIRED)
#message(STATUS "Found fmt: ${fmt_DIR}")
#target_link_libraries(zeno PUBLIC fmt::fmt)

#find_package(spdlog REQUIRED)
#message(STATUS "Found spdlog: ${spdlog_DIR}")
#target_link_libraries(zeno PUBLIC spdlog::spdlog)

#find_package(range-v3 REQUIRED)
#message(STATUS "Found range-v3: ${range-v3_DIR}")
#target_link_libraries(zeno PUBLIC range-v3::range-v3)

find_package(TBB COMPONENTS tbb REQUIRED)
message(STATUS "Found TBB: ${TBB_DIR}")
target_link_libraries(zeno PUBLIC TBB::tbb)

if (ZENO_WITH_SYCL)
    message(STATUS "Building Zeno with hipSYCL targets: [${HIPSYCL_TARGETS}]")
    find_package(hipSYCL CONFIG REQUIRED)
    add_sycl_to_target(TARGET zeno)
    target_compile_definitions(zeno PUBLIC -DZENO_WITH_SYCL)
endif()

if (ZENO_WITH_LEGACY)
    message(STATUS "Building Zeno with Legacy Nodes")
    zeno_glob_recurse(source legacy *.h *.cpp)
    target_include_directories(zeno PUBLIC legacy)
    target_sources(zeno PRIVATE ${source})
endif()

if (ZENO_WITH_BACKWARD)
    message(STATUS "Building Zeno with Stack Traceback")
    add_subdirectory(3rdparty/mods/backward-cpp)
    target_sources(zeno PRIVATE ${BACKWARD_ENABLE})
    add_backward(zeno)
endif()

install(TARGETS zeno DESTINATION bin)
