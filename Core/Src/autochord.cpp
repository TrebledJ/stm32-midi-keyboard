#include "autochord.hpp"
#include "settings.hpp"


namespace autochord
{
    chord autonotes(Note note, AutoChord strategy)
    {
        if (strategy == AUTOCHORD_NONE) {
            return {note};
        } else if (strategy == AUTOCHORD_PERFECT) {
            return {note, note + 7, note + 12};
        } else if (strategy == AUTOCHORD_TRIADS) {
            switch (get_scale_degree(note, settings::curr().key)) {
                case ScaleDegree::MAJOR: return major_triad(note);
                case ScaleDegree::MINOR: return minor_triad(note);
                default:
                    // TODO: How to handle???
                    return {note};
            }
        } else if (strategy == AUTOCHORD_OPEN_CHORD) {
            switch (get_scale_degree(note, settings::curr().key)) {
                case ScaleDegree::MAJOR: return open_major_triad(note);
                case ScaleDegree::MINOR: return open_minor_triad(note);
                default:
                    // TODO: How to handle???
                    return {note};
            }
        }
        return {};
    }
} // namespace autochord
