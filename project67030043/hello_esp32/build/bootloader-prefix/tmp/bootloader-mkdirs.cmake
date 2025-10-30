# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/OHMZUKi/esp/v5.5.1/esp-idf/components/bootloader/subproject")
  file(MAKE_DIRECTORY "C:/Users/OHMZUKi/esp/v5.5.1/esp-idf/components/bootloader/subproject")
endif()
file(MAKE_DIRECTORY
  "C:/Users/OHMZUKi/OneDrive/Desktop/Week12_043/043_Week-12-FreeRTOS_Worksheets/01-freertos-overview/practice/lab1-setup-first-project/hello_esp32/build/bootloader"
  "C:/Users/OHMZUKi/OneDrive/Desktop/Week12_043/043_Week-12-FreeRTOS_Worksheets/01-freertos-overview/practice/lab1-setup-first-project/hello_esp32/build/bootloader-prefix"
  "C:/Users/OHMZUKi/OneDrive/Desktop/Week12_043/043_Week-12-FreeRTOS_Worksheets/01-freertos-overview/practice/lab1-setup-first-project/hello_esp32/build/bootloader-prefix/tmp"
  "C:/Users/OHMZUKi/OneDrive/Desktop/Week12_043/043_Week-12-FreeRTOS_Worksheets/01-freertos-overview/practice/lab1-setup-first-project/hello_esp32/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/OHMZUKi/OneDrive/Desktop/Week12_043/043_Week-12-FreeRTOS_Worksheets/01-freertos-overview/practice/lab1-setup-first-project/hello_esp32/build/bootloader-prefix/src"
  "C:/Users/OHMZUKi/OneDrive/Desktop/Week12_043/043_Week-12-FreeRTOS_Worksheets/01-freertos-overview/practice/lab1-setup-first-project/hello_esp32/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/OHMZUKi/OneDrive/Desktop/Week12_043/043_Week-12-FreeRTOS_Worksheets/01-freertos-overview/practice/lab1-setup-first-project/hello_esp32/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/OHMZUKi/OneDrive/Desktop/Week12_043/043_Week-12-FreeRTOS_Worksheets/01-freertos-overview/practice/lab1-setup-first-project/hello_esp32/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
