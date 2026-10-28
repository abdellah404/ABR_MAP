#ifndef ARBRE_BINAIRE_RECHERCHE_H
#define ARBRE_BINAIRE_RECHERCHE_H

// Un noeud contient une valeur et deux fils.
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

// Arbre binaire de recherche implemente sans conteneur STL.
class ArbreBinaireRecherche {
private:
    Noeud* racine;
    int nombreNoeuds;

    void vider(Noeud* node) {
        if (node == nullptr) return;
        vider(node->gauche);
        vider(node->droite);
        delete node;
    }

public:
    ArbreBinaireRecherche() {
        racine = nullptr;
        nombreNoeuds = 0;
    }

    // Un arbre contenant des pointeurs ne doit pas etre copie directement.
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
                return; // Pas de doublons, comme dans std::map.
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

        // Rechercher le noeud a supprimer.
        while (courant != nullptr && courant->valeur != valeur) {
            parent = courant;
            if (valeur < courant->valeur)
                courant = courant->gauche;
            else
                courant = courant->droite;
        }

        if (courant == nullptr) return false;

        // Si le noeud a deux fils, utiliser le maximum a gauche.
        if (courant->gauche != nullptr && courant->droite != nullptr) {
            Noeud* maximumGauche = courant->gauche;
            Noeud* parentMaximum = courant;

            while (maximumGauche->droite != nullptr) {
                parentMaximum = maximumGauche;
                maximumGauche = maximumGauche->droite;
            }

            courant->valeur = maximumGauche->valeur;
            courant = maximumGauche;
            parent = parentMaximum;
        }

        // Le noeud possede maintenant au maximum un fils.
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
        vider(racine);
    }
};

#endif
