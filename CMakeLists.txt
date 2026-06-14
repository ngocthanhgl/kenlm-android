cmake_minimum_required(VERSION 3.21)
project(kenlm-android LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(JNI REQUIRED)

set(KENLM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/kenlm")

set(KENLM_SRCS
    ${KENLM_DIR}/lm/model.cc
    ${KENLM_DIR}/lm/left.cc
    ${KENLM_DIR}/lm/bhiksha.cc
    ${KENLM_DIR}/lm/binary_format.cc
    ${KENLM_DIR}/lm/quantize.cc
    ${KENLM_DIR}/lm/config.cc
    ${KENLM_DIR}/lm/sri.cc
    ${KENLM_DIR}/lm/trie.cc
    ${KENLM_DIR}/lm/trie_sort.cc
    ${KENLM_DIR}/lm/search_hashed.cc
    ${KENLM_DIR}/lm/search_trie.cc
    ${KENLM_DIR}/lm/partial.cc
    ${KENLM_DIR}/lm/vocab.cc
    ${KENLM_DIR}/util/file.cc
    ${KENLM_DIR}/util/file_piece.cc
    ${KENLM_DIR}/util/ersatz_progress.cc
    ${KENLM_DIR}/util/exception.cc
    ${KENLM_DIR}/util/string_piece.cc
    ${KENLM_DIR}/util/mmap.cc
    ${KENLM_DIR}/util/bit_packing.cc
    ${KENLM_DIR}/util/pool.cc
    ${KENLM_DIR}/util/murmur_hash.cc
    ${KENLM_DIR}/util/sized_iterator.cc
)

add_library(kenlm STATIC ${KENLM_SRCS})
target_include_directories(kenlm PUBLIC ${KENLM_DIR})
target_compile_definitions(kenlm PRIVATE KENLM_MAX_ORDER=5)
target_compile_options(kenlm PRIVATE -fPIC -w)

add_library(kenlm_jni SHARED kenlm_jni.cpp)
target_link_libraries(kenlm_jni PRIVATE kenlm JNI::JNI)

find_path(BOOST_INCLUDE_DIR boost/assert.hpp)
if(BOOST_INCLUDE_DIR)
    target_include_directories(kenlm PRIVATE ${BOOST_INCLUDE_DIR})
endif()
