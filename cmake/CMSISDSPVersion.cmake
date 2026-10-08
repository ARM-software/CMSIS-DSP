# Copyright (c) 2026, Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Keep this fallback in sync with releases for archives and checkouts without tags.
# Packagers can override it with -DCMSISDSP_VERSION=<major>.<minor>.<patch>.
if (NOT DEFINED CMSISDSP_VERSION)
  set(CMSISDSP_VERSION "1.18.0")

  # Only inspect this repository: an unpacked archive may be inside another repo.
  if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/../.git")
    find_package(Git QUIET)
    if (GIT_FOUND)
      execute_process(
        COMMAND "${GIT_EXECUTABLE}" describe --tags --abbrev=0
          --match "v[0-9]*.[0-9]*.[0-9]*" --exclude "*-*" --exclude "*+*"
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/.."
        RESULT_VARIABLE _cmsisdsp_git_result
        OUTPUT_VARIABLE _cmsisdsp_git_tag
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET)
      if (_cmsisdsp_git_result EQUAL 0 AND
          _cmsisdsp_git_tag MATCHES "^v([0-9]+\\.[0-9]+\\.[0-9]+)$")
        set(CMSISDSP_VERSION "${CMAKE_MATCH_1}")
      endif()
      unset(_cmsisdsp_git_result)
      unset(_cmsisdsp_git_tag)
    endif()
  endif()
endif()

if (NOT CMSISDSP_VERSION MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+$")
  message(FATAL_ERROR "CMSISDSP_VERSION must have the form <major>.<minor>.<patch>")
endif()
