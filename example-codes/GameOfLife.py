import time
import pygame
import numpy as np

CELL_SIZE = 20

COLOR_BG = (10, 10, 10)
COLOR_GRID = (40, 40, 40)
COLOR_DIE_NEXT = (170, 170, 170)
COLOR_ALIVE_NEXT = (255, 255, 255)

SCREEN_WIDTH = 1200
SCREEN_HEIGHT = 800

def update(screen, cells, size, with_progress=False):
    updated_cells = np.zeros((cells.shape[0], cells.shape[1]))

    for row, col in np.ndindex(cells.shape):
        alive = np.sum(cells[row-1:row+2, col-1:col+2]) - cells[row, col]
        color = COLOR_BG if cells[row, col] == 0 else COLOR_ALIVE_NEXT

        if cells[row, col] == 1:
            if alive < 2 or alive > 3:
                if with_progress:
                    color = COLOR_DIE_NEXT
            elif 2 <= alive <= 3:
                updated_cells[row, col] = 1
                if with_progress:
                    color = COLOR_ALIVE_NEXT
        else:
            if alive == 3:
                updated_cells[row, col] = 1
                if with_progress:
                    color = COLOR_ALIVE_NEXT

        pygame.draw.rect(screen, color, (col * size, row * size, size - 1, size - 1))

    return updated_cells

N_CELLS_X = SCREEN_WIDTH // CELL_SIZE
N_CELLS_Y = SCREEN_HEIGHT // CELL_SIZE

def main():
    pygame.init()
    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))

    cells = np.zeros((N_CELLS_Y, N_CELLS_X))
    screen.fill(COLOR_GRID)
    update(screen, cells, CELL_SIZE)

    pygame.display.flip()
    
    running = False

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                return
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    running = not running
                    update(screen, cells, CELL_SIZE)
                elif event.key == pygame.K_c:
                    running = False
                    cells = np.zeros((N_CELLS_Y, N_CELLS_X))
                    screen.fill(COLOR_GRID)
                    update(screen, cells, CELL_SIZE)
                    
            if pygame.mouse.get_pressed()[0]:
                pos = pygame.mouse.get_pos()
                row, col = pos[1] // CELL_SIZE, pos[0] // CELL_SIZE
                if row < N_CELLS_Y and col < N_CELLS_X:
                    cells[row, col] = 1
                    update(screen, cells, CELL_SIZE)
                    pygame.display.update()
            elif pygame.mouse.get_pressed()[2]:
                pos = pygame.mouse.get_pos()
                row, col = pos[1] // CELL_SIZE, pos[0] // CELL_SIZE
                if row < N_CELLS_Y and col < N_CELLS_X:
                    cells[row, col] = 0
                    update(screen, cells, CELL_SIZE)
                    pygame.display.update()

        screen.fill(COLOR_GRID)

        if running:
            cells = update(screen, cells, CELL_SIZE, with_progress=True)
            pygame.display.update()

        time.sleep(0.01)

if __name__ == '__main__':
    main()