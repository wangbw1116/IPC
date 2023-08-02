############ Setup project and cmake
# Minimum cmake requirement. We should require a quite recent
# cmake for the dependency find macros etc. to be up to date.
cmake_minimum_required (VERSION 2.8.8)

project(mq)

add_executable(posix_mq_demo posix_mq_demo.c)

target_link_libraries(posix_mq_demo rt)
target_link_libraries(posix_mq_demo pthread)

#add_executable(systemv_mq_demo systemv_mq_demo.c)