<div align="center">
  <img src="./ime.png" alt="Brasão do Instituto Militar de Engenharia" height="115">
  &nbsp;&nbsp;&nbsp;&nbsp;
  <img src="./ppgen.png" alt="Logotipo do Programa de Pós-Graduação em Engenharia Nuclear" height="115">

  # Simulação Computacional Aplicada à Engenharia Nuclear

  **Instituto Militar de Engenharia (IME)**  
  **Programa de Pós-Graduação em Engenharia Nuclear (PPGEN)**

  Materiais didáticos e atividades práticas de simulação computacional.
</div>

## Apresentação

Este repositório reúne materiais de apoio à disciplina **Simulação Computacional Aplicada à Engenharia Nuclear**, oferecida no âmbito do **PPGEN/IME**. Seu propósito é apoiar a compreensão e a aplicação de métodos computacionais na modelagem do transporte de partículas e das interações da radiação com a matéria.

Os materiais estão organizados de modo que cada atividade possua seu próprio código, documentação e instruções de execução. Os conceitos de Monte Carlo abordados na disciplina podem ser estudados com diferentes ferramentas, incluindo **MCNP5** e **Geant4**, conforme as características do problema.

## Materiais disponíveis

| Material | Descrição |
| --- | --- |
| [Guia de instalação do Geant4 no Windows/WSL](./Guia_Instalacao_Geant4_WSL_Alunos_IME_PPGEN.pdf) | Configuração do WSL 2 e do Ubuntu 24.04 LTS, instalação do Geant4 a partir do código-fonte e primeiros testes. |
| [Geant4Demo — demonstração prática](./Geant4Demo/) | Projeto C++ de transporte de fótons gama por uma blindagem de chumbo até um detector de silício. |
| [README da demonstração](./Geant4Demo/README.md) | Instruções de compilação, execução, visualização e análise dos resultados. |
| [Roteiro dos alunos](./Geant4Demo/ROTEIRO_ALUNOS.md) | Atividade orientada, exercícios e questões para discussão. |

## Como começar

1. Se ainda não possui o ambiente de simulação, comece pelo **guia de instalação**.
2. Após instalar o Geant4, acesse a pasta **[Geant4Demo](./Geant4Demo/)**.
3. Siga o README específico da demonstração para compilar e executar o programa.
4. Consulte o roteiro da atividade para registrar e interpretar os resultados.

## Organização do repositório

```text
.
├── README.md                         # Apresentação geral
├── .gitignore
├── ime.png                           # Brasão do IME
├── ppgen.png                         # Logotipo do PPGEN
├── Guia_Instalacao_Geant4_WSL_Alunos_IME_PPGEN.pdf
└── Geant4Demo/
    ├── README.md                     # Documentação da demonstração
    ├── ROTEIRO_ALUNOS.md             # Atividade prática
    ├── main.cc
    ├── CMakeLists.txt
    ├── include/
    ├── src/
    ├── tools/
    └── *.mac                         # Macros de execução e visualização
```

## Referências

- [Geant4 — site oficial](https://geant4.web.cern.ch/)
- [Geant4 — documentação](https://geant4.web.cern.ch/documentation/)
- [Microsoft — instalação do WSL](https://learn.microsoft.com/pt-br/windows/wsl/install/)

---

**Elaboração:** Diogenes Kreusch Filho  
**Instituição:** Instituto Militar de Engenharia — PPGEN  
**Disciplina:** Simulação Computacional Aplicada à Engenharia Nuclear
