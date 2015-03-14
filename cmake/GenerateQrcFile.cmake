
function(generate_qrc_file)
  set(oneValueArgs FILENAME FILEPATH_VAR)
  set(multiValueArgs RESOURCES)

  cmake_parse_arguments(GENQRC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  foreach(resource ${GENQRC_RESOURCES})
    string(CONCAT resources_string ${resources_string} "    <file>" ${CMAKE_CURRENT_SOURCE_DIR}/${resource} "</file>/n")
  endforeach()

  set(${GENQRC_FILEPATH_VAR} ${CMAKE_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${GENQRC_FILENAME})

  file(GENERATE
     OUTPUT ${${GENQRC_FILEPATH_VAR}}
     CONTENT
      "<!DOCTYPE RCC><RCC version=\"1.0\">
<qresource>
    ${resources_string}
</qresource>
</RCC>
"
  )

endfunction()
