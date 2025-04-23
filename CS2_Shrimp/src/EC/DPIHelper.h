#pragma once
#ifndef DPISCALING_H
#define DPISCALING_H

class DPIScaling {
public:
    static float GetSystemScale();

private:
    static float GetModernDPI();
    static float GetCompatibleDPI();
};

#endif // DPISCALING_H