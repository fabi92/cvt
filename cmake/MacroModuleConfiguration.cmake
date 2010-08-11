# Macro for configuring a library module (sublibrary)
MACRO(MODULE_CONFIGURATION
	MODULE_NAME
	MODULE_HEADERS
	MODULE_SOURCES
	LINKER_DEPENDENCIES
	)
	
	MESSAGE(STATUS "Configuring module: ${MODULE_NAME}")

	ADD_LIBRARY("${MODULE_NAME}"  ${MODULE_HEADERS} ${MODULE_SOURCES})
	
	TARGET_LINK_LIBRARIES("${MODULE_NAME}" ${LINKER_DEPENDENCIES})
	
	SET_TARGET_PROPERTIES("${MODULE_NAME}" PROPERTIES VERSION ${LIB_VERSION})
	
	SET_TARGET_PROPERTIES("${MODULE_NAME}" PROPERTIES DEBUG_POSTFIX "${LIBRARY_POSTFIX_DEBUG}")	
	
#	SET_TARGET_PROPERTIES("${MODULE_NAME}" PROPERTIES FRAMEWORK TRUE)	

	#PREFIX hack for needed for Xcode and MVSC Projects
	IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		SET_TARGET_PROPERTIES("${MODULE_NAME}" PROPERTIES PREFIX "../")	
	ENDIF()

	INSTALL(TARGETS "${MODULE_NAME}"
	  		ARCHIVE DESTINATION lib
			LIBRARY DESTINATION lib
			RUNTIME DESTINATION bin
#FRAMEWORK DESTINATION lib
			COMPONENT "${MODULE_NAME}"
	)

	GET_FILENAME_COMPONENT(folderName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
	#install header
	foreach(HEADER_FILE ${MODULE_HEADERS})
		GET_FILENAME_COMPONENT(pathName "${HEADER_FILE}" PATH)
		INSTALL(FILES "${HEADER_FILE}" 
				DESTINATION "include/visionTools/${folderName}/${pathName}" 
				COMPONENT headers
		)
	endforeach(HEADER_FILE)
ENDMACRO(MODULE_CONFIGURATION)
