#include "Functions.h"
#include "Melodies.h"

// Визначення масиву (лише в одному місці)
const int ByeByeMelody[] = {
    600, 200,
    300, 200,
    800, 200,
    700, 200,
    600, 200,
    500, 200,
    400, 200,
    300, 200
};

// Визначення розміру (constexpr для оптимізації)
constexpr int sizeStartMelody = sizeof(ByeByeMelody) / sizeof(ByeByeMelody[0]);

// Реалізація функції (лише в одному місці)
void PlayMelody(int Melody) {
    switch (Melody) {
    case 1:
        for (int i = 0; i < sizeStartMelody; i += 2) {
            SmoothBeep(ByeByeMelody[i], ByeByeMelody[i + 1], 0.3);
        }
        break;
    default:
        break;
    }
}