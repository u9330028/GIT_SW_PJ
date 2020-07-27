from emit import *

def map_func(key, value):
    words = value.lower().split()
    for word in words:
        emit(word, 1)

def reduce_func(key, values):
    emit(key, sum(values))
