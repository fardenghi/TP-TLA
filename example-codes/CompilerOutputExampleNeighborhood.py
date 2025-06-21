SCREEN_WIDTH = 1000
SCREEN_HEIGHT = 800
FRONTIER_MODE = 'Periodic' 
EVOLUTION_MODE = 'SB'
      
BIRTH_RULES = [3]
SURVIVE_RULES = [2, 3]

STATES = {'dead': 0, 'alive': 1}

STATE_COLORS = {
    0: (10, 10, 10),    
    1: (255, 255, 255)  
}


def neighborhood_function(row, col):
    neighbors = set()
    
    for i in [-3, 3]:
        neighbors.update([(i, 0), (0, i)])


    return neighbors
