#ifndef MAP_RECHERCHE_H
#define MAP_RECHERCHE_H

#include <map>

// La meme interface que l'ABR, avec std::map.
class MapRecherche {
private:
    std::map<int, int> donnees;

public:
    void inserer(int valeur) {
        donnees.emplace(valeur, valeur);
    }

    bool rechercher(int valeur) const {
        return donnees.find(valeur) != donnees.end();
    }

    bool supprimer(int valeur) {
        return donnees.erase(valeur) > 0;
    }

    int taille() const {
        return static_cast<int>(donnees.size());
    }
};

#endif
