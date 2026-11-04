cmake_minimum_required(VERSION 3.16)
project(Geant4Demo LANGUAGES CXX)

# Para ambientes sem interface grafica:
# cmake -S . -B build -DDEMO_WITH_VIS=OFF
option(DEMO_WITH_VIS "Habilitar interface interativa e visualizacao" ON)

if(DEMO_WITH_VIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})

add_executable(Geant4Demo
  main.cc
  src/DetectorConstruction.cc
  src/PrimaryGeneratorAction.cc
  src/ActionInitialization.cc
  src/RunAction.cc
  src/EventAction.cc
  src/SteppingAction.cc
)

target_include_directories(Geant4Demo PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Geant4Demo PRIVATE ${Geant4_LIBRARIES})
target_compile_features(Geant4Demo PRIVATE cxx_std_17)

if(DEMO_WITH_VIS)
  target_compile_definitions(Geant4Demo PRIVATE DEMO_WITH_VIS)
endif()

# Copia macros para permitir: cd build && ./Geant4Demo run.mac
foreach(_macro IN ITEMS
    run.mac
    run_662keV.mac
    run_1MeV.mac
    run_duas_energias.mac
    test_smoke.mac
    vis.mac)
  configure_file(${PROJECT_SOURCE_DIR}/${_macro}
                 ${PROJECT_BINARY_DIR}/${_macro} COPYONLY)
endforeach()
