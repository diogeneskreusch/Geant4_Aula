# Geant4Demo — Aula de Simulação Computacional em Engenharia Nuclear

## Objetivo

Aplicação didática **completa em C++** para demonstrar o transporte de fótons gama
através de uma placa de chumbo e a resposta de um detector de silício.

Foi preparada para alunos que já conhecem o básico de MCNP5. Permite identificar
`DetectorConstruction`, `PrimaryGeneratorAction`, `ActionInitialization`,
`SteppingAction`, `EventAction`, `RunAction`, `main.cc`, CMake e macros.

**Geometria padrão**

| Componente | Definição |
| --- | --- |
| World | cubo de 40 cm de lado, ar (`G4_AIR`) |
| Fonte | fóton gama pontual, `z = -10 cm`, direção `+z` |
| Blindagem | Pb (`G4_Pb`), 6 × 6 cm, centro em `z = 0` |
| Espessura inicial | **1 cm** (depois alterar para **3 cm**) |
| Detector | Si (`G4_Si`), 6 × 6 cm e espessura de 1 mm, centro em `z = 8 cm` |
| Física | lista de referência `FTFP_BERT` |
| Eventos | 20.000 por configuração; 1 fóton primário/evento |
| Execução | **sequencial** (intencional, para simplificar a primeira aula) |

A emissão ocorre em um único ponto com uma única direção: **não é um feixe amplo**.

## Pré-requisitos

- Geant4 instalado e configurado, preferencialmente uma versão da série **11.x**.
- Compilador C++ com suporte a C++17.
- CMake (3.16 ou superior).
- Para visualização: Geant4 com suporte a UI/visualização e ambiente gráfico
  (por exemplo, OpenGL/Qt). O modo em lote **não precisa de janela gráfica**.
- Opcional: Python 3 e `matplotlib` para os gráficos.

**Observação:** estes arquivos foram preparados com base nas APIs documentadas do
Geant4 e em seus exemplos oficiais. **Não foi possível compilá-los ou executá-los
neste ambiente, pois o toolkit Geant4 não está instalado aqui.** Recomenda-se executar
primeiro `test_smoke.mac` no computador que será utilizado na aula.

## Compilação — Linux ou WSL

Se o Geant4 já estiver configurado no terminal:

```bash
cd Geant4Demo_Aula
cmake -S . -B build
cmake --build build --parallel 4
```

Caso CMake não encontre o Geant4, informe a pasta que contém `Geant4Config.cmake`:

```bash
cmake -S . -B build -DGeant4_DIR=/caminho/para/lib/cmake/Geant4
```

Em um ambiente sem interface gráfica, use:

```bash
cmake -S . -B build -DDEMO_WITH_VIS=OFF
cmake --build build --parallel 4
```

A primeira configuração do projeto com `DEMO_WITH_VIS=ON` exige bibliotecas de
UI e de visualização, como nos exemplos oficiais. Para alternar entre os modos,
limpe ou reconfigure o diretório `build` com o valor desejado.

O CMake também copia os arquivos `.mac` para `build/`, permitindo rodar a partir
desse diretório. O CSV é criado no **diretório de trabalho atual**.

## Execução — modo em lote

A partir da raiz do projeto:

```bash
# 100 eventos: teste inicial
./build/Geant4Demo test_smoke.mac

# 662 keV, 20.000 eventos, geometria compilada
./build/Geant4Demo run.mac

# Dois runs, 662 keV e 1 MeV, cada um com 20.000 eventos
./build/Geant4Demo run_duas_energias.mac
```

Também é possível executar:

```bash
cd build
./Geant4Demo run_duas_energias.mac
```

O programa utiliza `/run/initialize` **dentro das macros**. Não é necessário
colocar outra inicialização em `main.cc`.

## Execução — visualização

Em uma instalação com suporte gráfico, a partir da raiz do projeto:

```bash
./build/Geant4Demo
```

O programa abrirá `vis.mac` e tentará utilizar o driver `OGL`. Serão simulados
**10 eventos** para visualização das trajetórias. Se o driver `OGL` não estiver
disponível, altere a linha `/vis/open OGL` para um driver suportado ou execute
apenas o modo em lote.

Não há referência a `init_vis.mac`; o macro padrão para esta aplicação é
`vis.mac`, evitando a falha "Can not open init_vis.mac".

## Sequência recomendada das quatro configurações

Não é preciso recompilar para alterar a energia. Há necessidade de recompilar
para alterar a espessura, porque **nesta versão didática** ela foi fixada no C++.

**Parte A — Pb com 1 cm (geometria inicial)**

```bash
./build/Geant4Demo run_duas_energias.mac
```

**Parte B — Pb com 3 cm**

Abra `src/DetectorConstruction.cc`, localize:

```cpp
fLeadThickness = 1.0 * cm;
```

Substitua por:

```cpp
fLeadThickness = 3.0 * cm;
```

Recompile e execute novamente:

```bash
cmake --build build --parallel 4
./build/Geant4Demo run_duas_energias.mac
```

A execução das Partes A e B produzirá quatro linhas de dados, uma para cada
combinação de energia (662 e 1000 keV) e espessura (1 e 3 cm), no arquivo
`resultados.csv`.

