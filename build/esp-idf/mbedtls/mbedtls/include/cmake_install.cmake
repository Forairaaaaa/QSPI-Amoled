# Install script for directory: /home/parker/esp/esp-idf/components/mbedtls/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aes.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aria.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1write.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/base64.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/bignum.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/build_info.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/camellia.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ccm.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chacha20.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chachapoly.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/check_config.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cipher.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cmac.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/compat-2.x.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_psa.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/constant_time.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ctr_drbg.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/debug.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/des.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/dhm.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdh.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdsa.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecjpake.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecp.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/entropy.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/error.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/gcm.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hkdf.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hmac_drbg.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/mbedtls_config.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md5.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/net_sockets.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/nist_kw.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/oid.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pem.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pk.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs12.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs5.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_time.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_util.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/poly1305.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/private_access.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/psa_util.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ripemd160.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/rsa.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha1.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha256.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha512.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cache.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cookie.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ticket.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/threading.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/timing.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/version.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crl.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crt.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_csr.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/psa" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/psa/crypto.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_composites.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_primitives.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_compat.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_config.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_common.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_composites.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_primitives.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_extra.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_platform.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_se_driver.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_sizes.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_struct.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_types.h"
    "/home/parker/esp/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_values.h"
    )
endif()

