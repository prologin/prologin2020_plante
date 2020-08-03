#include "rules.hh"

template <typename T>
static void dump_vector(std::vector<T> vector, std::ostream& ss,
                        std::function<void(std::ostream&, T)> func)
{
    bool is_first = true;
    ss << "[";
    for (T elem : vector)
    {
        if (!is_first)
            ss << ", ";
        func(ss, elem);
        is_first = false;
    }
    ss << "]";
}

/// Decodes a UTF-8 string to a list of 32 bit unicode codepoints. Ignores
/// erroneous characters.
/// Imported from prologin2016
static std::u32string utf8_decode(const std::string& s)
{
    std::u32string ret;
    size_t i = 0;
    size_t size = s.size();

    while (i < size)
    {
        if ((s[i] & 0x80) == 0)
        {
            ret.push_back(s[i++]);
        }
        else if ((s[i] & 0xe0) == 0xc0 && (i + 1) < size &&
                 (s[i + 1] & 0xc0) == 0x80)
        {
            ret.push_back(((s[i] & 0x1f) << 6) | (s[i + 1] & 0x3f));
            i += 2;
        }
        else if ((s[i] & 0xf0) == 0xe0 && (i + 2) < size &&
                 (s[i + 1] & 0xc0) == 0x80 && (s[i + 2] & 0xc0) == 0x80)
        {
            ret.push_back(((s[i] & 0x0f) << 12) | ((s[i + 1] & 0x3f) << 6) |
                          (s[i + 2] & 0x3f));
            i += 3;
        }
        else if ((s[i] & 0xf8) == 0xf0 && (i + 3) < size &&
                 (s[i + 1] & 0xc0) == 0x80 && (s[i + 2] & 0xc0) == 0x80 &&
                 (s[i + 1] & 0xc0) == 0x80)
        {
            ret.push_back(((s[i] & 0x07) << 18) | ((s[i + 1] & 0x3f) << 12) |
                          ((s[i + 2] & 0x3f) << 6) | (s[i + 3] & 0x3f));
            i += 4;
        }
        else
        {
            i++;
        }
    }

    return ret;
}

/// Dump a JSON-escaped UTF-8 string
/// Imported from prologin2016
static void dump_string(std::ostream& ss, const std::string& s)
{
    /*
     * RFC4627, 2.5:
     * All Unicode characters may be placed within the quotation marks except
     * for the characters that must be escaped: quotation mark, reverse solidus,
     * and the control characters (U+0000 through U+001F).
     */
    std::ios state(nullptr);
    state.copyfmt(ss);
    std::u32string utf32 = utf8_decode(s);
    ss << "\"";
    for (char32_t& c : utf32)
    {
        if (c == u'"')
        {
            ss << "\\\"";
        }
        else if (c == u'\\')
        {
            ss << "\\\\";
        }
        else if (u'\u0020' <= c && c <= u'\u007E')
        {
            // printable ASCII
            ss << static_cast<char>(c);
        }
        else if (c > u'\uFFFF')
        {
            // surrogate pair
            // http://unicode.org/faq/utf_bom.html#utf16-2
            const unsigned s = c - 0x010000;
            const unsigned lead = (s >> 10) + 0xD800;
            const unsigned trail = (s & 0x3FF) + 0xDC00;
            ss << "\\u" << std::hex << std::setfill('0') << std::setw(4)
               << lead;
            ss.copyfmt(state);
            ss << "\\u" << std::hex << std::setfill('0') << std::setw(4)
               << trail;
            ss.copyfmt(state);
        }
        else
        {
            ss << "\\u" << std::hex << std::setfill('0') << std::setw(4) << c;
            ss.copyfmt(state);
        }
    }
    ss << '"';
}

static void dump_map(std::ostream& ss, const Map& map)
{
    ss << ", \"carte\": \"";
    for (int y = 0; y < TAILLE_GRILLE; ++y)
    {
        for (int x = 0; x < TAILLE_GRILLE; ++x)
        {
            const auto& res = map.ressources_at({x, y});
            for (size_t k = 0; k < NB_TYPES_RESSOURCES - 1; ++k)
                ss << res[k] << ",";
            ss << res[NB_TYPES_RESSOURCES - 1];
            if (x < TAILLE_GRILLE - 1)
                ss << " ";
        }
        if (y < TAILLE_GRILLE - 1)
            ss << "\\n";
    }
    ss << "\"";
}

static void dump_plants(std::ostream& ss, const GameState& st, int player_id)
{
    const auto& plants = st.get_map().player_plants(player_id);
    dump_vector<plante>(plants, ss, [](auto& ss, auto plant) {
        ss << "{\"pos\": ";
        to_json(ss, plant.pos);
        ss << ", \"adulte\": " << plant.adulte;
        ss << ", \"enracinée\": " << plant.enracinee;
        ss << ", \"vie\": " << plant.vie;
        ss << ", \"vie_max\": " << plant.vie_max;
        ss << ", \"force\": " << plant.force;
        ss << ", \"élégance\": " << plant.elegance;
        ss << ", \"rayon_déplacement\": " << plant.rayon_deplacement;
        ss << ", \"rayon_collecte\": " << plant.rayon_collecte;
        ss << ", \"consommation\": ";
        dump_vector<int>(plant.consommation, ss,
                         [](auto& ss, auto res) { ss << res; });
        ss << ", \"age\": " << plant.age << "}";
    });
}

static void dump_history(std::ostream& ss, const GameState&,
                         const PlayerInfo& player)
{
    const auto& actions = player.get_internal_history();
    dump_vector<internal_action>(
        actions, ss, [](auto& ss, auto action) { ss << action.json; });
}

static void dump_players(std::ostream& ss, const GameState& st)
{

    const auto& players = st.get_player_info();
    std::vector<std::pair<int, PlayerInfo>> players_vec;

    for (size_t id = 0; id < 2; ++id)
        players_vec.emplace_back(id, players[id]);

    dump_vector<std::pair<int, PlayerInfo>>(
        players_vec, ss, [&](auto& ss, auto player) {
            ss << "{ \"id\": " << player.first << ", \"name\": ";
            dump_string(ss, player.second.get_name());
            ss << ", \"score\": " << player.second.get_score();
            ss << ", \"plantes\": ";
            dump_plants(ss, st, player.first);
            ss << ", \"historique\": ";
            dump_history(ss, st, player.second);
            ss << "}";
        });
}

static void dump_stream(std::ostream& ss, const GameState& st)
{
    ss << "{";
    ss << "\"tour\": [" << st.get_round() << ", " << NB_TOURS << "] ";
    ss << ", \"joueurs\": ";
    dump_players(ss, st);
    if (st.get_round() == 0)
        dump_map(ss, st.get_map());
    ss << "},\n";
}

void Rules::dump_state(std::ostream& ss)
{
    dump_stream(ss, api_->game_state());
}

// from api.cc
extern Api* api;

extern "C" const char* dump_state_json()
{
    // Warning: everytime this function is called, it invalidates the previous
    // return values by free-ing them.
    static std::string s;
    std::ostringstream ss;
    dump_stream(ss, api->game_state());
    s = ss.str();
    return s.c_str();
}