**Importante:** evite executar `run.mac` e `run_duas_energias.mac` na mesma campanha
sem saber que o arquivo CSV é acumulativo. Para reiniciar a coleta, renomeie
ou remova **somente o arquivo `resultados.csv`**, preservando o código-fonte.

## Qual é a definição exata das grandezas?

### 1. Fração de eventos com chegada de fótons ao Si

- A `SteppingAction` identifica um *step* de **fóton gama** cujo pré-ponto é a
  fronteira de entrada no volume lógico do detector.
- `EventAction` registra um **booleano por evento**. Mesmo que dois fótons
  alcancem o detector, o evento é contado **uma única vez**.
- São aceitos fótons primários e secundários, incluindo fótons espalhados.
- Isto **não é** a transmissão de fótons não colididos `exp(-mu*x)`.
- Trata-se de **chegada geométrica**, não de um sinal eletrônico medido.

Fração estimada:

`f = N_eventos_com_gamma / N_eventos`

Incerteza-padrão aproximada (modelo binomial):

`sigma_f = sqrt(f*(1-f)/N)`.

Para frações próximas de 0 ou 1, recomenda-se usar intervalos de confiança
adequados (por exemplo, Wilson) em vez de interpretar apenas o erro normal.

### 2. Energia média depositada no Si por evento

- São somadas as energias depositadas em **todos os steps**, de todas as
  partículas, ocorridos no volume lógico do silício.
- Eventos sem deposição contribuem com **zero** para a média.
- `EventAction` repassa a soma de cada evento a `RunAction`.

`Edep_media = sum(Edep_evento)/N`

O erro-padrão da média é calculado como `s/sqrt(N)`, com `s` sendo o desvio-
padrão amostral dos valores **por evento**.

Não confunda **entrada de um fóton** com **energia depositada**: um fóton pode
atravessar o Si sem qualquer interação.

## Resultados gravados em CSV

O programa mostra os resultados no terminal e acrescenta uma linha por run em
`resultados.csv`, com as colunas:

```text
run_id,energia_keV,espessura_Pb_cm,eventos,eventos_com_gamma,
fracao_gamma,erro_fracao_1sigma,edep_media_keV,erro_edep_media_keV,
soma_edep_keV,soma_edep_quadrado_keV2
```

Cada execução cria seu próprio conjunto de runs (`run_id` pode recomeçar em 0).
Para diferenciar configurações, use conjuntamente energia e espessura.

### Análise e gráficos

```bash
python3 tools/analisar_resultados.py resultados.csv
```

A tabela no terminal não exige bibliotecas externas. Para gerar os gráficos:

```bash
python3 -m pip install matplotlib
python3 tools/analisar_resultados.py resultados.csv --output figuras
```

Serão criados `figuras/fracao_chegada.png` e
`figuras/energia_depositada.png`.

O script escolhe a **última execução** para cada par (energia, espessura),
sem combinar automaticamente runs repetidos: executar duas vezes com as mesmas
sementes pode produzir histórias correlacionadas ou idênticas.

## Organização dos arquivos

```text
Geant4Demo_Aula/
├── main.cc
├── CMakeLists.txt
├── include/
│   ├── DetectorConstruction.hh
│   ├── PrimaryGeneratorAction.hh
│   ├── ActionInitialization.hh
│   ├── RunAction.hh
│   ├── EventAction.hh
│   └── SteppingAction.hh
├── src/
│   ├── DetectorConstruction.cc
│   ├── PrimaryGeneratorAction.cc
│   ├── ActionInitialization.cc
│   ├── RunAction.cc
│   ├── EventAction.cc
│   └── SteppingAction.cc
├── run.mac
├── run_662keV.mac
├── run_1MeV.mac
├── run_duas_energias.mac
├── test_smoke.mac
├── vis.mac
├── tools/analisar_resultados.py
├── ROTEIRO_ALUNOS.md
└── NOTAS_DOCENTE.md
```

## Solução de problemas

| Sintoma | Possível causa e providência |
| --- | --- |
| CMake não encontra Geant4 | Carregue o ambiente do Geant4 ou informe `Geant4_DIR`. |
| Faltam bibliotecas Qt/OpenGL | Configure com `-DDEMO_WITH_VIS=OFF` para modo em lote. |
| `Cannot open vis.mac` | Execute a partir da raiz do projeto ou de `build/`; verifique o diretório de trabalho. |
| Erro em `/vis/open OGL` | Falta de driver gráfico ou `DISPLAY`; escolha outro driver ou execute `run.mac`. |
| `resultados.csv` não aparece | O arquivo é gravado no **diretório de trabalho**, não necessariamente junto do executável. |
| Mudança de espessura não aparece | Altere `fLeadThickness`, recompile e confirme `Espessura Pb` no terminal. |
| Resultados não se repetem exatamente | A simulação Monte Carlo depende das sementes e configurações de execução. |
| Resultado inesperado | Confira unidades, espessura, fonte, macro, física e critério de scoring. |

## Referências oficiais

- [Geant4 — Book for Application Developers](https://geant4.web.cern.ch/documentation/)
- [Geant4 — Exemplo básico B1](https://github.com/Geant4/geant4/tree/master/examples/basic/B1)
- [Geant4 — Exemplos básicos B4 (scoring e análise)](https://github.com/Geant4/geant4/tree/master/examples/basic/B4)
