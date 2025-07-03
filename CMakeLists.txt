cmake_minimum_required(VERSION 3.28)
project(qt_programming)

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
        src/Items/Weapon/Weapon.h          #
        src/Items/Weapon/Weapon.cpp        #
        src/Items/Weapon/Fist.h            #
        src/Items/Weapon/Fist.cpp          #
        src/Items/FloatingText.h
        src/Items/FloatingText.cpp         #
        src/Items/Armors/FlamebreakerArmor.cpp
        src/Items/Armors/FlamebreakerArmor.h
        src/Items/Weapon/Knife.cpp
        src/Items/Weapon/Knife.h
        # --- 新增Buff文件 ---
        src/Items/Buff/Speed.cpp
        src/Items/Buff/Speed.h
        src/Items/Buff/Health.cpp
        src/Items/Buff/Health.h
        # --- 新增医疗品文件 ---
        src/Items/Medicine/Medicine.cpp
        src/Items/Medicine/Medicine.h
        src/Items/Medicine/Bandage.cpp
        src/Items/Medicine/Bandage.h
        src/Items/Medicine/Kit.cpp
        src/Items/Medicine/Kit.h
        src/Items/Medicine/Adrenaline.cpp
        src/Items/Medicine/Adrenaline.h
)
target_link_libraries(qt_programming_2024
        Qt::Core
        Qt::Gui
        Qt::Widgets
)