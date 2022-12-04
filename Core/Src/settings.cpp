#include "settings.hpp"
#include "utils/magic_enum.hpp"


void settings::update()
{
    const settings& c = curr();
    settings& p       = prev();
    p                 = c;
}
