#include "map.hh"
#include "utils.hh"

#include <string>

Map::Map(std::istream& stream)
{
    for (size_t x = 0; x < TAILLE_GRILLE; x++)
    {
        for (size_t y = 0; y < TAILLE_GRILLE; y++)
        {
            std::string ressources_str;
            stream >> ressources_str;
            const auto cell_ressources = split(ressources_str, ",");

            for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
                ressources[x][y][k] = std::stoi(cell_ressources[k]);
        }
    }

    for (const int player : {0, 1})
    {
        size_t nb_plants;
        stream >> nb_plants;

        for (size_t i = 0; i < nb_plants; i++)
        {
            plante plant;

            stream >> plant.pos.x >> plant.pos.y >> plant.vie_max >>
                plant.force >> plant.elegance >> plant.rayon_deplacement >>
                plant.rayon_collecte;

            for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
                stream >> plant.consommation[k];

            plant.jardinier = player;
            plant.adulte = true;
            plant.enracinee = false;
            plant.age = AGE_DE_POUSSE;
            plant.vie = plant.vie_max;

            plants[plant.pos.x][plant.pos.y] = plant;
        }
    }
}
