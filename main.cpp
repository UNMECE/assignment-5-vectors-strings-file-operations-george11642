#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "pixel.h"

static bool parse_pixel_line(const std::string &line, Pixel &out_pixel)
{
    // Parse comma-separated values using find/substr per assignment guidance
    size_t p0 = 0;
    size_t p1 = line.find(',', p0);
    if (p1 == std::string::npos) return false;
    out_pixel.x = std::stoi(line.substr(p0, p1 - p0));

    size_t p2 = line.find(',', p1 + 1);
    if (p2 == std::string::npos) return false;
    out_pixel.y = std::stoi(line.substr(p1 + 1, p2 - (p1 + 1)));

    size_t p3 = line.find(',', p2 + 1);
    if (p3 == std::string::npos) return false;
    out_pixel.r = std::stof(line.substr(p2 + 1, p3 - (p2 + 1)));

    size_t p4 = line.find(',', p3 + 1);
    if (p4 == std::string::npos) return false;
    out_pixel.g = std::stof(line.substr(p3 + 1, p4 - (p3 + 1)));

    // Remaining substring is b
    out_pixel.b = std::stof(line.substr(p4 + 1));
    return true;
}

void average_colors(std::vector<Pixel> &pixel_list)
{
    if (pixel_list.empty()) {
        std::cout << "Average R: 0\nAverage G: 0\nAverage B: 0" << std::endl;
        return;
    }

    double sum_r = 0.0, sum_g = 0.0, sum_b = 0.0;
    for (const auto &px : pixel_list) {
        sum_r += px.r;
        sum_g += px.g;
        sum_b += px.b;
    }
    const double count = static_cast<double>(pixel_list.size());

    std::cout << std::setprecision(10)
              << "Average R: " << (sum_r / count) << '\n'
              << "Average G: " << (sum_g / count) << '\n'
              << "Average B: " << (sum_b / count) << std::endl;
}

void flip_vertically(std::vector<Pixel> &pixel_list)
{
    if (pixel_list.empty()) return;

    int max_y = 0;
    for (const auto &px : pixel_list) {
        if (px.y > max_y) max_y = px.y;
    }

    for (auto &px : pixel_list) {
        px.y = max_y - px.y;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_filename>" << std::endl;
        return 1;
    }

    const std::string input_filename = argv[1];
    std::ifstream input(input_filename);
    if (!input.is_open()) {
        std::cerr << "Error: Could not open file " << input_filename << std::endl;
        return 1;
    }

    std::vector<Pixel> pixel_list;
    pixel_list.reserve(131072); // 512 * 256 as a hint from README

    std::string line;
    size_t line_number = 0;
    while (std::getline(input, line)) {
        ++line_number;
        if (line.empty()) continue;
        Pixel px{};
        if (parse_pixel_line(line, px)) {
            pixel_list.push_back(px);
        } else {
            // Skip malformed line but notify
            std::cerr << "Warning: Failed to parse line " << line_number << std::endl;
        }
    }
    input.close();

    std::cout << "Read " << pixel_list.size() << " pixels from file." << std::endl;

    // Step 4: compute averages
    average_colors(pixel_list);

    // Step 4: flip vertically
    flip_vertically(pixel_list);

    // Step 5: write flipped data
    std::ofstream output("flipped.dat");
    if (!output.is_open()) {
        std::cerr << "Error: Could not create output file flipped.dat" << std::endl;
        return 1;
    }
    output << std::setprecision(17);
    for (const auto &px : pixel_list) {
        output << px.x << ',' << px.y << ',' << px.r << ',' << px.g << ',' << px.b << '\n';
    }
    output.close();

    std::cout << "Flipped data saved to flipped.dat" << std::endl;
    return 0;
}


