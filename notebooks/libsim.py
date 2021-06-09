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