cmake_minimum_required (VERSION 2.6)
project(SAAIIR)

# Optionally you can use -g:
# The "-g" tells the compiler to add debugging information to the executable.
set(LIBB64_BUFFERSIZE "-DBUFFERSIZE=16777216")
set(CMAKE_CXX_FLAGS "-Wall -pedantic -O2 -march=native -mtune=native -pipe -fomit-frame-pointer ${LIBB64_BUFFERSIZE}")


# if named Dep.cmake
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SAAIIR_SOURCE_DIR})
# else named depConfig.cmake or dep-config.cmake
set(dep_DIR ${SAAIIR_SOURCE_DIR})

find_package( dep )

if (DEP_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
   link_directories(${Boost_LIBRARY_DIRS})

   include_directories(${CRYPTOPP_INCLUDE_DIR})
   link_directories(${CRYPTOPP_LIBRARY})

   include_directories(${DBIXX_INCLUDE_DIR})
   link_directories(${DBIXX_LIBRARY})

   include_directories(${FASTCGIPP_INCLUDE_DIR})
   link_directories(${FASTCGIPP_LIBRARY})

   include_directories(${GEOIP_INCLUDE_DIR})
   link_directories(${GEOIP_LIBRARY})

   include_directories(${JWSMTP_INCLUDE_DIR})
   link_directories(${JWSMTP_LIBRARY})

   include_directories(${LIBB64_INCLUDE_DIR})
   link_directories(${LIBB64_LIBRARY})

   include_directories(${MAGICKPP_INCLUDE_DIR})
   link_directories(${MAGICKPP_LIBRARY})

   include_directories(${SQLITE_INCLUDE_DIR})
   link_directories(${SQLITE_LIBRARY})

   include_directories(${WT_INCLUDE_DIR})
   link_directories(${WT_LIBRARY})
   #link_directories(${WT_EXT_LIBRARY})
   link_directories(${WT_FCGI_LIBRARY})

   include(_src.cmake)

   set (ROOT_BIN "root.fcgi")
   set (CAPTCHA_BIN "captcha.fcgi")
   set (SHOWPICS_BIN "showpics.fcgi")

   add_executable(${ROOT_BIN} ${ROOT_SOURCE_FILES})
   target_link_libraries(${ROOT_BIN}
	${Boost_LIBRARIES} ${CRYPTOPP_LIBRARY} ${DBIXX_LIBRARY} ${GEOIP_LIBRARY} ${JWSMTP_LIBRARY}
        ${LIBB64_LIBRARY} ${MAGICKPP_FLAGS} ${SQLITE_LIBRARY} ${WT_LIBRARY} ${WT_FCGI_LIBRARY}
	-pthread #${WT_FLAGS}
   )

   add_executable(${CAPTCHA_BIN} ${CAPTCHA_SOURCE_FILES})
   target_link_libraries(${CAPTCHA_BIN}
	${Boost_LIBRARIES} ${FASTCGIPP_LIBRARY} ${MAGICKPP_FLAGS}
	-pthread
   )

   add_executable(${SHOWPICS_BIN} ${SHOWPICS_SOURCE_FILES})
   target_link_libraries(${SHOWPICS_BIN}
	${Boost_LIBRARIES} ${DBIXX_LIBRARY} ${FASTCGIPP_LIBRARY} ${LIBB64_LIBRARY} ${MAGICKPP_FLAGS}
	-pthread
   )

   set(CMAKE_INSTALL_PREFIX "${SAAIIR_SOURCE_DIR}/../www/")

   install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${ROOT_BIN}"
	"${CMAKE_CURRENT_BINARY_DIR}/${CAPTCHA_BIN}"
	"${CMAKE_CURRENT_BINARY_DIR}/${SHOWPICS_BIN}"

	DESTINATION "${CMAKE_INSTALL_PREFIX}"
	PERMISSIONS
	OWNER_READ OWNER_WRITE OWNER_EXECUTE
	GROUP_READ GROUP_EXECUTE
	WORLD_READ WORLD_EXECUTE
   )

endif (DEP_FOUND)

