macro(print_resource front_message resource end_message)
    message(STATUS "${front_message}")
    foreach(item  ${resource})
        message(STATUS "${item}")
    endforeach()
    message(STATUS "${end_message}")
endmacro()