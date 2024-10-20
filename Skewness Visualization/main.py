import dash
from dash import dcc, html
from dash.dependencies import Input, Output, State
import base64
import io
import numpy as np
import plotly.graph_objs as go
from scipy.stats import skew, boxcox, norm
import pandas as pd
import dash_bootstrap_components as dbc

# Initialize the Dash app with a Bootstrap theme
app = dash.Dash(__name__, external_stylesheets=[dbc.themes.MINTY])
app.title = "Variable Transformation Dashboard"

# Define the layout
app.layout = dbc.Container([
  # Header
  dbc.Row([
    dbc.Col(html.H1("Variable Transformation from Skewness", className="text-center text-primary mb-4"), width=12)
  ]),

  # Input Section
  dbc.Row([
    dbc.Col([
      dbc.Card([
        dbc.CardHeader("Data Input"),
        dbc.CardBody([
          # Data Input Textarea
          dbc.Row([
            dbc.Col([
              dbc.Label("Enter your data (comma-separated values):"),
              dcc.Textarea(
                id='data-input',
                value='',
                style={'width': '100%', 'height': '100px'},
                className="form-control"
              ),
            ])
          ], className="mb-3"),

          # File Upload
          dbc.Row([
            dbc.Col([
              dbc.Label("Or upload a CSV file:"),
              dcc.Upload(
                id='upload-data',
                children=html.Div([
                  'Drag and Drop or ',
                  html.A('Select Files')
                ]),
                multiple=False,
                className="mb-3",
                style={
                  'width': '100%',
                  'height': '60px',
                  'lineHeight': '60px',
                  'borderWidth': '1px',
                  'borderStyle': 'dashed',
                  'borderRadius': '5px',
                  'textAlign': 'center',
                  'backgroundColor': '#f9f9f9',
                  'cursor': 'pointer'
                }
              ),
            ])
          ], className="mb-3"),

          # Buttons
          dbc.Row([
            dbc.Col([
              dbc.Button('Submit', id='submit-button', color="primary", className="me-2"),
              dbc.Button('Download CSV', id='download-button', color="success"),
            ])
          ])
        ])
      ], className="mb-4")
    ], md=6),

    # Visualization Options
    dbc.Col([
      dbc.Card([
        dbc.CardHeader("Visualization Options"),
        dbc.CardBody([
          dbc.Label("Select data to visualize:"),
          dbc.Checklist(
            id='transformation-options',
            options=[
              {'label': 'Original Data', 'value': 'original'},
              {'label': 'Log Transform', 'value': 'log'},
              {'label': 'Square Root Transform', 'value': 'sqrt'},
              {'label': 'Box-Cox Transform', 'value': 'boxcox'},
            ],
            value=['original', 'log', 'sqrt', 'boxcox'],
            inline=True,
            switch=True
          ),
        ])
      ], className="mb-4")
    ], md=6)
  ]),

  # Alert for Messages
  dbc.Row([
    dbc.Col([
      dbc.Alert(
        id='output-container',
        color="warning",
        is_open=False,
        dismissable=True
      )
    ], width=12)
  ]),

  # Histogram Graph
  dbc.Row([
    dbc.Col([
      dbc.Card([
        dbc.CardHeader("Histogram"),
        dbc.CardBody([
          dcc.Graph(id='histogram-graph')
        ])
      ])
    ], width=12)
  ]),

  # Download Component
  dcc.Download(id='download-dataframe-csv'),
], fluid=True)


