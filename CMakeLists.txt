# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(OnlineMallSystem VERSION 1.0.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# 启用自动处理MOC、UIC和RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 包含当前目录
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 添加可执行文件
add_executable(OnlineMallSystem
    main.cpp
    mainwindow.cpp
    mainwindow.h
    user.cpp
    user.h
    product.cpp
    product.h
)

# 链接Qt库
target_link_libraries(OnlineMallSystem
    Qt6::Core
    Qt6::Widgets
)

# 添加单元测试（如果存在）
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test_user.cpp")
    add_executable(test_user
        test_user.cpp
        user.cpp
        product.cpp
    )
    target_link_libraries(test_user
        Qt6::Core
        Qt6::Test
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test_product.cpp")
    add_executable(test_product
        test_product.cpp
        product.cpp
    )
    target_link_libraries(test_product
        Qt6::Core
        Qt6::Test
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test_integration.cpp")
    add_executable(test_integration
        test_integration.cpp
        test_integration_main.cpp
        mainwindow.cpp
        user.cpp
        product.cpp
    )
    target_link_libraries(test_integration
        Qt6::Core
        Qt6::Widgets
        Qt6::Test
    )
endif()

# 安装规则（可选）
install(TARGETS OnlineMallSystem
    RUNTIME DESTINATION bin
)