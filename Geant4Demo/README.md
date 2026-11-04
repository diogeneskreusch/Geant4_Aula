# Geant4Demo — Transporte de fótons gama em blindagem de chumbo

**Disciplina:** Simulação Computacional Aplicada à Engenharia Nuclear  
**Instituição:** Instituto Militar de Engenharia (IME) — PPGEN  
**Elaboração:** Diogenes Kreusch Filho

[← Página principal do repositório](../README.md) · [Roteiro da atividade](./ROTEIRO_ALUNOS.md) · [Guia de instalação](../Guia_Instalacao_Geant4_WSL_Alunos_IME_PPGEN.pdf)

## 1. Objetivo da demonstração

Esta aplicação didática em **C++/Geant4** simula a emissão de fótons gama de uma fonte pontual e monodirecional, seu transporte através de uma placa de chumbo e sua interação com um detector de silício.

A atividade permite reconhecer os componentes da arquitetura Geant4 e investigar o efeito de dois parâmetros:

- **Energia do fóton:** 662 keV ou 1 MeV;
- **Espessura da blindagem:** 1 cm ou 3 cm de Pb.

São analisadas a fração de eventos em que **pelo menos um fóton gama entra no detector** e a **energia média depositada no silício por evento**, ambas acompanhadas de suas incertezas estatísticas.

## 2. Configuração física

| Item | Configuração |
| --- | --- |
| Fonte | Um fóton gama primário por evento; posição `(0, 0, -10 cm)`; direção `+z` |
| Energias | **662 keV** e **1 MeV** |
| Blindagem | Chumbo (`G4_Pb`); seção de **6 × 6 cm²**; centro em `z = 0` |
| Espessura inicial | **1 cm**; modificável para **3 cm** no código C++ |
| Detector | Silício (`G4_Si`); **6 × 6 cm²**, espessura **1 mm**; centro em `z = +8 cm` |
| World | Cubo de **40 cm de lado**, preenchido com ar (`G4_AIR`) |
| Física | Lista de referência **FTFP_BERT** |
| Execução | **Sequencial** (um thread), por simplicidade didática |
| Eventos | **20.000** por configuração na campanha; **100** no teste rápido |

A fonte representa um **feixe estreito (*pencil beam*)**, e não um feixe amplo.

## 3. Arquivos da aplicação

```text
Geant4Demo/
├── README.md
├── ROTEIRO_ALUNOS.md
├── CMakeLists.txt
├── main.cc
├── include/                   # Declarações das classes Geant4 (.hh)
├── src/                       # Implementação das classes (.cc)
├── tools/
│   └── analisar_resultados.py
├── test_smoke.mac             # Teste de 100 eventos
├── run.mac                    # Referência: 662 keV, 20.000 eventos
├── run_662keV.mac             # Apenas 662 keV
├── run_1MeV.mac               # Apenas 1 MeV
├── run_duas_energias.mac      # Dois runs com energias diferentes
└── vis.mac                    # Visualização de trajetórias
```

| Arquivo ou classe | Responsabilidade |
| --- | --- |
| `main.cc` | Criação do `G4RunManager` e registro de geometria, física e ações |
| `DetectorConstruction` | Materiais, volumes e espessura do chumbo |
| `PrimaryGeneratorAction` | Gerador de fótons (`G4ParticleGun`) |
| `ActionInitialization` | Registro das ações do usuário |
| `SteppingAction` | Identificação da entrada de fótons e energia depositada nos passos |
| `EventAction` | Consolidação e contagem de resultados por evento |
| `RunAction` | Acumulação, incertezas e gravação do CSV |

## 4. Pré-requisitos

- Ubuntu (preferencialmente no **WSL 2**) ou Linux compatível;
- **Geant4 11.x** instalado e configurado no terminal;
- Compilador com suporte a **C++17** e **CMake 3.16+**;
- Para gráficos, **Python 3** e **Matplotlib**;
- Para visualização, Geant4 compilado com suporte a Qt/OpenGL e ambiente gráfico funcional.

Para a instalação completa, consulte o [guia da disciplina](../Guia_Instalacao_Geant4_WSL_Alunos_IME_PPGEN_Revisado.pdf). **Não é necessário instalar o toolkit dentro desta pasta**: o Geant4 já instalado será usado pelo CMake.

## 5. Obter e compilar o programa

Se ainda não clonou o repositório, copie sua URL HTTPS na opção **Code** do GitHub e execute no terminal Ubuntu, substituindo o campo indicado:

```bash
git clone URL_HTTPS_DO_REPOSITORIO repositorio-geant4
cd repositorio-geant4/Geant4Demo
```

Se já clonou o repositório, basta entrar na pasta `Geant4Demo`, que contém o `CMakeLists.txt`.

### 5.1. Carregar o ambiente Geant4

Se o ambiente não foi ativado automaticamente pelo `~/.bashrc`:

```bash
source "$HOME/Geant4/geant4-install/bin/geant4.sh"
geant4-config --version
```

Ajuste o caminho acima caso o toolkit tenha sido instalado em outro local.

