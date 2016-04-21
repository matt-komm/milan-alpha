macro(copy_file_if_changed in_file out_file target)  
	add_custom_command (
		TARGET     ${target}
		POST_BUILD
		COMMAND    ${CMAKE_COMMAND}
		ARGS       -E copy_if_different ${in_file} ${out_file}
		COMMENT "Copying file: ${in_file} to: ${out_file}"
	)
endmacro(copy_file_if_changed)
