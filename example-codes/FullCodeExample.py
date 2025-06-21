import time
import pygame
import numpy as np

CELL_SIZE = 20

### INICIO SECCION GENERADA POR COMPILADOR ###

SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
FRONTIER_MODE = 'Open'     
EVOLUTION_MODE = 'Transition'

STATES = {'a': 0, 'b': 1, 'c': 2}
STATE_COLORS = [
    (255, 0, 0),
    (0, 0, 0),      
    (0, 255, 0)     
]

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

### FIN SECCION GENERADA POR COMPILADOR ###

N_CELLS_X = SCREEN_WIDTH // CELL_SIZE
N_CELLS_Y = SCREEN_HEIGHT // CELL_SIZE

def get_cell_value(cells, row, col):
    
    if FRONTIER_MODE == 'Periodic':
        row = row % N_CELLS_Y
        col = col % N_CELLS_X
        return cells[row, col]

    if FRONTIER_MODE == 'Mirror':
        if row < 0: row = -row
        if col < 0: col = -col
        if row >= N_CELLS_Y: row = 2 * (N_CELLS_Y - 1) - row
        if col >= N_CELLS_X: col = 2 * (N_CELLS_X - 1) - col
        return cells[row, col]
        
    if 0 <= row < N_CELLS_Y and 0 <= col < N_CELLS_X:
        return cells[row, col]
    else:
        return 0

def update(screen, cells):
    
    updated_cells = np.zeros((N_CELLS_Y, N_CELLS_X))

    for row, col in np.ndindex(cells.shape):
        
        if EVOLUTION_MODE == 'SB':
            alive_neighbors = 0
            for dr, dc in NEIGHBORHOOD:
                if get_cell_value(cells, row + dr, col + dc) == 1:
                    alive_neighbors += 1
            
            current_state = cells[row, col]
            
            if current_state == 1:
                if alive_neighbors in SURVIVE_RULES:
                    updated_cells[row, col] = 1
                else:
                    updated_cells[row, col] = 0
            else:
                if alive_neighbors in BIRTH_RULES:
                    updated_cells[row, col] = 1
                else:
                    updated_cells[row, col] = 0

        elif EVOLUTION_MODE == 'Transition':
            updated_cells[row, col] = transition_function(cells, row, col)

        color = STATE_COLORS[int(updated_cells[row, col])]
        pygame.draw.rect(screen, color, (col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE -1, CELL_SIZE -1))

    return updated_cells

def main():
    pygame.init()
    pygame.display.set_caption("Cellular Automata")
    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))

    cells = np.zeros((N_CELLS_Y, N_CELLS_X))
    
    for row, col in np.ndindex(cells.shape):
        color = STATE_COLORS[0]
        pygame.draw.rect(screen, color, (col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1))

    pygame.display.flip()
    
    running = False
    num_states = len(STATES)
    
    drawing_mode = 0
    painted_cells_this_drag = set()

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                return
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    running = not running
                elif event.key == pygame.K_c:
                    running = False
                    cells = np.zeros((N_CELLS_Y, N_CELLS_X))
            
            elif event.type == pygame.MOUSEBUTTONDOWN:
                drawing_mode = event.button
                painted_cells_this_drag.clear()
                pos = pygame.mouse.get_pos()
                row, col = pos[1] // CELL_SIZE, pos[0] // CELL_SIZE
                if row < N_CELLS_Y and col < N_CELLS_X:
                    if drawing_mode == 1:
                        cells[row, col] = (cells[row, col] + 1) % num_states
                    elif drawing_mode == 3:
                        cells[row, col] = 0
                    painted_cells_this_drag.add((row, col))

            elif event.type == pygame.MOUSEBUTTONUP:
                drawing_mode = 0
                painted_cells_this_drag.clear()

            elif event.type == pygame.MOUSEMOTION:
                if drawing_mode != 0:
                    pos = event.pos
                    row, col = pos[1] // CELL_SIZE, pos[0] // CELL_SIZE
                    if (row, col) not in painted_cells_this_drag:
                        if row < N_CELLS_Y and col < N_CELLS_X:
                            if drawing_mode == 1:
                                cells[row, col] = (cells[row, col] + 1) % num_states
                            elif drawing_mode == 3:
                                cells[row, col] = 0
                            painted_cells_this_drag.add((row, col))

        screen.fill((40,40,40))

        if running:
            cells = update(screen, cells)
        else:
            for row, col in np.ndindex(cells.shape):
                color = STATE_COLORS[int(cells[row, col])]
                pygame.draw.rect(screen, color, (col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1))

        pygame.display.update()
        time.sleep(0.01)

if __name__ == '__main__':
    main()