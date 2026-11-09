# Desafio prático — Geant4 (IME / PPGEN)

**Disciplina:** Simulação Computacional Aplicada à Engenharia Nuclear  
**Duração:** 2 tempos (~100 min)  
**Modalidade:** em duplas ou individual, com consulta livre ao material da aula e à documentação.

## Situação-problema

Foi fornecido o código de uma aplicação Geant4 que simula o transporte de fótons gama através de uma blindagem e a resposta de um detector. Parte dos arquivos foi entregue com nomes genéricos e a aplicação ainda precisa ser organizada.

Vocês deverão **investigar o código**, reconstruir o modelo físico, colocar a aplicação em funcionamento e produzir uma segunda versão com uma geometria modificada.

## Desafios

**1. Reconstruir e compilar.** Identifiquem os arquivos de implementação e cabeçalhos com nomes genéricos, atribuam nomes compatíveis com o projeto e obtenham a compilação da aplicação localizada em `GeometriaOriginal/`.

**2. Reconstruir a geometria.** Antes de utilizar o visualizador, determinem os materiais, dimensões reais e coordenadas dos centros dos volumes, além das características da fonte. Façam um esquema do problema e confiram a interpretação no **modo visual**.

**3. Executar a aplicação.** Demonstrem o funcionamento em modo visual e em modo batch. Identifiquem, no código ou nas macros, quais grandezas físicas são registradas e onde os resultados são salvos.

**4. Criar a segunda geometria.** A pasta `GeometriaModificada/` foi entregue vazia. Organizem nela uma **cópia independente e funcional** da aplicação, modificando a espessura total da placa de **chumbo para 2 cm**, sem alterar a posição de seu centro. Compilem e verifiquem visualmente a nova geometria. Preservem o código da geometria original.

**5. Comparar as simulações.** Produzam resultados batch com **20.000 eventos por configuração** e comparem os três casos:

| Caso | Energia inicial | Espessura de chumbo | Pasta da aplicação |
|---|---:|---:|---|
| A | 662 keV | 1 cm | `GeometriaOriginal` |
| B | 1332 keV (1,332 MeV) | 1 cm | `GeometriaOriginal` |
| C | 1332 keV (1,332 MeV) | 2 cm | `GeometriaModificada` |

Os arquivos dos diferentes casos devem ser preservados, sem sobrescrever os anteriores. É permitido ajustar macros para modificar condições da fonte e nomes de saída. **Nenhuma classe C++ nova é exigida.**

## Entrega

- Aplicações original e modificada organizadas e funcionais;
- Esquema da geometria original com materiais, dimensões e posições;
- Captura de tela do modo visual, demonstrando a nova geometria;
- Arquivos CSV dos scorers e a tabela comparativa gerada pelo script Python;
- Breve texto de um parágrafo com a interpretação física: efeito da energia (**A → B**) e da espessura (**B → C**); significado físico de energia depositada e fluência no detector.

## Condições da atividade

- Consulta livre à documentação e aos materiais disponibilizados;
- Não é necessário implementar novos algoritmos de scoring;
- O modo visual e o modo batch são **obrigatórios**;
- O ambiente Geant4/WSL deve estar configurado antes da aula;
- É permitido esclarecer dúvidas de ambiente de compilação com o professor, mas a identificação dos componentes e a solução do problema são responsabilidade da dupla.
