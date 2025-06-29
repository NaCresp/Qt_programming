cmake_minimum_required(VERSION 3.28)
project(qt_programming_2024)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(qt_programming_2024 src/main.cpp
        src/MyGame.cpp
        src/MyGame.h
        src/Items/Item.cpp
        src/Items/Item.h
        src/Items/Characters/Character.cpp
        src/Items/Characters/Character.h
        src/Items/Characters/Link.cpp
        src/Items/Characters/Link.h
        src/Items/Armors/Armor.cpp
        src/Items/Armors/Armor.h
        src/Items/HeadEquipments/HeadEquipment.cpp
        src/Items/HeadEquipments/HeadEquipment.h
        src/Items/LegEquipments/LegEquipment.cpp
        src/Items/LegEquipments/LegEquipment.h
        src/Items/HeadEquipments/Head.cpp
        src/Items/HeadEquipments/Head.h
        src/Items/Armors/body.cpp
        src/Items/Armors/body.h
        src/Items/LegEquipments/Leg.cpp
        src/Items/LegEquipments/Leg.h
        src/Scenes/Scene.cpp
        src/Scenes/Scene.h
        src/Scenes/BattleScene.cpp
        src/Scenes/BattleScene.h
        src/Scenes/StartMenuScene.cpp      #
        src/Scenes/StartMenuScene.h        #
        assets/assets.qrc
        src/Items/Maps/Map.cpp
        src/Items/Maps/Map.h
        src/Items/Maps/Battlefield.cpp
        src/Items/Maps/Battlefield.h
        src/Items/Mountable.cpp
        src/Items/Mountable.h
        src/Items/Armors/FlamebreakerArmor.cpp
        src/Items/Armors/FlamebreakerArmor.h)
target_link_libraries(qt_programming_2024
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

