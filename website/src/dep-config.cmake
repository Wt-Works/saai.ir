set(Boost_FIND_REQUIRED TRUE)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS OFF)

set(CRYPTOPP_FIND_REQUIRED TRUE)

set(DBIXX_FIND_REQUIRED TRUE)

set(FASTCGIPP_FIND_REQUIRED TRUE)

set(GEOIP_FIND_REQUIRED TRUE)

set(JWSMTP_FIND_REQUIRED TRUE)
set(JWSMTP_VERSION 1.32)
set(JWSMTP_HEADER_DIR "jwsmtp-${JWSMTP_VERSION}")

set(LIBB64_FIND_REQUIRED TRUE)

set(MAGICKPP_FIND_REQUIRED TRUE)
set(MAGICKPP_CONFIG_FIND_REQUIRED TRUE)

set(SQLITE_FIND_REQUIRED TRUE)

set(WT_FIND_REQUIRED TRUE)
#set(WT_EXT_FIND_REQUIRED TRUE)
set(WT_FCGI_FIND_REQUIRED TRUE)



### Boost ###

find_package( Boost 1.42.0 COMPONENTS date_time filesystem thread )

if (Boost_FOUND)
   set(DEP_FOUND TRUE)
else (Boost_FOUND)
   set(DEP_FOUND FALSE)
   if (Boost_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Boost")
   endif (Boost_FIND_REQUIRED)
endif (Boost_FOUND)



### Crypto++ ###

find_path(CRYPTOPP_INCLUDE_DIR crypto++ /usr/include/ /usr/local/include/)
find_library(CRYPTOPP_LIBRARY NAMES cryptopp PATH /usr/lib /usr/local/lib) 

if (CRYPTOPP_INCLUDE_DIR AND CRYPTOPP_LIBRARY)
   set(CRYPTOPP_FOUND TRUE)
endif (CRYPTOPP_INCLUDE_DIR AND CRYPTOPP_LIBRARY)


if (CRYPTOPP_FOUND)
   set(DEP_FOUND TRUE)
   if (NOT CRYPTOPP_FIND_QUIETLY)
      message(STATUS "Found Crypto++: ${CRYPTOPP_LIBRARY}")
   endif (NOT CRYPTOPP_FIND_QUIETLY)
else (CRYPTOPP_FOUND)
   set(DEP_FOUND FALSE)
   if (CRYPTOPP_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Crypto++")
   endif (CRYPTOPP_FIND_REQUIRED)
endif (CRYPTOPP_FOUND)



### DbiXX ###

find_path(DBIXX_INCLUDE_DIR dbixx /usr/include/ /usr/local/include/)
find_library(DBIXX_LIBRARY NAMES dbixx PATH /usr/lib /usr/local/lib) 

if (DBIXX_INCLUDE_DIR AND DBIXX_LIBRARY)
   set(DBIXX_FOUND TRUE)
endif (DBIXX_INCLUDE_DIR AND DBIXX_LIBRARY)


if (DBIXX_FOUND)
   set(DEP_FOUND TRUE)
   if (NOT DBIXX_FIND_QUIETLY)
      message(STATUS "Found DbiXX: ${DBIXX_LIBRARY}")
   endif (NOT DBIXX_FIND_QUIETLY)
else (DBIXX_FOUND)
   set(DEP_FOUND FALSE)
   if (DBIXX_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find DbiXX")
   endif (DBIXX_FIND_REQUIRED)
endif (DBIXX_FOUND)



### fastcgi++ ###

find_path(FASTCGIPP_INCLUDE_DIR fastcgi++ /usr/include/ /usr/local/include/)
find_library(FASTCGIPP_LIBRARY NAMES fastcgipp PATH /usr/lib /usr/local/lib) 

if (FASTCGIPP_INCLUDE_DIR AND FASTCGIPP_LIBRARY)
   set(FASTCGIPP_FOUND TRUE)
endif (FASTCGIPP_INCLUDE_DIR AND FASTCGIPP_LIBRARY)


if (FASTCGIPP_FOUND)
   set(DEP_FOUND TRUE)
   if (NOT FASTCGIPP_FIND_QUIETLY)
      message(STATUS "Found fastcgi++: ${FASTCGIPP_LIBRARY}")
   endif (NOT FASTCGIPP_FIND_QUIETLY)
else (FASTCGIPP_FOUND)
   set(DEP_FOUND FALSE)
   if (FASTCGIPP_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find fastcgi++")
   endif (FASTCGIPP_FIND_REQUIRED)
endif (FASTCGIPP_FOUND)



### GeoIP ###

find_path(GEOIP_INCLUDE_DIR GeoIP.h /usr/include/ /usr/local/include/)
find_library(GEOIP_LIBRARY NAMES GeoIP PATH /usr/lib /usr/local/lib) 

if (GEOIP_INCLUDE_DIR AND GEOIP_LIBRARY)
   set(GEOIP_FOUND TRUE)
endif (GEOIP_INCLUDE_DIR AND GEOIP_LIBRARY)


if (GEOIP_FOUND)
   set(DEP_FOUND TRUE)
   if (NOT GEOIP_FIND_QUIETLY)
      message(STATUS "Found GeoIP: ${GEOIP_LIBRARY}")
   endif (NOT GEOIP_FIND_QUIETLY)
else (GEOIP_FOUND)
   set(DEP_FOUND FALSE)
   if (GEOIP_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find GeoIP")
   endif (GEOIP_FIND_REQUIRED)
endif (GEOIP_FOUND)



### jwSMTP ###

find_path(JWSMTP_INCLUDE_DIR jwsmtp-1.32 /usr/include/ /usr/local/include/)
find_library(JWSMTP_LIBRARY NAMES jwsmtp PATH /usr/lib /usr/local/lib) 

if (JWSMTP_INCLUDE_DIR AND JWSMTP_LIBRARY)
   set(JWSMTP_FOUND TRUE)
   set(JWSMTP_INCLUDE_DIR "${JWSMTP_INCLUDE_DIR}/${JWSMTP_HEADER_DIR}")
endif (JWSMTP_INCLUDE_DIR AND JWSMTP_LIBRARY)


if (JWSMTP_FOUND)
   set(DEP_FOUND TRUE)
   if (NOT JWSMTP_FIND_QUIETLY)
      message(STATUS "Found jwSMTP: ${JWSMTP_LIBRARY}")
   endif (NOT JWSMTP_FIND_QUIETLY)
else (JWSMTP_FOUND)
   set(DEP_FOUND FALSE)
   if (JWSMTP_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find jwSMTP")
   endif (JWSMTP_FIND_REQUIRED)
endif (JWSMTP_FOUND)



### libb64 ###

find_path(LIBB64_INCLUDE_DIR b64 /usr/include/ /usr/local/include/)
find_library(LIBB64_LIBRARY NAMES b64 PATH /usr/lib /usr/local/lib) 

if (LIBB64_INCLUDE_DIR AND LIBB64_LIBRARY)
   set(LIBB64_FOUND TRUE)
endif (LIBB64_INCLUDE_DIR AND LIBB64_LIBRARY)


if (LIBB64_FOUND)
   set(DEP_FOUND TRUE)
   if (NOT LIBB64_FIND_QUIETLY)
      message(STATUS "Found libb64: ${LIBB64_LIBRARY}")
   endif (NOT LIBB64_FIND_QUIETLY)
else (LIBB64_FOUND)
   set(DEP_FOUND FALSE)
   if (LIBB64_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find libb64")
   endif (LIBB64_FIND_REQUIRED)
endif (LIBB64_FOUND)



### Magick++ ###

find_path(MAGICKPP_INCLUDE_DIR ImageMagick/Magick++ /usr/include/ /usr/local/include/)
find_library(MAGICKPP_LIBRARY NAMES Magick++ PATH /usr/lib /usr/local/lib) 
find_file(MAGICKPP_CONFIG_SCRIPT NAMES Magick++-config PATH /usr/bin /usr/local/bin) 

if (MAGICKPP_INCLUDE_DIR AND MAGICKPP_LIBRARY)
   set(MAGICKPP_FOUND TRUE)
endif (MAGICKPP_INCLUDE_DIR AND MAGICKPP_LIBRARY)


if (MAGICKPP_FOUND)
   set(DEP_FOUND TRUE)
   if (NOT MAGICKPP_FIND_QUIETLY)
      message(STATUS "Found Magick++: ${MAGICKPP_LIBRARY}")
   endif (NOT MAGICKPP_FIND_QUIETLY)
else (MAGICKPP_FOUND)
   set(DEP_FOUND FALSE)
   if (MAGICKPP_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Magick++")
   endif (MAGICKPP_FIND_REQUIRED)
endif (MAGICKPP_FOUND)


if (MAGICKPP_CONFIG_SCRIPT)
   set(MAGICKPP_CONFIG_FOUND TRUE)
endif (MAGICKPP_CONFIG_SCRIPT)


if (MAGICKPP_CONFIG_FOUND)
   set(DEP_FOUND TRUE)
   if (NOT MAGICKPP_CONFIG_FIND_QUIETLY)
      EXEC_PROGRAM ("${MAGICKPP_CONFIG_SCRIPT}" ARGS --cppflags | sed 's/-I//g' OUTPUT_VARIABLE MAGICKPP_INCLUDE_DIR)
      #EXEC_PROGRAM ("${MAGICKPP_CONFIG_SCRIPT}" ARGS --libs | cut -d ' ' -f '2-99' | sed 's/ -l/ /g' | sed 's/-l//' OUTPUT_VARIABLE MAGICKPP_FLAGS)
      EXEC_PROGRAM ("${MAGICKPP_CONFIG_SCRIPT}" ARGS --libs OUTPUT_VARIABLE MAGICKPP_FLAGS)
      message(STATUS "Found Magick++-config: ${MAGICKPP_CONFIG_SCRIPT}")
   endif (NOT MAGICKPP_CONFIG_FIND_QUIETLY)
else (MAGICKPP_CONFIG_FOUND)
   set(DEP_FOUND FALSE)
   if (MAGICKPP_CONFIG_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Magick++-config")
   endif (MAGICKPP_CONFIG_FIND_REQUIRED)
endif (MAGICKPP_CONFIG_FOUND)



### SQLite ###

find_path(SQLITE_INCLUDE_DIR sqlite3.h /usr/include/ /usr/local/include/)
find_library(SQLITE_LIBRARY NAMES sqlite3 PATH /usr/lib /usr/local/lib) 

if (SQLITE_INCLUDE_DIR AND SQLITE_LIBRARY)
   set(SQLITE_FOUND TRUE)
endif (SQLITE_INCLUDE_DIR AND SQLITE_LIBRARY)


if (SQLITE_FOUND)
   set(DEP_FOUND TRUE)
   if (NOT SQLITE_FIND_QUIETLY)
      message(STATUS "Found SQLite3: ${SQLITE_LIBRARY}")
   endif (NOT SQLITE_FIND_QUIETLY)
else (SQLITE_FOUND)
   set(DEP_FOUND FALSE)
   if (SQLITE_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find SQLite3")
   endif (SQLITE_FIND_REQUIRED)
endif (SQLITE_FOUND)



### Wt ###

find_path(WT_INCLUDE_DIR Wt /usr/include/ /usr/local/include/)
find_library(WT_LIBRARY NAMES wt PATH /usr/lib /usr/local/lib) 
find_library(WT_FCGI_LIBRARY NAMES wtfcgi PATH /usr/lib /usr/local/lib) 

if (WT_INCLUDE_DIR AND WT_LIBRARY)
   set(WT_FOUND TRUE)
endif (WT_INCLUDE_DIR AND WT_LIBRARY)


if (WT_FOUND)
   set(DEP_FOUND TRUE)
   if (NOT WT_FIND_QUIETLY)
      message(STATUS "Found Wt: ${WT_LIBRARY}")
   endif (NOT WT_FIND_QUIETLY)
else (WT_FOUND)
   set(DEP_FOUND FALSE)
   if (WT_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Wt")
   endif (WT_FIND_REQUIRED)
endif (WT_FOUND)


#if (WT_EXT_LIBRARY)
#   set(WT_EXT_FOUND TRUE)
#endif (WT_EXT_LIBRARY)


#if (WT_EXT_FOUND)
#   set(DEP_FOUND TRUE)
#   if (NOT WT_EXT_FIND_QUIETLY)
#      message(STATUS "Found WtExt: ${WT_EXT_LIBRARY}")
#   endif (NOT WT_EXT_FIND_QUIETLY)
#else (WT_EXT_FOUND)
#   set(DEP_FOUND FALSE)
#   if (WT_EXT_FIND_REQUIRED)
#      message(FATAL_ERROR "Could not find WtExt")
#   endif (WT_EXT_FIND_REQUIRED)
#endif (WT_EXT_FOUND)


if (WT_FCGI_LIBRARY)
   set(WT_FCGI_FOUND TRUE)
endif (WT_FCGI_LIBRARY)


if (WT_FCGI_FOUND)
   set(DEP_FOUND TRUE)
   if (NOT WT_FCGI_FIND_QUIETLY)
      message(STATUS "Found WtFastCGI: ${WT_FCGI_LIBRARY}")
   endif (NOT WT_FCGI_FIND_QUIETLY)
else (WT_FCGI_FOUND)
   set(DEP_FOUND FALSE)
   if (WT_FCGI_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find WtFastCGI")
   endif (WT_FCGI_FIND_REQUIRED)
endif (WT_FCGI_FOUND)




