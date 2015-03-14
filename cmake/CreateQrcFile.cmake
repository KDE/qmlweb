
function(generate_qrc_file)
  set(oneValueArgs FILENAME)
  set(multiValueArgs RESOURCES)

  cmake_parse_arguments("" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  foreach(resource ${RESOURCES})
    string(CONCAT "    <file>" ${resources_string} "</file>/n" resources_string)
  endforeach()

    file(GENERATE
     OUTPUT ${FILENAME}
     CONTENT
     "<!DOCTYPE RCC><RCC version="1.0">
<qresource>
    ${resources_string}
</qresource>
</RCC>
"
  )

endfunction()
