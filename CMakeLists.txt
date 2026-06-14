cmake_minimum_required(VERSION 3.21)
project(kenlm-android LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(JNI REQUIRED)

set(KENLM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/kenlm")

set(KENLM_SRCS
    ${KENLM_DIR}/lm/model.cc
    ${KENLM_DIR}/lm/search_hashed.cc
    ${KENLM_DIR}/lm/search_trie.cc
    ${KENLM_DIR}/lm/vocab.cc
    ${KENLM_DIR}/lm/quantize.cc
    ${KENLM_DIR}/lm/bhiksha.cc
    ${KENLM_DIR}/lm/binary_format.cc
    ${KENLM_DIR}/lm/config.cc
    ${KENLM_DIR}/lm/trie.cc
    ${KENLM_DIR}/lm/trie_sort.cc
    ${KENLM_DIR}/lm/virtual_interface.cc
    ${KENLM_DIR}/lm/value_build.cc
    ${KENLM_DIR}/lm/lm_exception.cc
    ${KENLM_DIR}/lm/sizes.cc
    ${KENLM_DIR}/lm/read_arpa.cc
    ${KENLM_DIR}/util/bit_packing.cc
    ${KENLM_DIR}/util/ersatz_progress.cc
    ${KENLM_DIR}/util/exception.cc
    ${KENLM_DIR}/util/file.cc
    ${KENLM_DIR}/util/file_piece.cc
    ${KENLM_DIR}/util/float_to_string.cc
    ${KENLM_DIR}/util/integer_to_string.cc
    ${KENLM_DIR}/util/mmap.cc
    ${KENLM_DIR}/util/murmur_hash.cc
    ${KENLM_DIR}/util/pool.cc
    ${KENLM_DIR}/util/scoped.cc
    ${KENLM_DIR}/util/spaces.cc
    ${KENLM_DIR}/util/string_piece.cc
    ${KENLM_DIR}/util/read_compressed.cc
    ${KENLM_DIR}/util/usage.cc
    ${KENLM_DIR}/util/parallel_read.cc
    ${KENLM_DIR}/util/double-conversion/bignum-dtoa.cc
    ${KENLM_DIR}/util/double-conversion/bignum.cc
    ${KENLM_DIR}/util/double-conversion/cached-powers.cc
    ${KENLM_DIR}/util/double-conversion/double-to-string.cc
    ${KENLM_DIR}/util/double-conversion/fast-dtoa.cc
    ${KENLM_DIR}/util/double-conversion/fixed-dtoa.cc
    ${KENLM_DIR}/util/double-conversion/string-to-double.cc
    ${KENLM_DIR}/util/double-conversion/strtod.cc
)

add_library(kenlm STATIC ${KENLM_SRCS})
target_include_directories(kenlm PUBLIC ${KENLM_DIR})
target_compile_definitions(kenlm PUBLIC KENLM_MAX_ORDER=6)
target_compile_options(kenlm PRIVATE -fPIC -w)

add_library(kenlm_jni SHARED kenlm_jni.cpp)
target_link_libraries(kenlm_jni PRIVATE kenlm JNI::JNI)
