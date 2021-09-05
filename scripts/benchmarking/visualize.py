#!/usr/bin/env python3

import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots
import argparse


def main():
    parser = argparse.ArgumentParser(description='Graph measurements from SpelunkyPSP benchmark')
    parser.add_argument('--csv_path', type=str, required=True)
    args = parser.parse_args()

    df = pd.read_csv(args.csv_path)

    fig = make_subplots(specs=[[{"secondary_y": True}]])

    fig.add_trace(
        go.Scatter(x=None, y=df['game_loop_update_time_ms'], name="game_loop_update_time_ms"),
        secondary_y=False,
    )

    fig.add_trace(
        go.Scatter(x=None, y=df['total_delta_time_ms'], name="total_delta_time_ms"),
        secondary_y=False,
    )

    fig.update_layout(title_text="SpelunkyPSP benchmarking results (TODO: Commit hash / branch)")
    fig.update_xaxes(title_text="Frame measurements")
    fig.update_yaxes(title_text="Time [ms]", secondary_y=False)
    fig.show()


if __name__ == '__main__':
    main()
