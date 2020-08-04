#include "history.hh"

void death_dump_json(std::ostream& ss, position pos)
{
    ss << '{';
    ss << R"("action_type":"death",)";
    ss << R"("position":)";
    to_json(ss, pos);
    ss << '}';
}
