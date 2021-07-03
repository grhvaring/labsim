# standard imports

import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl

# graphical settings

from IPython.display import set_matplotlib_formats, HTML

set_matplotlib_formats('svg') # little improvement on the default "png" output format

results_directory = "../results/"

def custom_css():
    return '{}'.format(open('custom.css').read())

def estimation_plot(ax, filename, expected_y=None, label="simulation", fmt="-", mfc='darkblue', lw=2, ms=4, color='darkblue', alpha=.7):
    '''Assume the file ``filename'' has the counts of individual ``throws'' in the first
    column, the estimations of some quantity in the second column and the correspondent 
    uncertainties in the third column. Extract the contents and plot them.'''
    # load data
    data = np.loadtxt(filename)
    x = data[:,0]
    y = data[:,-2]
    err = data[:,-1]
    # plot
    if expected_y is not None:
        ax.axhline(0, c='k', alpha=0.3, ls='--', lw=2)
    else:
        expected_y = 0
    markers, caps, bars = ax.errorbar(x, y-expected_y, yerr=err, fmt=fmt, mfc=mfc, c=color, lw=lw, label=label, ms=ms, alpha=alpha);
    # loop through bars and caps and set custom alpha value
    [bar.set_alpha(alpha*0.3) for bar in bars]
    [cap.set_alpha(alpha*0.3) for cap in caps]

def just_plot(ax, filename):
    data = np.loadtxt(filename)
    ax.plot(data[:,-1])

def double_plot(files, suptitle, titles, xlabel, ylabels):
    fig, ax = plt.subplots(1, 2, figsize=(10,3.8))
    fig.suptitle(f"\\textbf{{{suptitle}}}")
    for axis, title, ylabel, file in zip(ax, titles, ylabels, files):
        set_labels(axis, title, xlabel, ylabel)
        just_plot(axis, file)

def double_estimation(files, suptitle, titles, xlabel, ylabels, expected_ys=[None, None]):
    fig, ax = plt.subplots(1, 2, figsize=(10,3.8))
    fig.suptitle(suptitle)
    for axis, title, ylabel, file, exp_y in zip(ax, titles, ylabels, files, expected_ys):
        set_labels(axis, title, xlabel, ylabel)
        estimation_plot(axis, file, exp_y)

def set_labels(ax, title='', xlabel='', ylabel=''):
    'set labels and title of the graph'
    ax.set_title(f'\\textbf{{{title}}}')
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)


def settings_3d(ax, limit=5):
    ax.view_init(5, 45)
    ax.set_frame_on(False)
    ax.grid(False)

    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
        
    ax.set_xlim(-limit, limit)
    ax.set_ylim(-limit, limit)
    ax.set_zlim(-limit, limit)
    
    ax.xaxis.pane.fill = False
    ax.yaxis.pane.fill = False
    ax.zaxis.pane.fill = False


# +
from matplotlib.gridspec import GridSpec

def surface_plus_normal_plot():
    x = np.arange(-1,1,0.01)
    y = np.arange(-1,1,0.01)
    X, Y = np.meshgrid(x,y)
    Z = np.sin(X**2 + Y**2)

    print(x.shape)
    print(y.shape)
    print(Z.shape)

    fig = plt.figure(figsize=(10,5))
    gs = GridSpec(5, 5)
    ax = fig.add_subplot(gs[:,0:-2], projection='3d')

    # Plot a 3D surface + scatterplot
    np.random.shuffle(x)
    np.random.shuffle(y)
    ax.scatter(x, y, np.sin(x**2 + y**2), color='darkgreen', alpha=.9, label='surface')
    ax.plot_surface(X, Y, Z, alpha=0.1, color='black', label='data')
    ax.view_init(30,-150)

    legend_elements = [Line2D([0], [0], lw=10, alpha=0.1, color='k', label='surface'), 
                       Line2D([0], [0], lw=0, marker='o', color='darkgreen', label='data')]
    ax.legend(handles=legend_elements)

    # draw a normal plot on the side, with the correct aspect ratio
    ax1 = fig.add_subplot(gs[1:-1,-2:])
    ax1.plot(np.sin(np.linspace(0, np.pi, 100)))
