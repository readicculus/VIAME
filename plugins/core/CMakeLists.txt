# Build / Install Plugin containing core algorithm implementations

set( plugin_headers
  add_timestamp_from_filename.h
  read_detected_object_set_habcam.h
  read_detected_object_set_viame_csv.h
  read_object_track_set_viame_csv.h
  write_detected_object_set_viame_csv.h
  write_object_track_set_viame_csv.h
  )

set( plugin_sources
  add_timestamp_from_filename.cxx
  read_detected_object_set_habcam.cxx
  read_detected_object_set_viame_csv.cxx
  read_object_track_set_viame_csv.cxx
  write_detected_object_set_viame_csv.cxx
  write_object_track_set_viame_csv.cxx
  )

kwiver_install_headers(
  SUBDIR     viame
  ${plugin_headers}
  )

kwiver_install_headers(
  ${CMAKE_CURRENT_BINARY_DIR}/viame_core_export.h
  NOPATH   SUBDIR     viame
  )

kwiver_add_library( viame_core
  ${plugin_headers}
  ${plugin_sources}
  )

target_link_libraries( viame_core
  PUBLIC               vital
                       vital_algo
                       vital_config
                       vital_exceptions
                       vital_logger
                       vital_util
                       kwiversys
  )

set_target_properties( viame_core PROPERTIES
  SOVERSION            ${VIAME_VERSION_MAJOR}
  )

algorithms_create_plugin( viame_core
  register_algorithms.cxx
  )
  
target_link_libraries( viame_core_plugin
  PUBLIC               vital_vpm
  )

kwiver_add_executable( viame_train_detector
  viame_train_detector.cxx
  )

find_package(Boost ${KWIVER_BOOST_VERSION} REQUIRED
    COMPONENTS
      chrono
      filesystem
      system)

include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )

target_link_libraries( viame_train_detector
  PRIVATE      vital
               vital_vpm
               vital_config
               vital_exceptions
               vital_logger
               vital_algo
               vital_util
               kwiversys
               kwiver::sprokit_pipeline
               kwiver::kwiver_adapter
               ${Boost_SYSTEM_LIBRARY}
               ${Boost_FILESYSTEM_LIBRARY}
  )

# Add process library
set( process_headers
  align_multimodal_imagery_process.h
  filter_frame_process.h
  write_homography_list_process.h
)

set( process_sources
  register_processes.cxx
  align_multimodal_imagery_process.cxx
  filter_frame_process.cxx
  write_homography_list_process.cxx
)

kwiver_add_plugin( viame_processes_core
  SUBDIR           ${viame_plugin_process_subdir}
  SOURCES          ${process_sources}
                   ${private_headers}
  PRIVATE          sprokit_pipeline
                   kwiver_algo_core kwiversys kwiver_adapter
                   vital vital_vpm vital_logger vital_config
                   vital_util vital_exceptions
                   ${Boost_SYSTEM_LIBRARY}
                   ${Boost_FILESYSTEM_LIBRARY}
 )

if( VIAME_ENABLE_PYTHON )

  kwiver_create_python_init( viame/arrows/core )

  kwiver_add_python_module(
    ${CMAKE_CURRENT_SOURCE_DIR}/npy_image_normalization.py
    viame/arrows/core
    npy_image_normalization )

  kwiver_add_python_module(
    ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
    viame/processes/core
    __init__ )

  kwiver_add_python_module(
    ${CMAKE_CURRENT_SOURCE_DIR}/utility_processes.py
    viame/processes/core
    utility_processes )

endif()