### 5.2. Configurar e compilar

**Execute os próximos comandos a partir da pasta `Geant4Demo/`, não da raiz do repositório.**

```bash
cmake -S . -B build \
  -DCMAKE_PREFIX_PATH="$HOME/Geant4/geant4-install" \
  -DDEMO_WITH_VIS=ON
cmake --build build --parallel 2
```

Em ambiente sem interface gráfica, substitua `-DDEMO_WITH_VIS=ON` por `-DDEMO_WITH_VIS=OFF`.

Ao final, o executável esperado é `build/Geant4Demo`.

## 6. Teste inicial (100 eventos)

```bash
./build/Geant4Demo test_smoke.mac
```

Esse teste deve mostrar um resumo numérico no terminal e criar o arquivo `resultados.csv` **na pasta atual**. Antes de realizar a campanha com 20.000 eventos, preserve ou renomeie o resultado do teste:

```bash
mv resultados.csv resultados_teste.csv
```

> **Validação:** o projeto foi preparado para fins didáticos; a compilação e a execução real ainda devem ser verificadas na instalação Geant4 utilizada na disciplina.

## 7. Campanha com quatro configurações

| Configuração | Energia | Pb | Eventos |
| --- | ---: | ---: | ---: |
| A | 662 keV | 1 cm | 20.000 |
| B | 1 MeV | 1 cm | 20.000 |
| C | 662 keV | 3 cm | 20.000 |
| D | 1 MeV | 3 cm | 20.000 |

**Parte A — geometria inicial (1 cm):**

```bash
./build/Geant4Demo run_duas_energias.mac
```

**Parte B — geometria modificada (3 cm):**

1. Abra `src/DetectorConstruction.cc`.
2. Encontre `fLeadThickness = 1.0 * cm;`.
3. Substitua por `fLeadThickness = 3.0 * cm;` e salve o arquivo.
4. Recompile **a aplicação**, sem recompilar o Geant4, e execute:

```bash
cmake --build build --parallel 2
./build/Geant4Demo run_duas_energias.mac
```

O arquivo `resultados.csv` é **acumulativo**. Se já houver execuções anteriores, renomeie-o antes de iniciar uma nova campanha para evitar misturar resultados.

## 8. Visualização da geometria e das trajetórias

Com Qt/OpenGL habilitado:

```bash
./build/Geant4Demo
```

O programa utiliza `vis.mac` e executa dez eventos para exibição das trajetórias. Se a visualização não estiver disponível, utilize os arquivos `.mac` em **modo lote**; o estudo numérico não depende da interface gráfica.

## 9. Significado dos resultados

**Fração de eventos com chegada de fótons:**

\[
\hat f = \frac{N_{\text{eventos com chegada de }\gamma}}{N_{\text{eventos}}}
\]

Cada evento conta no máximo uma vez, mesmo que mais de um fóton entre no detector. Fótons espalhados podem ser incluídos. A fração **não equivale necessariamente à transmissão não colidida** \(e^{-\mu x}\).

**Energia média depositada por evento:**

\[
\overline{E}_{\mathrm{dep}} = \frac{1}{N}\sum_{i=1}^{N} E_{\mathrm{dep},i}
\]

A energia depositada é acumulada pelos passos de todas as partículas no silício. Eventos sem deposição são incluídos com valor zero. **Entrada de um fóton no detector não significa necessariamente deposição de energia**.

O programa apresenta, para ambas as grandezas, estimativas de incerteza estatística de **1 desvio-padrão (1σ)**.

## 10. Organizar dados e gerar gráficos

Para imprimir a tabela de resultados:

```bash
python3 tools/analisar_resultados.py resultados.csv
```

Para gerar também os gráficos (requer Matplotlib):

```bash
sudo apt install -y python3-matplotlib
python3 tools/analisar_resultados.py resultados.csv --output figuras
```

Os arquivos gerados serão `figuras/fracao_chegada.png` e `figuras/energia_depositada.png`. Se existirem configurações repetidas no CSV, o script utiliza a **última ocorrência de cada par energia–espessura**.

## 11. Roteiro da atividade e problemas frequentes

Consulte o [roteiro dos alunos](./ROTEIRO_ALUNOS.md) para as questões e entregas propostas.

- **`Geant4Config.cmake` não encontrado:** confira a instalação e o valor de `CMAKE_PREFIX_PATH`.
- **Macro não encontrada:** execute os comandos na pasta `Geant4Demo/` ou entre em `build/` e rode `./Geant4Demo nome.mac`.
- **Erro de interface gráfica:** reconfigure com `-DDEMO_WITH_VIS=OFF` e compile novamente.
- **Erro SSL ao clonar:** consulte a seção de certificados HTTPS no guia de instalação; não desative a verificação de certificados do Git.

## 12. Referências

- [Geant4 — documentação oficial](https://geant4.web.cern.ch/documentation/)
- [Geant4 — exemplos básicos](https://github.com/Geant4/geant4/tree/master/examples/basic)

---

[← Voltar à página principal](../README.md)
