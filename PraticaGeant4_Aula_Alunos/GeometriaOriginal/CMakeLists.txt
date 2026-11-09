cmake_minimum_required(VERSION 3.16)
project(PraticaGeant4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# O modo visual com Qt e obrigatorio nesta pratica.

find_package(Geant4 REQUIRED qt)
include(${Geant4_USE_FILE})

# Arquivos explicitados para a atividade de identificacao.
add_executable(PraticaGeant4
    main.cc
    src/DetectorConstruction.cc
    src/PrimaryGeneratorAction.cc
    src/ActionInitialization.cc
)

target_include_directories(PraticaGeant4 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(PraticaGeant4 PRIVATE ${Geant4_LIBRARIES})

# Permite usar macros/ a partir da pasta do build; recomendado
# executar o programa a partir da raiz do projeto.
file(COPY ${PROJECT_SOURCE_DIR}/macros DESTINATION ${PROJECT_BINARY_DIR})
