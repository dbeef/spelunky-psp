#!/usr/bin/env python3
import numpy

import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots
import argparse
from datetime import datetime
import subprocess
from collections import namedtuple
import sys

color_index = 0
scenes = []
time_measurements = []


def parse_time_measurements(df):
    for scene_index, scene in enumerate(scenes):
        sum_game_loop_update_time_ms = 0
        sum_total_delta_time_ms = 0

        min_game_loop_update_time_ms = pow(2, 16)
        max_game_loop_update_time_ms = 0

        min_total_delta_time_ms = pow(2, 16)
        max_total_delta_time_ms = 0

        for i in range(scene.start_index, scene.end_index):
            game_loop_update_time_ms = df['game_loop_update_time_ms'][i]
            total_delta_time_ms = df['total_delta_time_ms'][i]

            sum_game_loop_update_time_ms += game_loop_update_time_ms
            sum_total_delta_time_ms += total_delta_time_ms

            min_game_loop_update_time_ms = min(min_game_loop_update_time_ms, game_loop_update_time_ms)
            max_game_loop_update_time_ms = max(max_game_loop_update_time_ms, game_loop_update_time_ms)

            min_total_delta_time_ms = min(min_total_delta_time_ms, total_delta_time_ms)
            max_total_delta_time_ms = max(max_total_delta_time_ms, total_delta_time_ms)

        measurement = namedtuple('measurement', ['avg_game_loop_update_time_ms', 'avg_total_delta_time_ms',
                                                 'min_game_loop_update_time_ms', 'max_game_loop_update_time_ms',
                                                 'min_total_delta_time_ms', 'max_total_delta_time_ms'])
        measurement.avg_game_loop_update_time_ms = round((sum_game_loop_update_time_ms / (scene.end_index - scene.start_index)), 2)
        measurement.avg_total_delta_time_ms = round((sum_total_delta_time_ms / (scene.end_index - scene.start_index)), 2)
        measurement.min_game_loop_update_time_ms = round((min_game_loop_update_time_ms), 2)
        measurement.max_game_loop_update_time_ms = round((max_game_loop_update_time_ms), 2)
        measurement.min_total_delta_time_ms = round((min_total_delta_time_ms), 2)
        measurement.max_total_delta_time_ms = round((max_total_delta_time_ms), 2)

        time_measurements.append(measurement)


def parse_scene_titles(df):
    last_title = ''
    last_index = 0

    for index, title in enumerate(df['scene_title']):

        if index == 0:
            last_title = title
            continue
        if title != last_title or index == len(df['scene_title']) - 1:
            scene = namedtuple('scene', ['title', 'start_index', 'end_index'])
            scene.title = last_title
            scene.start_index = last_index
            scene.end_index = index

            scenes.append(scene)

            last_index = index
            last_title = title


def get_next_color():
    global color_index
    color_index += 1
    colors = ['red', 'blue', 'green', 'pink', 'orange']
    return colors[color_index % len(colors)]


def get_branch():
    process = subprocess.run(['git', 'branch', '--show-current'], capture_output=True)
    if process.returncode != 0:
        return '-'
    else:
        return process.stdout.decode('utf-8').removesuffix('\n')


def get_commit_hash():
    process = subprocess.run(['git', 'rev-parse', '--short', 'HEAD'], capture_output=True)
    if process.returncode != 0:
        return '-'
    else:
        return process.stdout.decode('utf-8').removesuffix('\n')


def main():
    parser = argparse.ArgumentParser(description='Graph measurements from SpelunkyPSP benchmark')
    parser.add_argument('--csv_path', type=str, required=True)
    args = parser.parse_args()

    df = pd.read_csv(args.csv_path)

    fig = make_subplots(rows=3, cols=1,
                        specs=[[{"type": "scatter", "b": 0.075}], [{"type": "domain"}], [{"type": "domain"}]],
                        row_heights=[0.475, 0.2, 0.2])

    # Title / labels:

    date = datetime.now().strftime("%d/%m/%Y %H:%M:%S")
    fig.update_layout(
        title_text=f"<b>SpelunkyPSP benchmarking results</b><br>Git: {get_branch()}:{get_commit_hash()}, generated: {date}")
    fig.update_yaxes(title_text="Time [ms]", secondary_y=False)
    fig.update_xaxes(title_text="Frame index")

    # First row (actual graph):

    fig.add_trace(go.Scatter(x=None, y=df['game_loop_update_time_ms'], name="Game loop update time [ms]"), row=1, col=1)
    fig.add_trace(go.Scatter(x=None, y=df['total_delta_time_ms'], name="Total delta time [ms]"), row=1, col=1)

    parse_scene_titles(df)

    for _, scene in enumerate(scenes):
        fig.add_vrect(x0=scene.start_index, x1=scene.end_index,
                      annotation_text='<b>' + scene.title + '</b>', annotation_position="top left",
                      fillcolor=get_next_color(), opacity=0.25, line_width=0, row=1, col=1)

    # Second and third row (tables):

    parse_time_measurements(df)

    total_delta_time_cols = [['Average', 'Min', 'Max']]
    game_loop_update_time_cols = [['Average', 'Min', 'Max']]
    for measurement in time_measurements:
        total_delta_time_cols.append([measurement.avg_total_delta_time_ms, measurement.min_total_delta_time_ms, measurement.max_total_delta_time_ms])
        game_loop_update_time_cols.append([measurement.avg_game_loop_update_time_ms, measurement.min_game_loop_update_time_ms, measurement.max_game_loop_update_time_ms])

    fig.add_trace(go.Table(header=dict(values=['Game loop update time [ms]'] + [scene.title for scene in scenes],
                                       fill=dict(color='lightgray')),
                           cells=dict(fill=dict(color=['lightgray', 'whitesmoke']),
                                      values=game_loop_update_time_cols)),
                  row=2, col=1)

    fig.add_trace(go.Table(
        header=dict(values=['Total delta time [ms]'] + [scene.title for scene in scenes], fill=dict(color='lightgray')),
        cells=dict(fill=dict(color=['lightgray', 'whitesmoke']),
                   values=total_delta_time_cols)),
        row=3, col=1)

    # Set vertical axis to fixed range so comparing measurements would make sense:
    fig.update(layout_yaxis_range=[0, 60])

    fig.show()


if __name__ == '__main__':
    main()
