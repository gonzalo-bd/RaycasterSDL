#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <float.h>

#define EPSILON DBL_EPSILON

#define mapWidth 24
#define mapHeight 24
#define screenWidth 800
#define screenHeight 640

#define TARGET_FPS 60
#define FRAME_TIME (1000 / TARGET_FPS) // Target frame time in milliseconds

SDL_Renderer *renderer;

int worldMap[mapWidth][mapHeight] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

void drawVLine(Sint16 x, Sint16 by, Sint16 ey, Uint32 color)
{
    // Extract RGB components from color (hex format 0xRRGGBB)
    Uint8 r = (color >> 16) & 0xFF;
    Uint8 g = (color >> 8) & 0xFF;
    Uint8 b = color & 0xFF;

    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, x, by, x, ey); // Draw the vertical line at once
}

void drawMinimap(double posX, double posY, double dirX, double dirY)
{
    // Define the size of the minimap and calculate tile size dynamically
const int minimapSize = 200;       // Total size of the minimap in pixels
const int minimapXOffset = 20;     // Offset from the left side of the screen
const int minimapYOffset = 20;     // Offset from the top of the screen

// Calculate tile size based on the minimap size and the map dimensions
int minimapTileSize = minimapSize / ((mapWidth > mapHeight) ? mapWidth : mapHeight);

// Draw the minimap
for (int y = 0; y < mapHeight; y++) {
    for (int x = 0; x < mapWidth; x++) {
        if (worldMap[x][y] > 0) { // Wall
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        } else { // Empty space
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
        }

        // Render the cell as a filled square
        SDL_Rect tileRect = {
            minimapXOffset + x * minimapTileSize,
            minimapYOffset + y * minimapTileSize,
            minimapTileSize,
            minimapTileSize
        };
        SDL_RenderFillRect(renderer, &tileRect);
    }
}

// Draw the player's position on the minimap
int playerMapX = minimapXOffset + (int)(posX * minimapTileSize);
int playerMapY = minimapYOffset + (int)(posY * minimapTileSize);
SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE); // Red color for player
SDL_Rect playerRect = {playerMapX - 2, playerMapY - 2, 4, 4}; // Player as small square
SDL_RenderFillRect(renderer, &playerRect);

// Draw the rays casted by the DDA algorithm on the minimap
for (int x = 0; x < screenWidth; x += 10) { // Step by 10 for better performance on the minimap
    double cameraX = 2 * x / (double)screenWidth - 1; // Camera plane x-coordinate
    double rayDirX = dirX + 0 * cameraX;
    double rayDirY = dirY + 0.66 * cameraX;

    // Map position and distance calculations (similar to your DDA algorithm)
    int mapX = (int)posX;
    int mapY = (int)posY;
    double sideDistX, sideDistY;
    double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1.0 / rayDirX);
    double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1.0 / rayDirY);
    int stepX, stepY;
    int hit = 0;

    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
    }
    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
    }

    // Perform DDA to find where the ray hits a wall
    while (hit == 0) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
        }
        if (worldMap[mapX][mapY] > 0) hit = 1; // Wall hit
    }

    // Calculate the endpoint of the ray on the minimap
    double hitX = posX + rayDirX * (hit ? (sideDistX - deltaDistX) : (sideDistY - deltaDistY));
    double hitY = posY + rayDirY * (hit ? (sideDistX - deltaDistX) : (sideDistY - deltaDistY));
    int rayEndX = minimapXOffset + (int)(hitX * minimapTileSize);
    int rayEndY = minimapYOffset + (int)(hitY * minimapTileSize);

    // Draw the ray on the minimap
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE); // Green for rays
    SDL_RenderDrawLine(renderer, playerMapX, playerMapY, rayEndX, rayEndY);
}

   
}

