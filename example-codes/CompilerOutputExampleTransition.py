SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
FRONTIER_MODE = 'Open'     
EVOLUTION_MODE = 'Transition'

STATES = {'a': 0, 'b': 1, 'c': 2}
STATE_COLORS = {
    0: (255, 0, 0),    
    1: (0, 0, 0),      
    2: (0, 255, 0)     
}

def transition_function(cells, row, col):
    neighbor_above = get_cell_value(cells, row - 1, col)
    neighbor_right = get_cell_value(cells, row, col + 1)
    
    state_a = STATES['a']
    state_b = STATES['b']
    state_c = STATES['c']

    neighbor_below = get_cell_value(cells, row + 1, col)

    if get_cell_value(cells, row, col + 1) == state_a:
        return state_b
    else:
        if get_cell_value(cells, row + 1, col) == state_b:
            return state_c
        else:
            return state_a