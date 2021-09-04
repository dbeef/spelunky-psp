#!/usr/bin/env python3

import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots

# TODO: Take path from args
df = pd.read_csv('/home/dbeef/Desktop/spelunky-psp/cmake-build-debug/benchmark_measurements.csv')
# TODO: Interpolate points inbetween
# https://stackoverflow.com/questions/5283649/plot-smooth-line-with-pyplot

# Create figure with secondary y-axis
fig = make_subplots(specs=[[{"secondary_y": True}]])

# Add traces
fig.add_trace(
    go.Scatter(x=None, y=df['game_loop_update_time_ms'], name="game_loop_update_time_ms"),
    secondary_y=False,
)

fig.add_trace(
    go.Scatter(x=None, y=df['total_delta_time_ms'], name="total_delta_time_ms"),
    secondary_y=False,
)

# Add figure title
fig.update_layout(
    title_text="SpelunkyPSP benchmarking results (TODO: Commit hash / branch)"
)

# Set x-axis title
fig.update_xaxes(title_text="Frame measurements")

# Set y-axes titles
fig.update_yaxes(title_text="Time [ms]", secondary_y=False)

fig.show()