# Callback to Update Graph and Show Messages
@app.callback(
  [Output('histogram-graph', 'figure'),
   Output('output-container', 'children'),
   Output('output-container', 'is_open')],
  [Input('submit-button', 'n_clicks')],
  [State('transformation-options', 'value'),
   State('data-input', 'value'),
   State('upload-data', 'contents'),
   State('upload-data', 'filename')]
)
def update_graph(n_clicks, selected_transformations, data_text, upload_contents, upload_filename):
  # Ensure the callback runs only when the submit button is clicked
  if n_clicks is None or n_clicks == 0:
    return dash.no_update, "", False

  # Initialize data_array
  data_array = None
  messages = []

  # Check if a file was uploaded
  if upload_contents is not None:
    content_type, content_string = upload_contents.split(',')
    decoded = base64.b64decode(content_string)
    try:
      if upload_filename.endswith('.csv'):
        # Use pandas to read the CSV file
        df = pd.read_csv(io.StringIO(decoded.decode('utf-8')))
        # Flatten the DataFrame into a 1D NumPy array
        data_array = df.select_dtypes(include=[np.number]).values.flatten()
      else:
        messages.append('Unsupported file format. Please upload a CSV file.')
    except Exception as e:
      messages.append(f'Error processing file: {e}')
  else:
    # Use data from the text area
    try:
      data_array = np.array([float(x.strip()) for x in data_text.split(',') if x.strip() != ''])
    except ValueError:
      messages.append('Invalid input data. Please enter numeric values separated by commas or upload a CSV file.')

  if data_array is None or len(data_array) == 0:
    messages.append('No data provided. Please enter numeric values or upload a CSV file.')

  if messages:
    # If there are messages, display them as an alert
    return dash.no_update, "<br>".join(messages), True

  # Original skewness
  orig_skewness = skew(data_array)

  # Prepare data for transformations
  data_dict = {}

  # Original Data
  if 'original' in selected_transformations:
    data_dict['original'] = {
      'data': data_array,
      'skewness': orig_skewness,
      'label': f'Original Data (Skewness: {orig_skewness:.2f})'
    }

  # Positive data for log and Box-Cox transformations
  positive_data = data_array[data_array > 0]
  non_negative_data = data_array[data_array >= 0]

  # Log Transformation
  if 'log' in selected_transformations:
    if len(positive_data) > 0:
      log_data = np.log(positive_data)
      log_skewness = skew(log_data)
      data_dict['log'] = {
        'data': log_data,
        'skewness': log_skewness,
        'label': f'Log Transform (Skewness: {log_skewness:.2f})'
      }
    else:
      messages.append('Data must contain positive values for Log Transformation.')

  # Square Root Transformation
  if 'sqrt' in selected_transformations:
    if len(non_negative_data) > 0:
      sqrt_data = np.sqrt(non_negative_data)
      sqrt_skewness = skew(sqrt_data)
      data_dict['sqrt'] = {
        'data': sqrt_data,
        'skewness': sqrt_skewness,
        'label': f'Sqrt Transform (Skewness: {sqrt_skewness:.2f})'
      }
    else:
      messages.append('Data must contain non-negative values for Square Root Transformation.')

  # Box-Cox Transformation
  if 'boxcox' in selected_transformations:
    if len(positive_data) > 0:
      try:
        boxcox_data, _ = boxcox(positive_data)
        boxcox_skewness = skew(boxcox_data)
        data_dict['boxcox'] = {
          'data': boxcox_data,
          'skewness': boxcox_skewness,
          'label': f'Box-Cox Transform (Skewness: {boxcox_skewness:.2f})'
        }
      except ValueError as ve:
        messages.append(f'Box-Cox Transformation error: {ve}')
    else:
      messages.append('Data must contain positive values for Box-Cox Transformation.')

  if messages:
    return dash.no_update, "<br>".join(messages), True

  if not data_dict:
    messages.append('No valid data available for the selected transformations.')
    return dash.no_update, "<br>".join(messages), True

  # Create histograms and distribution curves
  histograms = []
  distribution_curves = []
  colors = {
    'original': 'blue',
    'log': 'green',
    'sqrt': 'orange',
    'boxcox': 'red'
  }
  line_styles = {
    'original': 'solid',
    'log': 'dash',
    'sqrt': 'dot',
    'boxcox': 'dashdot'
  }

  for key, value in data_dict.items():
    # Histogram
    hist = go.Histogram(
      x=value['data'],
      opacity=0.6,
      name=value['label'],
      histnorm='probability density',  # Normalize histograms
      marker_color=colors.get(key, 'gray'),
      showlegend=True
    )
    histograms.append(hist)

    # Calculate Normal Distribution Parameters
    mu, std = norm.fit(value['data'])
    xmin = min(value['data'])
    xmax = max(value['data'])
    x_values = np.linspace(xmin, xmax, 100)
    p = norm.pdf(x_values, mu, std)

    # Normal Distribution Curve
    curve = go.Scatter(
      x=x_values,
      y=p,
      mode='lines',
      name=f'Normal Fit ({key.capitalize()})',
      line=dict(color=colors.get(key, 'gray'), dash=line_styles.get(key, 'solid')),
      showlegend=True
    )
    distribution_curves.append(curve)

  # Combine histograms and curves
  all_traces = histograms + distribution_curves

  # Prepare figure
  figure = go.Figure(
    data=all_traces,
    layout=go.Layout(
      barmode='overlay',
      title='Histogram and Normal Distribution Fit of Original and Transformed Data',
      xaxis_title='Value',
      yaxis_title='Density',
      height=600,
      template='plotly_white'
    )
  )

  return figure, "", False


# Callback to Download CSV
@app.callback(
  Output('download-dataframe-csv', 'data'),
  [Input('download-button', 'n_clicks')],
  [State('transformation-options', 'value'),
   State('data-input', 'value'),
   State('upload-data', 'contents'),
   State('upload-data', 'filename')],
  prevent_initial_call=True,
)
def download_csv(n_clicks, selected_transformations, data_text, upload_contents, upload_filename):
  if n_clicks is None:
    return

  # Initialize data_array
  data_array = None

  # Check if a file was uploaded
  if upload_contents is not None:
    content_type, content_string = upload_contents.split(',')
    decoded = base64.b64decode(content_string)
    try:
      if upload_filename.endswith('.csv'):
        # Use pandas to read the CSV file
        df = pd.read_csv(io.StringIO(decoded.decode('utf-8')))
        # Flatten the DataFrame into a 1D NumPy array
        data_array = df.select_dtypes(include=[np.number]).values.flatten()
      else:
        return
    except Exception:
      return
  else:
    # Use data from the text area
    try:
      data_array = np.array([float(x.strip()) for x in data_text.split(',') if x.strip() != ''])
    except ValueError:
      return

  if data_array is None or len(data_array) == 0:
    return

  # Apply selected transformations
  transformed_data = {}

  # Original Data
  if 'original' in selected_transformations:
    transformed_data['Original Data'] = data_array

  # Log Transformation
  if 'log' in selected_transformations and len(data_array[data_array > 0]) > 0:
    transformed_data['Log Transform'] = np.log(data_array[data_array > 0])

  # Square Root Transformation
  if 'sqrt' in selected_transformations and len(data_array[data_array >= 0]) > 0:
    transformed_data['Square Root Transform'] = np.sqrt(data_array[data_array >= 0])

  # Box-Cox Transformation
  if 'boxcox' in selected_transformations and len(data_array[data_array > 0]) > 0:
    try:
      boxcox_data, _ = boxcox(data_array[data_array > 0])
      transformed_data['Box-Cox Transform'] = boxcox_data
    except ValueError:
      pass  # Skip if Box-Cox fails

  # Create a DataFrame from transformed data
  df_transformed = pd.DataFrame(transformed_data)

  # Convert the DataFrame to CSV
  return dcc.send_data_frame(df_transformed.to_csv, "transformed_data.csv", index=False)


if __name__ == '__main__':
  app.run_server(debug=True)
