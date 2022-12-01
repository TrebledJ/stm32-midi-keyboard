#include "settings.hpp"

void settings::update()
{
    const settings& c = curr();
    settings& p       = prev();
    if (c.transpose != p.transpose) {
        // Update transpose.
        // speaker::on_transpose(c.transpose);
        p.transpose = c.transpose;
    }
}
