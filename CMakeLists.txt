
file(GLOB SOURCES "*.cc")
#set (SOURCES Matrix.cc Matrix_Base.cc Matrix_Data.cc Matrix_Data_Allocator.cc Matrix_Traits.cc)
IF (WIN32)
add_library(Core_Matrix STATIC ${SOURCES} ${MKL_Libs} )
ELSE (WIN32)
  add_library(Core_Matrix SHARED ${SOURCES} )
ENDIF (WIN32)
	link_directories( ${Boost_Root}  ${LibPath} ${HPX_Root}/lib ${MKL_Path})
	target_link_libraries (Core_Matrix  ${MKL_Libs}  )
	
target_compile_definitions(
Core_Matrix PRIVATE "_CRT_SECURE_NO_WARNINGS"
Core_Matrix PRIVATE "FX_EXPORTS"
Core_Matrix PRIVATE "MKL_ILP64"
)

set (TEST_SOURCES ${TEST_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/test/test.matrix.cc ${CMAKE_CURRENT_SOURCE_DIR}/test/test.matrix.iterator.cc)
set (TEST_SOURCES ${TEST_SOURCES} PARENT_SCOPE)

