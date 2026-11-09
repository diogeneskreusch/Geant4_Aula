#!/usr/bin/env python3
"""Compara os arquivos CSV de scoring de duas geometrias Geant4.

Usa somente a biblioteca padrão do Python 3. Localiza as duas pastas
relativas a este próprio script; pode ser executado a partir de qualquer local.

Execução usual:
    python3 tools/comparar_scores_geant4.py

Opcional:
    python3 tools/comparar_scores_geant4.py --eventos 20000

Cada caso precisa dos arquivos <caso>_edep.csv e <caso>_fluxo.csv.
Formato Geant4: iX,iY,iZ,total(value),total(val^2),entry.
A coluna física é a 4ª (índice 3); a 6ª é o número de entradas.
"""

from __future__ import annotations

import argparse
import csv
from html import escape
from pathlib import Path
import re
import sys

RAIZ = Path(__file__).resolve().parent.parent
IGNORAR = ("teste", "test", "smoke")


def ler_score(caminho: Path, scorer_esperado: str) -> tuple[float, str]:
    """Lê a soma da coluna total(value) e a unidade declarada no cabeçalho."""
    total = 0.0
    unidade = ""
    nome = None
    cabecalho_encontrado = False
    linhas_numericas = 0

    with caminho.open("r", encoding="utf-8-sig", newline="") as arquivo:
        for numero, linha in enumerate(arquivo, 1):
            linha = linha.strip()
            if not linha:
                continue
            if linha.startswith("#"):
                if "primitive scorer name:" in linha:
                    nome = linha.split("primitive scorer name:", 1)[1].strip()
                if "total(value)" in linha and "total(val^2)" in linha and "entry" in linha:
                    cabecalho_encontrado = True
                    match = re.search(r"total\(value\)\s*\[([^]]+)\]", linha)
                    unidade = match.group(1).strip() if match else ""
                continue

            campos = next(csv.reader([linha], skipinitialspace=True))
            if len(campos) != 6:
                raise ValueError(
                    f"{caminho.name}, linha {numero}: esperado CSV Geant4 com seis colunas; "
                    f"foram encontradas {len(campos)}"
                )
            try:
                total += float(campos[3])
            except ValueError as exc:
                raise ValueError(
                    f"{caminho.name}, linha {numero}: valor inválido em total(value)"
                ) from exc
            linhas_numericas += 1

    if not cabecalho_encontrado or nome != scorer_esperado:
        raise ValueError(
            f"{caminho}: cabeçalho/scorer inesperado. "
            f"Esperado: {scorer_esperado}; encontrado: {nome!r}"
        )
    if linhas_numericas == 0:
        raise ValueError(f"{caminho}: nenhum registro numérico foi encontrado")
    return total, unidade


def encontrar_pares(diretorio: Path, incluir_testes: bool):
    if not diretorio.is_dir():
        raise FileNotFoundError(
            f"Pasta não encontrada: {diretorio}. "
            "Execute as simulações e crie a pasta resultados/ de cada geometria."
        )

    pares = {}
    for sufixo in ("edep", "fluxo"):
        for arquivo in diretorio.glob(f"*_{sufixo}.csv"):
            caso = arquivo.name[: -len(f"_{sufixo}.csv")]
            if not incluir_testes and caso.lower().startswith(IGNORAR):
                continue
            pares.setdefault(caso, {})[sufixo] = arquivo

    if not pares:
        raise ValueError(f"Nenhum resultado para comparação em: {diretorio}")

    saida = []
    for caso in sorted(pares):
        if set(pares[caso]) != {"edep", "fluxo"}:
            faltante = "fluxo" if "fluxo" not in pares[caso] else "edep"
            raise ValueError(
                f"Em {diretorio}, o caso '{caso}' não tem o arquivo _{faltante}.csv correspondente"
            )
        saida.append((caso, pares[caso]["edep"], pares[caso]["fluxo"]))
    return saida


