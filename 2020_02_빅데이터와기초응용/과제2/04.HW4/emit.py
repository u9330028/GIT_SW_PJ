shuffle = {}
result = []
map_phase = True
def emit(k, v):
    if map_phase:
        if k not in shuffle:
            shuffle[k] = []
        shuffle[k].append(v)
    else:
        result.append((k, v))

def switchPhase():
    global map_phase
    map_phase = False