int main(int argc, char *argv[])
{

    // Raycaster Variables
    double posX = 22, posY = 12;      // x and y starting positions
    double dirX = -1, dirY = 0;       // Initial direction vector
    double planeX = 0, planeY = 0.66; // Camera plane

    double time = 0;    // Time of current frame
    double oldTime = 0; // Time of previous frame

    printf("Initializing SDL...\n");
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    printf("Creating window...\n");
    SDL_Window *window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Event windowEvent;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer)
    {
        fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    while (1)
    {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        for (int x = 0; x < screenWidth; x++)
        {
            // Calculate ray position and direction
            double cameraX = 2 * x / (double)screenWidth - 1; // The x-coordinate on the camera plane that the current x-coodinate represents.
            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;

            // Which box of the map we are in
            int mapX = (int)posX;
            int mapY = (int)posY;

            // Length of ray from origin to next y or x side
            double sideDistX;
            double sideDistY;

            // Length of ray from one x or y side to the next
            double epsilon = 1e-9;
            double deltaDistX = (fabs(rayDirX) < EPSILON) ? 1e30 : fabs(1.0 / (double)rayDirX); // Fixed: gotta use fabs
            double deltaDistY = (fabs(rayDirY) < EPSILON) ? 1e30 : fabs(1.0 / (double)rayDirY);
            double perpWallDist;



            // What direction to step in x or y (+1 or -1)
            int stepX;
            int stepY;

            int hit = 0; // wall hit?
            int side;    // NS or EW hit? 0 and 1 respectively

            // Calculate step and initial sideDist
            if (rayDirX < 0)
            {
                stepX = -1;
                sideDistX = (posX - mapX) * deltaDistX;
            }
            else
            {
                stepX = 1;
                sideDistX = (posX + 1.0 - mapX) * deltaDistX;
            }

            if (rayDirY < 0)
            {
                stepY = -1;
                sideDistY = (posY - mapY) * deltaDistY;
            }
            else
            {
                stepY = 1;
                sideDistY = (posY + 1.0 - mapY) * deltaDistY;
            }

            // Perform DDA
            while (hit == 0)
            {
                // jump to next map square either in x-direction or in y
                if (sideDistX < sideDistY)
                {
                    mapX += stepX;
                    sideDistX += deltaDistX;
                    side = 0;
                }
                else
                {
                    mapY += stepY;
                    sideDistY += deltaDistY;
                    side = 1;
                }

                // Check if we hit a wall
                if (worldMap[mapX][mapY] > 0)
                    hit = 1;


            }

            // Calculate distance perpendicular to camera plane (Euclidean is fisheye :/)
            if (side == 0)
                perpWallDist = (sideDistX - deltaDistX);
            else
                perpWallDist = (sideDistY - deltaDistY);

            // Calculate height of line to draw on screen
            int lineHeight = (int)(screenHeight / perpWallDist);

            // calculate lowest and highest pixel to fill in current stripe
            int drawStart = -lineHeight / 2 + screenHeight / 2;
            if (drawStart < 0)
                drawStart = 0;
            int drawEnd = lineHeight / 2 + screenHeight / 2;
            if (drawEnd >= screenHeight)
                drawEnd = screenHeight - 1;

            // Choose wall color
            Uint32 color;
            switch (worldMap[mapX][mapY])
            {
            case 1:
                color = 0xFF0000;
                break; // red
            case 2:
                color = 0x00FF00;
                break; // green
            case 3:
                color = 0x0000FF;
                break; // blue
            case 4:
                color = 0xFFFFFF;
                ;
                break; // white
            default:
                color = 0xFFFF00;
                break; // yellow
            }

            // give x and y sides different brightness
            if (side == 1)
                color = color / 2;

            drawVLine(x, drawStart, drawEnd, color);
        }

        drawMinimap(posX, posY, dirX, dirY);

        oldTime = time;
        time = SDL_GetTicks();
        double frameTime = (time - oldTime) / 1000.0; // frameTime is the time this frame has taken, in seconds
        if (frameTime < FRAME_TIME)
        {
            SDL_Delay(FRAME_TIME - frameTime); // Delay to limit FPS
        }
        // printf("FPS: %f", 1.0/frameTime);
        SDL_RenderPresent(renderer);
        // speed modifiers
        double moveSpeed = frameTime * 5.0; // the constant value is in squares/second
        double rotSpeed = frameTime * 3.0;  // the constant value is in radians/second

        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
                break;
            if (SDL_KEYDOWN)
            {
                // Handle key presses
                if (windowEvent.key.keysym.sym == SDLK_ESCAPE)
                {
                    break; // Exit on Escape key press
                }
                else if (windowEvent.key.keysym.sym == SDLK_w)
                {
                    int newPosX = (int)(posX + dirX * moveSpeed);
                    int newPosY = (int)(posY + dirY * moveSpeed);
                    // Check bounds and obstacles
                    if (newPosX >= 0 && newPosX < mapWidth && worldMap[newPosX][(int)posY] == 0)
                        posX += dirX * moveSpeed;
                    if (newPosY >= 0 && newPosY < mapHeight && worldMap[(int)posX][newPosY] == 0)
                        posY += dirY * moveSpeed;
                }
                else if (windowEvent.key.keysym.sym == SDLK_a)
                {
                    // both camera direction and camera plane must be rotated
                    double oldDirX = dirX;
                    dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
                    dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
                    double oldPlaneX = planeX;
                    planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
                    planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
                }
                else if (windowEvent.key.keysym.sym == SDLK_s)
                {
                    int newPosX = (int)(posX - dirX * moveSpeed);
                    int newPosY = (int)(posY - dirY * moveSpeed);
                    // Check bounds and obstacles
                    if (newPosX >= 0 && newPosX < mapWidth && worldMap[newPosX][(int)posY] == 0)
                        posX -= dirX * moveSpeed;
                    if (newPosY >= 0 && newPosY < mapHeight && worldMap[(int)posX][newPosY] == 0)
                        posY -= dirY * moveSpeed;
                }
                else if (windowEvent.key.keysym.sym == SDLK_d)
                {
                    // both camera direction and camera plane must be rotated
                    double oldDirX = dirX;
                    dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
                    dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
                    double oldPlaneX = planeX;
                    planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
                    planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
                }
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("Exiting...\n");
    return 0;
}
