import time
import pygame
import numpy as np
import math

pygame.init()

display_info = pygame.display.Info()
MAX_SCREEN_WIDTH = display_info.current_w - 100
MAX_SCREEN_HEIGHT = display_info.current_h - 100

###
N_CELLS_Y=50
N_CELLS_X=50
FRONTIER_MODE='Periodic'
alive=0
dead=1
STATES={'dead': 0, 'alive': 1}
STATE_COLORS=[0,16777215]
def neighborhood_function(row, col):
    return {
        (-1, -1), (-1, 0), (-1, 1),
        (0, -1),          (0, 1),
        (1, -1),  (1, 0),  (1, 1)
    }
EVOLUTION_MODE='SB'
SURVIVE_RULES = [2,3]
BIRTH_RULES = [3]

###

cell_size_w = MAX_SCREEN_WIDTH // N_CELLS_X
cell_size_h = MAX_SCREEN_HEIGHT // N_CELLS_Y

CELL_SIZE = min(cell_size_w, cell_size_h)

SCREEN_WIDTH = N_CELLS_X * CELL_SIZE
SCREEN_HEIGHT = N_CELLS_Y * CELL_SIZE

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
            for dr, dc in neighborhood_function(row, col):
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
    
    is_dropdown_open = False
    is_dragging_slider = False
    speed_slider_value = 0.9     

    is_dropdown_open = False
    
    GUI_BG_COLOR = (45, 52, 54)
    GUI_ACCENT_COLOR = (99, 110, 114)
    GUI_TEXT_COLOR = (223, 230, 232)
    GUI_BORDER_RADIUS = 8
    GUI_PADDING = 10
    SYMBOL_SIZE = 10
    
    font_names = ["Calibri", "Arial", "Helvetica", "sans-serif"]
    title_font = pygame.font.SysFont(font_names, 26)
    title_font.set_bold(True)
    controls_font = pygame.font.SysFont(font_names, 22)
    symbol_font = pygame.font.SysFont(font_names, 24)

    controls_font_bold = pygame.font.SysFont(font_names, 22, bold=True)
    controls_font_regular = pygame.font.SysFont(font_names, 22)
    
    title_surface = title_font.render("Controls", True, GUI_TEXT_COLOR)
    
    title_rect_width = title_surface.get_width() + SYMBOL_SIZE + GUI_PADDING * 3
    title_rect = pygame.Rect(GUI_PADDING, GUI_PADDING, title_rect_width, title_surface.get_height() + GUI_PADDING)

    controls_data = [
        ("Left Click / Drag:", " Cycle cell state"),
        ("Right Click / Drag:", " Kill cell (state 0)"),
        ("Spacebar:", " Play / Pause simulation"),
        ("'C' Key:", " Clear the grid"),
    ]
    
    controls_surface_pairs = []
    for key, description in controls_data:
        key_surf = controls_font_bold.render(key, True, GUI_TEXT_COLOR)
        desc_surf = controls_font_regular.render(description, True, GUI_TEXT_COLOR)
        controls_surface_pairs.append((key_surf, desc_surf))

    speed_label_surf = controls_font_bold.render("Speed:", True, GUI_TEXT_COLOR)

    max_text_width = max(pair[0].get_width() + pair[1].get_width() for pair in controls_surface_pairs)
    dropdown_width = max(title_rect.width, max_text_width + GUI_PADDING * 2)
    
    slider_y_pos = title_rect.bottom + (controls_font_regular.get_height() + 5) * (len(controls_data) + 1)
    slider_bar_rect = pygame.Rect(title_rect.left + speed_label_surf.get_width() + GUI_PADDING, slider_y_pos, dropdown_width - speed_label_surf.get_width() - GUI_PADDING * 2, 8)

    dropdown_height = (slider_bar_rect.bottom - title_rect.bottom) + GUI_PADDING
    dropdown_rect = pygame.Rect(title_rect.left, title_rect.bottom + 5, dropdown_width, dropdown_height)
    
    while True:
        mouse_pos = pygame.mouse.get_pos()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                return
            
            elif event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1:
                    if title_rect.collidepoint(event.pos):
                        is_dropdown_open = not is_dropdown_open
                    elif is_dropdown_open and slider_bar_rect.inflate(0, 20).collidepoint(event.pos):
                        is_dragging_slider = True
                        click_offset = event.pos[0] - slider_bar_rect.x
                        speed_slider_value = max(0.0, min(1.0, click_offset / slider_bar_rect.width))
                    else:
                        drawing_mode = 1
                        painted_cells_this_drag.clear()
                        row, col = event.pos[1] // CELL_SIZE, event.pos[0] // CELL_SIZE
                        if 0 <= row < N_CELLS_Y and 0 <= col < N_CELLS_X:
                            cells[row, col] = (cells[row, col] + 1) % num_states
                            painted_cells_this_drag.add((row, col))
                elif event.button == 3:
                    drawing_mode = 3
                    painted_cells_this_drag.clear()
                    row, col = event.pos[1] // CELL_SIZE, event.pos[0] // CELL_SIZE
                    if 0 <= row < N_CELLS_Y and 0 <= col < N_CELLS_X:
                        cells[row, col] = 0
                        painted_cells_this_drag.add((row, col))

            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    running = not running
                elif event.key == pygame.K_c:
                    running = False
                    cells = np.zeros((N_CELLS_Y, N_CELLS_X))
            
            elif event.type == pygame.MOUSEBUTTONUP:
                drawing_mode = 0
                is_dragging_slider = False

            elif event.type == pygame.MOUSEMOTION:
                if is_dragging_slider:
                    click_offset = event.pos[0] - slider_bar_rect.x
                    speed_slider_value = max(0.0, min(1.0, click_offset / slider_bar_rect.width))
                elif drawing_mode != 0:
                    pos = event.pos
                    row, col = pos[1] // CELL_SIZE, pos[0] // CELL_SIZE
                    if (row, col) not in painted_cells_this_drag:
                        if 0 <= row < N_CELLS_Y and 0 <= col < N_CELLS_X:
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
        
        is_hovering = title_rect.collidepoint(mouse_pos)
        current_title_bg = GUI_ACCENT_COLOR if is_hovering else GUI_BG_COLOR
        
        pygame.draw.rect(screen, current_title_bg, title_rect, border_radius=GUI_BORDER_RADIUS)
        screen.blit(title_surface, (title_rect.left + GUI_PADDING, title_rect.centery - title_surface.get_height() // 2))
        
        symbol_cx = title_rect.right - GUI_PADDING - (SYMBOL_SIZE // 2)
        symbol_cy = title_rect.centery
        if is_dropdown_open:
            points = [(symbol_cx - SYMBOL_SIZE // 2, symbol_cy + SYMBOL_SIZE // 3), (symbol_cx + SYMBOL_SIZE // 2, symbol_cy + SYMBOL_SIZE // 3), (symbol_cx, symbol_cy - SYMBOL_SIZE // 2)]
        else:
            points = [(symbol_cx - SYMBOL_SIZE // 2, symbol_cy - SYMBOL_SIZE // 3), (symbol_cx + SYMBOL_SIZE // 2, symbol_cy - SYMBOL_SIZE // 3), (symbol_cx, symbol_cy + SYMBOL_SIZE // 2)]
        pygame.draw.polygon(screen, GUI_TEXT_COLOR, points)

        if is_dropdown_open:
            pygame.draw.rect(screen, GUI_BG_COLOR, dropdown_rect, border_radius=GUI_BORDER_RADIUS)
            current_y = dropdown_rect.top + 5
            for key_surf, desc_surf in controls_surface_pairs:
                screen.blit(key_surf, (dropdown_rect.left + GUI_PADDING, current_y))
                screen.blit(desc_surf, (dropdown_rect.left + GUI_PADDING + key_surf.get_width(), current_y))
                current_y += key_surf.get_height() + 5
            
                screen.blit(speed_label_surf, (dropdown_rect.left + GUI_PADDING, slider_bar_rect.centery - speed_label_surf.get_height()//2))
                pygame.draw.rect(screen, GUI_ACCENT_COLOR, slider_bar_rect, border_radius=4)
                handle_x = slider_bar_rect.x + speed_slider_value * slider_bar_rect.width
                handle_rect = pygame.Rect(0, 0, 12, slider_bar_rect.height + 8)
                handle_rect.center = (handle_x, slider_bar_rect.centery)
                pygame.draw.rect(screen, GUI_TEXT_COLOR, handle_rect, border_radius=6)
        
        pygame.display.update()
        
        min_sleep, max_sleep = 0.01, 1.0
        log_min, log_max = math.log(min_sleep), math.log(max_sleep)
        log_sleep = log_max - (speed_slider_value * (log_max - log_min))
        sleep_duration = math.exp(log_sleep)
        time.sleep(sleep_duration)

if __name__ == '__main__':
    main()