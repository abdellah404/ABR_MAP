#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <map>
#include <vector>

using namespace std;

// =====================================================
// 1. NOEUD D'UN ARBRE BINAIRE
// =====================================================
class Noeud {
public:
    int valeur;
    Noeud* gauche;
    Noeud* droite;

    Noeud(int v) {
        valeur = v;
        gauche = nullptr;
        droite = nullptr;
    }
};

// =====================================================
// 2. ARBRE BINAIRE DE RECHERCHE (FROM SCRATCH)
// =====================================================
class ArbreBinaireRecherche {
private:
    Noeud* racine;
    int nombreNoeuds;

public:
    ArbreBinaireRecherche() {
        racine = nullptr;
        nombreNoeuds = 0;
    }

    // La copie est interdite pour eviter les doubles delete.
    ArbreBinaireRecherche(const ArbreBinaireRecherche&) = delete;
    ArbreBinaireRecherche& operator=(const ArbreBinaireRecherche&) = delete;

    void inserer(int valeur) {
        if (racine == nullptr) {
            racine = new Noeud(valeur);
            nombreNoeuds++;
            return;
        }

        Noeud* courant = racine;
        while (true) {
            if (valeur < courant->valeur) {
                if (courant->gauche == nullptr) {
                    courant->gauche = new Noeud(valeur);
                    nombreNoeuds++;
                    return;
                }
                courant = courant->gauche;
            }
            else if (valeur > courant->valeur) {
                if (courant->droite == nullptr) {
                    courant->droite = new Noeud(valeur);
                    nombreNoeuds++;
                    return;
                }
                courant = courant->droite;
            }
            else {
                return; // Pas de doublons, comme les cles de map.
            }
        }
    }

    bool rechercher(int valeur) const {
        Noeud* courant = racine;
        while (courant != nullptr) {
            if (valeur == courant->valeur) return true;
            if (valeur < courant->valeur)
                courant = courant->gauche;
            else
                courant = courant->droite;
        }
        return false;
    }

    bool supprimer(int valeur) {
        Noeud* courant = racine;
        Noeud* parent = nullptr;

        // Trouver le noeud a supprimer et son parent.
        while (courant != nullptr && courant->valeur != valeur) {
            parent = courant;
            if (valeur < courant->valeur)
                courant = courant->gauche;
            else
                courant = courant->droite;
        }
        if (courant == nullptr) return false;

        // Si deux fils : remplacer par le maximum du fils gauche.
        if (courant->gauche != nullptr && courant->droite != nullptr) {
            Noeud* maxGauche = courant->gauche;
            Noeud* parentMax = courant;

            while (maxGauche->droite != nullptr) {
                parentMax = maxGauche;
                maxGauche = maxGauche->droite;
            }
            courant->valeur = maxGauche->valeur;
            courant = maxGauche;
            parent = parentMax;
        }

        // Ici le noeud a au plus un fils.
        Noeud* enfant = courant->gauche;
        if (enfant == nullptr) enfant = courant->droite;

        if (parent == nullptr)
            racine = enfant;
        else if (parent->gauche == courant)
            parent->gauche = enfant;
        else
            parent->droite = enfant;

        delete courant;
        nombreNoeuds--;
        return true;
    }

    int taille() const {
        return nombreNoeuds;
    }

    ~ArbreBinaireRecherche() {
        // Liberation iterative, meme lorsque l'arbre est en chaine.
        vector<Noeud*> pile;
        if (racine != nullptr) pile.push_back(racine);

        while (!pile.empty()) {
            Noeud* courant = pile.back();
            pile.pop_back();
            if (courant->gauche != nullptr) pile.push_back(courant->gauche);
            if (courant->droite != nullptr) pile.push_back(courant->droite);
            delete courant;
        }
    }
};

// =====================================================
// 3. MAP DE LA STL
// =====================================================
class MapRecherche {
private:
    map<int, int> donnees;

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

#endif // STRUCTURES_H