def gravar_svg(caminho: Path, titulo: str, subtitulo: str, registros: list[dict],
               chave: str, unidade: str):
    """Gráfico horizontal independente de bibliotecas como matplotlib."""
    largura, altura_linha = 1120, 58
    topo, margem_label, largura_barras = 150, 380, 585
    altura = topo + altura_linha * len(registros) + 78
    maximo = max((max(0.0, item[chave]) for item in registros), default=0.0)
    limite = maximo * 1.12 if maximo > 0 else 1.0

    partes = [
        f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {largura} {altura}" '
        f'width="{largura}" height="{altura}" role="img" aria-label="{escape(titulo)}">',
        '<rect width="100%" height="100%" fill="#f8fafc"/>',
        f'<text x="36" y="45" fill="#13253b" font-size="25" font-weight="700" '
        f'font-family="Arial,sans-serif">{escape(titulo)}</text>',
        f'<text x="36" y="76" fill="#475569" font-size="15" font-family="Arial,sans-serif">'
        f'{escape(subtitulo)}</text>',
        '<rect x="36" y="91" width="12" height="12" rx="2" fill="#1976b9"/>',
        '<text x="55" y="102" font-size="13" fill="#475569" font-family="Arial,sans-serif">Original</text>',
        '<rect x="143" y="91" width="12" height="12" rx="2" fill="#cf7926"/>',
        '<text x="162" y="102" font-size="13" fill="#475569" font-family="Arial,sans-serif">Modificada</text>',
    ]

    for k in range(5):
        x = margem_label + largura_barras * k / 4
        y_final = topo + altura_linha * len(registros) - 7
        partes.append(
            f'<line x1="{x:.1f}" y1="{topo-13}" x2="{x:.1f}" y2="{y_final}" '
            'stroke="#dbe3ed" stroke-width="1"/>'
        )
        partes.append(
            f'<text x="{x:.1f}" y="{topo-22}" text-anchor="middle" font-size="12" '
            f'fill="#64748b" font-family="Arial,sans-serif">{limite*k/4:.3g}</text>'
        )

    for i, item in enumerate(registros):
        y = topo + i * altura_linha
        cor = "#1976b9" if item["geometria"] == "Original" else "#cf7926"
        valor = max(0, item[chave])
        tamanho = largura_barras * valor / limite
        nome = f'{item["geometria"]} / {item["caso"]}'
        if len(nome) > 39:
            nome = nome[:36] + "..."
        partes.extend([
            f'<text x="36" y="{y+23}" font-size="15" fill="#243346" '
            f'font-family="Arial,sans-serif">{escape(nome)}</text>',
            f'<rect x="{margem_label}" y="{y+3}" width="{tamanho:.1f}" height="31" '
            f'rx="4" fill="{cor}"/>',
            f'<text x="{margem_label+tamanho+10:.1f}" y="{y+23}" font-size="13" '
            f'font-family="Arial,sans-serif" fill="#243346">{valor:.5g}</text>',
        ])

    partes.append(
        f'<text x="36" y="{altura-26}" font-size="13" fill="#64748b" '
        f'font-family="Arial,sans-serif">Unidade: {escape(unidade)}. '
        'Valores acumulados normalizados pelo número de eventos informado; sem barras de incerteza.</text>'
    )
    partes.append('</svg>')
    caminho.write_text("\n".join(partes) + "\n", encoding="utf-8")


def executar():
    ap = argparse.ArgumentParser(description="Compara energia depositada e fluência de duas aplicações Geant4")
    ap.add_argument("--eventos", type=int, default=20000,
                    help="Número de eventos em CADA execução (padrão: 20000)")
    ap.add_argument("--original", type=Path, default=RAIZ / "GeometriaOriginal" / "resultados")
    ap.add_argument("--modificada", type=Path, default=RAIZ / "GeometriaModificada" / "resultados")
    ap.add_argument("--incluir-testes", action="store_true", help="Também incluir arquivos de teste")
    argumentos = ap.parse_args()
    if argumentos.eventos < 1:
        ap.error("--eventos deve ser um inteiro positivo")

    registros = []
    unidades = {"edep": None, "fluxo": None}
    for rotulo, pasta in (("Original", argumentos.original), ("Modificada", argumentos.modificada)):
        for caso, arq_e, arq_f in encontrar_pares(pasta, argumentos.incluir_testes):
            edep, un_e = ler_score(arq_e, "Edep")
            fluxo, un_f = ler_score(arq_f, "GammaFlux")
            for chave, unidade in (("edep", un_e), ("fluxo", un_f)):
                if unidades[chave] is None:
                    unidades[chave] = unidade
                elif unidades[chave] != unidade:
                    raise ValueError(
                        f"Unidades diferentes para {chave}: "
                        f"{unidades[chave]!r} versus {unidade!r}."
                    )
            registros.append({
                "geometria": rotulo,
                "caso": caso,
                "eventos": argumentos.eventos,
                "edep_total": edep,
                "edep_por_evento": edep / argumentos.eventos,
                "fluxo_total": fluxo,
                "fluxo_por_evento": fluxo / argumentos.eventos,
                "unidade_edep": un_e,
                "unidade_fluxo": un_f,
            })

    csv_saida = RAIZ / "comparacao_scores.csv"
    with csv_saida.open("w", encoding="utf-8", newline="") as arquivo:
        gravador = csv.DictWriter(arquivo, fieldnames=list(registros[0]))
        gravador.writeheader()
        gravador.writerows(registros)

    print(f"\nComparação Geant4 — {argumentos.eventos} eventos por execução")
    print(f"{'Geometria':<12} {'Caso':<28} {'Edep/evento':>16} {'Fluxo/evento':>16}")
    for r in registros:
        print(f"{r['geometria']:<12} {r['caso'][:28]:<28} "
              f"{r['edep_por_evento']:>16.7g} {r['fluxo_por_evento']:>16.7g}")
    print(f"Unidades: Edep [{unidades['edep']}], fluência [{unidades['fluxo']}].")

    graf_e = RAIZ / "grafico_energia_depositada.svg"
    graf_f = RAIZ / "grafico_fluencia_gamma.svg"
    gravar_svg(graf_e, "Energia depositada no silício", "Média por evento em cada configuração",
               registros, "edep_por_evento", unidades["edep"] or "não declarada")
    gravar_svg(graf_f, "Fluência de fótons no silício", "Estimador de comprimento de trajetória / volume, por evento",
               registros, "fluxo_por_evento", unidades["fluxo"] or "não declarada")
    print(f"\nArquivos produzidos:\n  {csv_saida}\n  {graf_e}\n  {graf_f}")
    print("ATENÇÃO: as energias e espessuras não são verificadas pelo script; confiram os casos.")
    print("O número de eventos é fornecido pelo usuário, não identificado automaticamente.")
    print("Os CSVs não fornecem diretamente a incerteza da média entre eventos.")


if __name__ == "__main__":
    try:
        executar()
    except (OSError, ValueError) as erro:
        sys.exit(f"ERRO: {erro}")
