#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include <string>
#include <cmath>

using namespace cs225;


void rotate(std::string inputFile, std::string outputFile) {
  // Part 2
	PNG image;
    
	image.readFromFile(inputFile);

	PNG result(image); // make a copy

    unsigned w = image.width();
    unsigned h = image.height();

	for (unsigned x = 0; x < w; x++) {
		for (unsigned y = 0; y < h; y++) {

			HSLAPixel * pixel0 = image.getPixel(x,y);

			HSLAPixel * pixel = result.getPixel(w - 1 - x,h - 1 - y); // index starts from (0,0) to (w-1,h-1)

			* pixel =  * pixel0;

		}
	}

	result.writeToFile(outputFile);
}

PNG myArt(unsigned int width, unsigned int height) {
  PNG png(width, height);
  // Part 3
  
  unsigned vertex_x = std::floor(width * 0.5);
  unsigned vertex_y = std::floor(height * 0.9);
  double coefficient = 2.0 / width; // suppose width = x, c * (x/2)^2 = x/2 --> c*x=2 --> c = 2/x

  for (unsigned x = 0; x < width; x++) {
		for (unsigned y = 0; y < height; y++) {


			double dist2hori = int(vertex_y - y); // distance to horizon at 0.9y 
			// Caveat: the subtraction between unsigned int will have overflow issue, use type convertion to int!

			double dist2bola = dist2hori - coefficient * pow(int(vertex_x - x),2); // above parabola, > 0; beneath, < 0

			double interpolate = dist2bola / height;

			HSLAPixel * pixel = png.getPixel(x,y);

			pixel -> h = double(x)/double(width) * 60.0; // use color in red-yellow range

			pixel -> s = 1.0;

			int sign = (interpolate < 0) ? -1 : 1;

			pixel -> l = 0.5 + 0.5 * sign * sqrt(abs(interpolate)); // accelerate the luminance change by quadratic dampening

			pixel -> a = 1.0 - abs(interpolate); // linear dampening

		}
   }

  return png;
}
