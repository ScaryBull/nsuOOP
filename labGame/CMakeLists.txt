cmake_minimum_required(VERSION 3.10)
project(labGame)

set(CMAKE_CXX_STANDARD 17)

set(SFML_PATH "${CMAKE_SOURCE_DIR}/SFML-3.0.2")

include_directories("${SFML_PATH}/include")
link_directories("${SFML_PATH}/lib")

file(GLOB SOURCES "*.cpp")
add_executable(labGame ${SOURCES}
        model/include/Item.h
        model/include/Ingredient.h
        model/src/Ingredient.cpp
        model/include/Potion.h
        model/src/Item.cpp
        model/src/Potion.cpp
        model/include/Amulet.h
        model/src/Amulet.cpp
        model/include/Order.h
        model/include/AlchemyLevel.h
        model/src/AlchemyLevel.cpp
        view/GameView.cpp
        view/GameView.h
        model/include/GameModel.h
        model/src/GameModel.cpp
        controller/GameController.cpp
        controller/GameController.h
)

target_link_libraries(labGame sfml-graphics sfml-window sfml-system)
