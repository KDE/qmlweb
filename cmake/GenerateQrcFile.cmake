
function(generate_qrc_file)
  set(oneValueArgs FILENAME FILEPATH_VAR)
  set(multiValueArgs RESOURCES)

  cmake_parse_arguments(GENQRC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  foreach(resource ${GENQRC_RESOURCES})
    get_filename_component(basename ${resource} NAME)
    string(CONCAT resources_string ${resources_string} "    <file alias=\"${basename}\">" ${CMAKE_CURRENT_SOURCE_DIR}/${resource} "</file>\n")
  endforeach()

  set(generatedqrc_filepath ${CMAKE_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${GENQRC_FILENAME})
  set(${GENQRC_FILEPATH_VAR} ${generatedqrc_filepath} PARENT_SCOPE)

  file(WRITE ${generatedqrc_filepath} "<!DOCTYPE RCC><RCC version=\"1.0\">
<qresource prefix=\"/test\">
    ${resources_string}
</qresource>
</RCC>
"
  )

endfunction()
