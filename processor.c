#include <stdio.h>
#include <stdlib.h>
// https://stackoverflow.com/questions/30097953/ascii-art-sorting-an-array-of-ascii-characters-by-brightness-levels-c-c
#define BRIGHT_MAP  " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@"
#define HEIGHT 150
#define WIDTH 200

struct pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

struct Result {
    struct pixel **pixels;
    int **lightmap;
};


struct pixel **create_pixel_matrix() {
    struct pixel **matrix = malloc(HEIGHT * sizeof(struct pixel*));
    for (int i = 0; i < HEIGHT; i++) {
        matrix[i] = malloc(WIDTH * sizeof(struct pixel));
    }
    return matrix;
}

int **create_light_matrix() {
    int **matrix = malloc(HEIGHT * sizeof(int*));
    for (int i = 0; i < HEIGHT; i++) {
        matrix[i] = malloc(WIDTH * sizeof(int));
    }
    return matrix;
}

struct Result *read_pixels(FILE *in, int width, int height) {
    struct pixel **pixels = create_pixel_matrix();
    int **lightmap = create_light_matrix();
    float widthRatio = (float)width / WIDTH;
    float heightRatio = (float)height / HEIGHT;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int startRow = i * heightRatio;
            int endRow = (i + 1) * heightRatio;
            int startCol = j * widthRatio;
            int endCol = (j + 1) * widthRatio;

            int totalBrightness = 0;
            int totalColor[] = {0, 0, 0};
            int count = 0;
            // https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color
            for (int row = startRow; row < endRow; row++) {
                for (int col = startCol; col < endCol; col++) {
                    //struct pixel p;
                    //p.red = fgetc(in);
                    //p.green = fgetc(in);
                    //p.blue = fgetc(in);
                    //totalBrightness += (0.2126 * p.red + 0.7152 * p.green + 0.0722 * p.blue);
                    totalColor[0] += p.red;
                    totalColor[1] += p.green;
                    totalColor[2] += p.blue;
                    count++;
                }
            }

            int averageBrightness = totalBrightness / count;
            int index = (int)(averageBrightness * 69 / 255);
            struct pixel p;
            p.red = totalColor[0] / count;
            p.green = totalColor[1] / count;
            p.blue = totalColor[2] / count;
            pixels[i][j] = p;
            lightmap[i][j] = index;
        }
    }
    struct Result *result = malloc(sizeof(struct Result));
    result->pixels = pixels;
    result->lightmap = lightmap;
    return result;
}

void print_ascii_art(FILE *out, int width, int height, struct pixel **pixels, int **lightmap) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int brightness = lightmap[i][j];
            int index = (int)(brightness * 69 / 255);
            fprintf(out, "%c", BRIGHT_MAP[index]);
        }
        fprintf(out, "\n");
    }
}

int main() {
    FILE *in = fopen("dd.ppm", "rb");
    char magic_number[3];
    fscanf(in, "%s", magic_number);
    int width, height, depth;
    fscanf(in, "%d %d %d", &width, &height, &depth);
    struct Result *result = read_pixels(in, width, height);
    fclose(in);

    FILE *out = fopen("ascii_art.txt", "w");
    print_ascii_art(out, WIDTH, HEIGHT, result->pixels, result->lightmap);
    fclose(out);

    return 0;

}
