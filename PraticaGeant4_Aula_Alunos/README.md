# Prática de Geant4 — IME / PPGEN

**Disciplina:** Simulação Computacional Aplicada à Engenharia Nuclear  
**Atividade:** investigação, compilação, execução e adaptação de uma aplicação Geant4.

Leia [`ENUNCIADO_ALUNOS.md`](ENUNCIADO_ALUNOS.md) antes de iniciar. O desafio foi planejado para ser realizado **com autonomia**. O código entregue não está totalmente organizado.

## Organização do material

```text
PraticaGeant4_Aula_Alunos/
├── ENUNCIADO_ALUNOS.md
├── README.md
├── GeometriaOriginal/
│   ├── main.cc
│   ├── CMakeLists.txt
│   ├── include/
│   ├── src/
│   ├── macros/
│   └── resultados/
├── GeometriaModificada/      # Vazia inicialmente
└── tools/
    └── comparar_scores_geant4.py
```

A pasta `GeometriaModificada/` contém somente `.gitkeep`, um arquivo oculto vazio para mantê-la no Git. **Não há código-fonte pronto nessa pasta.**

## Ambiente necessário

- WSL 2 / Ubuntu com compilador C++ e CMake;
- Geant4 instalado com seus dados físicos e **suporte a Qt e OpenGL**;
- WSLg ou ambiente gráfico equivalente para abrir janelas Qt;
- Python 3 para a análise (somente biblioteca padrão, sem instalação de pacotes adicionais).

O CMake exige Qt; não há opção de desativar o modo visual.

## Referência de comandos

Na **raiz da aplicação** que deseja compilar (não na raiz do pacote):

```bash
source ~/Geant4/geant4-install/bin/geant4.sh
cmake -S . -B build -DCMAKE_PREFIX_PATH="$HOME/Geant4/geant4-install"
cmake --build build --parallel 2
```

O caminho usado em `source` e `CMAKE_PREFIX_PATH` deve corresponder à instalação real do Geant4 no computador.

Para executar na **pasta da aplicação**:

```bash
./build/PraticaGeant4                  # Modo visual interativo
./build/PraticaGeant4 macros/teste.mac # Teste batch
./build/PraticaGeant4 macros/run.mac   # Simulação batch
```

**Importante:** a reconstrução da organização dos arquivos faz parte da atividade; é esperado que a aplicação ainda não compile antes dessa etapa. Para observar trajetórias no modo visual, pode ser necessário iniciar eventos pela interface. Execute os comandos acima a partir da pasta da aplicação correspondente, pois as macros usam caminhos relativos.

Ao criar uma segunda aplicação, faça uma **nova compilação**. Não copie `build/` da aplicação original: o cache do CMake registra caminhos absolutos do projeto de origem.

## Resultados e comparação

Cada geometria deve guardar os próprios arquivos `*_edep.csv` e `*_fluxo.csv` em sua pasta `resultados/`. Para cada condição simulada, mantenha o **mesmo prefixo** nos dois arquivos (exemplo genérico: `caso1_edep.csv` e `caso1_fluxo.csv`). Preserve os resultados de execuções distintas, sem sobrescrevê-los.

Após concluir as execuções, o comando abaixo pode ser utilizado **de qualquer diretório**, pois o Python localiza as duas geometrias com base na posição do próprio arquivo:

```bash
python3 tools/comparar_scores_geant4.py
```

O exemplo acima pressupõe que o terminal está na pasta principal do pacote. Para informar outro número de eventos por configuração:

```bash
python3 tools/comparar_scores_geant4.py --eventos 20000
```

O script ignora, por padrão, os pares cujos nomes começam por `teste`, `test` ou `smoke`. Produz `comparacao_scores.csv` e dois gráficos vetoriais SVG na raiz deste pacote.

**Limitação:** os CSVs de scoring não informam automaticamente a energia, a espessura da blindagem nem a quantidade de eventos simulados. O script **assume** o número informado na opção `--eventos` (20.000 por padrão). Compare apenas execuções cujo número de eventos esteja correto. Ele também não calcula incertezas estatísticas a partir de `total(val^2)`.

## Links para consulta

- [Geant4 — Book for Application Developers](https://geant4.web.cern.ch/documentation/dev/bfad_html/)
- [Command-based scoring](https://geant4.web.cern.ch/documentation/dev/bfad_html/ForApplicationDevelopers/Detector/commandScore.html)
- [Geant4 — Guia de instalação](https://geant4.web.cern.ch/documentation/dev/ig_html/)
