# Roteiro de prática — Simulação Geant4 de blindagem gama

**Disciplina:** Simulação Computacional em Engenharia Nuclear

**Problema:** avaliar como a energia dos fótons e a espessura de uma placa de
chumbo influenciam (i) a fração de eventos com chegada de fótons ao detector e
(ii) a energia média depositada no silício.

## Etapa 1 — Reconhecimento da aplicação

1. Abra a pasta `Geant4Demo_Aula` no editor.
2. Localize `main.cc`, `DetectorConstruction.cc`, `PrimaryGeneratorAction.cc`,
   `ActionInitialization.cc`, `SteppingAction.cc`, `EventAction.cc` e `RunAction.cc`.
3. Identifique onde se encontram a geometria, os materiais, a fonte e o
   registro dos resultados.
4. Confira as unidades de dimensões e energia.

## Etapa 2 — Compilação e teste

No terminal, a partir da pasta do projeto:

```bash
cmake -S . -B build
cmake --build build --parallel 4
./build/Geant4Demo test_smoke.mac
```

Se a instalação não tiver suporte gráfico, use
`cmake -S . -B build -DDEMO_WITH_VIS=OFF` na primeira linha.

**Verificação:** devem aparecer 100 eventos e um resumo final com fração de
chegada e energia depositada. O CSV é criado na pasta em que o comando é
executado.

## Etapa 3 — Campanha de simulações

Considere todas as combinações de:

- Energias: **662 keV e 1 MeV**;
- Espessuras de Pb: **1 cm e 3 cm**;
- **20.000 eventos por configuração**;
- Mesma fonte, mesma área do Pb, mesma posição do detector e mesma Physics List.

Com a geometria inicial de 1 cm, execute:

```bash
./build/Geant4Demo run_duas_energias.mac
```

Em seguida, altere em `src/DetectorConstruction.cc`:

```cpp
fLeadThickness = 1.0 * cm;
```

para:

```cpp
fLeadThickness = 3.0 * cm;
```

Recompile e repita a execução:

```bash
cmake --build build --parallel 4
./build/Geant4Demo run_duas_energias.mac
```

**Dica:** se executou testes antes da campanha, renomeie `resultados.csv` para
manter somente os quatro runs de interesse.

## Etapa 4 — Registro e análise

Preencha a tabela com dados **medidos na simulação**:

| Energia | Pb | Eventos com chegada (N) | Fração de chegada ± 1σ | Edep média (keV/evento) ± 1σ |
| --- | --- | ---: | ---: | ---: |
| 662 keV | 1 cm | | | |
| 662 keV | 3 cm | | | |
| 1 MeV | 1 cm | | | |
| 1 MeV | 3 cm | | | |

Para gerar a tabela no terminal e, se disponível, dois gráficos:

```bash
python3 tools/analisar_resultados.py resultados.csv
```

## Etapa 5 — Perguntas para discussão

1. Mantida a energia, como a espessura de Pb afeta a chegada de fótons?
2. Mantida a espessura, como a energia primária altera o resultado?
3. A fração de eventos com chegada representa transmissão **não colidida**?
   Justifique considerando o espalhamento.
4. Um fóton pode entrar no detector de Si sem depositar energia? O que isso
   significa para a definição de "detecção"?
5. Por que a energia média depositada por evento não pode ser substituída
   pela energia média apenas dos eventos com deposição positiva?
6. Se o número de histórias for quadruplicado, como deve variar a incerteza
   estatística, aproximadamente?
7. Em que ponto do código seria necessário intervir para medir somente os
   fótons primários não espalhados? Quais informações adicionais seriam úteis?

## Entrega sugerida

- Tabela com os quatro resultados e respectivas incertezas.
- Dois gráficos (chegada e energia depositada) ou descrição comparativa.
- Um parágrafo discutindo os efeitos de energia e espessura, incluindo
  uma limitação do modelo.

**Atenção:** a chegada de um fóton ao volume do detector é um critério
geométrico, não uma garantia de sinal físico mensurável.
