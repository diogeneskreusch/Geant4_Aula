#!/usr/bin/env python3
"""Analise dos resultados didaticos do Geant4Demo.

Uso:
    python3 tools/analisar_resultados.py resultados.csv
    python3 tools/analisar_resultados.py resultados.csv --output figuras

Apenas a ultima ocorrencia de cada configuracao e considerada. Isso evita
combinar inadvertidamente execucoes que tenham repetido a mesma semente.
Graficos requerem matplotlib; a tabela no terminal usa apenas a stdlib.
"""

import argparse
import csv
from collections import defaultdict
from pathlib import Path


def load_last_results(path):
    with path.open(newline='', encoding='utf-8') as handle:
        rows = list(csv.DictReader(handle))
    if not rows:
        raise ValueError('CSV sem resultados. Execute primeiro a aplicacao Geant4.')

    seen = defaultdict(int)
    latest = {}
    for row in rows:
        try:
            energy = float(row['energia_keV'])
            thickness = float(row['espessura_Pb_cm'])
            result = {
                'energy': energy,
                'thickness': thickness,
                'events': int(row['eventos']),
                'gamma_events': int(row['eventos_com_gamma']),
                'fraction': float(row['fracao_gamma']),
                'se_fraction': float(row['erro_fracao_1sigma']),
                'mean_edep': float(row['edep_media_keV']),
                'se_edep': float(row['erro_edep_media_keV']),
            }
        except (ValueError, KeyError) as exc:
            raise ValueError(f'Linha do CSV invalida: {row}') from exc

        key = (energy, thickness)
        latest[key] = result
        seen[key] += 1
    return sorted(latest.values(), key=lambda x: (x['energy'], x['thickness'])), seen


def print_results(rows, seen):
    print('RESULTADOS - Geant4Demo (incertezas de 1 sigma)')
    print('-' * 103)
    print(f'{"E (keV)":>8}  {"Pb (cm)":>7}  {"N":>8}  '
          f'{"chegadas":>8}  {"f (%)":>18}  {"Edep medio (keV)":>24}')
    print('-' * 103)
    for row in rows:
        frac = f'{row["fraction"]*100:.3f} +/- {row["se_fraction"]*100:.3f}'
        dep = f'{row["mean_edep"]:.5f} +/- {row["se_edep"]:.5f}'
        print(f'{row["energy"]:8.0f}  {row["thickness"]:7.2f}  '
              f'{row["events"]:8d}  {row["gamma_events"]:8d}  '
              f'{frac:>18}  {dep:>24}')
    print()

    repeated = [(key, n) for key, n in seen.items() if n > 1]
    if repeated:
        print('ATENCAO: ha configuracoes repetidas no CSV. Foram usadas somente')
        print('as ULTIMAS ocorrencias, sem presumir execucoes independentes:')
        for (energy, thickness), n in sorted(repeated):
            print(f'  E={energy:g} keV; Pb={thickness:g} cm: {n} registros')
        print()


def save_plots(rows, target):
    try:
        import matplotlib.pyplot as plt
    except ImportError:
        print('Graficos nao gerados: instale matplotlib (python3 -m pip install matplotlib).')
        return

    target.mkdir(parents=True, exist_ok=True)
    energies = sorted({r['energy'] for r in rows})

    definitions = (
        ('fracao_chegada.png', 'fraction', 'se_fraction', 100,
         'Eventos com chegada de fótons (%)'),
        ('energia_depositada.png', 'mean_edep', 'se_edep', 1,
         'Energia média depositada por evento (keV)'),
    )
    for filename, value_key, error_key, factor, ylabel in definitions:
        fig, ax = plt.subplots(figsize=(7.5, 4.5))
        for energy in energies:
            subset = sorted((r for r in rows if r['energy'] == energy),
                            key=lambda r: r['thickness'])
            x = [r['thickness'] for r in subset]
            y = [r[value_key] * factor for r in subset]
            dy = [r[error_key] * factor for r in subset]
            ax.errorbar(x, y, yerr=dy, marker='o', capsize=4,
                        label=f'{energy:g} keV')
        ax.set_xlabel('Espessura da blindagem de Pb (cm)')
        ax.set_ylabel(ylabel)
        ax.set_title('Geant4Demo: estudo de blindagem')
        ax.grid(alpha=0.25)
        ax.legend(title='Energia primária')
        fig.tight_layout()
        path = target / filename
        fig.savefig(path, dpi=180)
        plt.close(fig)
        print(f'Grafico salvo: {path}')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('csv', nargs='?', default='resultados.csv', type=Path)
    parser.add_argument('--output', type=Path, default=Path('figuras'))
    args = parser.parse_args()

    if not args.csv.is_file():
        parser.error(f'Nao foi encontrado: {args.csv}')

    try:
        rows, seen = load_last_results(args.csv)
    except ValueError as exc:
        parser.error(str(exc))

    print_results(rows, seen)
    save_plots(rows, args.output)


if __name__ == '__main__':
    main()
