#include "LineManager.h"
using namespace std;

LineManager::LineManager(const Vector<Line *>& lines) {

    this->lines.addAll(lines);
}

LineManager::~LineManager() {}

double LineManager::contentHeight() const {

    return lines.back()->highY();
}

Vector<Line *> LineManager::linesInRange(double lowY, double highY) const {

    Vector<Line *> result;

    for (Line *line: lines) {
        if (line->highY() >= lowY && line->lowY() <= highY) {
            result.add(line);
        }
    }

    return result;
}

Line* LineManager::lineAt(double y) const {

    for (Line *line: lines) {

        if (y >= line->lowY() && y <= line->highY()) {
            return line;
        }
    }

    return nullptr;
}

