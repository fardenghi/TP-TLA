SCREEN_WIDTH = 1000
SCREEN_HEIGHT = 800
FRONTIER_MODE = 'Periodic' 
EVOLUTION_MODE = 'SB'      

STATES = {'dead': 0, 'alive': 1}

STATE_COLORS = {
    0: (10, 10, 10),    
    1: (255, 255, 255)  
}

BIRTH_RULES = [3]
SURVIVE_RULES = [2, 3]

NEIGHBORHOOD = [
    (-1, -1), (-1, 0), (-1, 1),
    (0, -1),          (0, 1),
    (1, -1),  (1, 0),  (1, 1)
]
