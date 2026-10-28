#ifndef CHRONO_TEST_H
#define CHRONO_TEST_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>

#include "ArbreBinaireRecherche.h"
#include "MapRecherche.h"

class ChronoTest {
private:
    struct Resultats {
        double insertion = 0;
        double recherchePresente = 0;
        double rechercheAbsente = 0;
        double suppression = 0;
        int trouves = 0;
        int absents = 0;
        int supprimes = 0;
        int tailleFinale = 0;
    };

    // Les memes operations sont executees sur l'ABR et sur Map.
    template <class Structure>
    Resultats mesurer(const std::vector<int>& nombres,
                      const std::vector<int>& questions) {
        Structure structure;
        Resultats r;

        auto debut = std::chrono::steady_clock::now();
        for (int valeur : nombres)
            structure.inserer(valeur);
        auto fin = std::chrono::steady_clock::now();
        r.insertion = std::chrono::duration<double, std::milli>(fin - debut).count();

        debut = std::chrono::steady_clock::now();
        for (int valeur : questions)
            if (structure.rechercher(valeur)) r.trouves++;
        fin = std::chrono::steady_clock::now();
        r.recherchePresente = std::chrono::duration<double, std::milli>(fin - debut).count();

        // Les cles presentes sont impaires : valeur + 1 est absente.
        debut = std::chrono::steady_clock::now();
        for (int valeur : questions)
            if (!structure.rechercher(valeur + 1)) r.absents++;
        fin = std::chrono::steady_clock::now();
        r.rechercheAbsente = std::chrono::duration<double, std::milli>(fin - debut).count();

        debut = std::chrono::steady_clock::now();
        for (int valeur : questions)
            if (structure.supprimer(valeur)) r.supprimes++;
        fin = std::chrono::steady_clock::now();
        r.suppression = std::chrono::duration<double, std::milli>(fin - debut).count();

        r.tailleFinale = structure.taille();
        return r;
    }

    std::vector<int> creerNombres(int n, bool aleatoire) {
        std::vector<int> nombres;
        for (int i = 0; i < n; i++)
            nombres.push_back(2 * i + 1);

        if (aleatoire) {
            std::mt19937 generateur(42);
            std::shuffle(nombres.begin(), nombres.end(), generateur);
        }
        return nombres;
    }

    std::vector<int> creerQuestions(const std::vector<int>& nombres) {
        std::vector<int> questions;
        int nombreTests = static_cast<int>(nombres.size()) / 10;
        for (int i = 0; i < nombreTests; i++)
            questions.push_back(nombres[i * 10]);
        return questions;
    }

    void afficherResultats(const std::string& nom, const Resultats& r,
                           int n, int nombreTests) {
        std::cout << "\n" << nom << "\n";
        std::cout << "  Insertion (" << n << ") : " << r.insertion << " ms\n";
        std::cout << "  Recherche presente (" << nombreTests << ") : " << r.recherchePresente << " ms\n";
        std::cout << "  Recherche absente (" << nombreTests << ") : " << r.rechercheAbsente << " ms\n";
        std::cout << "  Suppression (" << nombreTests << ") : " << r.suppression << " ms\n";
        std::cout << "  Verification : trouves=" << r.trouves
                  << ", absents=" << r.absents
                  << ", supprimes=" << r.supprimes
                  << ", taille finale=" << r.tailleFinale << "\n";

        if (r.trouves != nombreTests || r.absents != nombreTests ||
            r.supprimes != nombreTests || r.tailleFinale != n - nombreTests)
            std::cout << "  ERREUR : verification echouee !\n";
    }

    void comparer(const std::string& titre, const std::vector<int>& nombres) {
        std::vector<int> questions = creerQuestions(nombres);
        int n = static_cast<int>(nombres.size());
        int nombreTests = static_cast<int>(questions.size());

        std::cout << "\n========== " << titre << " ==========\n";
        std::cout << "N = " << n << " ; tests par operation = " << nombreTests << "\n";

        Resultats abr = mesurer<ArbreBinaireRecherche>(nombres, questions);
        afficherResultats("ABR FROM SCRATCH", abr, n, nombreTests);

        Resultats mp = mesurer<MapRecherche>(nombres, questions);
        afficherResultats("STD::MAP", mp, n, nombreTests);
    }

public:
    ChronoTest() {
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "BENCHMARK ABR vs STD::MAP | temps en ms\n";
        std::cout << "Memes nombres et memes operations pour les deux structures.\n";
        std::cout << "Preparation et destruction exclues des temps.\n";
    }

    void testerAleatoire() {
        std::vector<int> nombres = creerNombres(1000000, true);
        comparer("SCENARIO 1 : ALEATOIRE (1 MILLION)", nombres);
    }

    void testerTrie() {
        std::vector<int> nombres = creerNombres(20000, false);
        comparer("SCENARIO 2 : TRIE CROISSANT (20 000)", nombres);
    }

    void testerMapTrie() {
        std::vector<int> nombres = creerNombres(1000000, false);
        std::vector<int> questions = creerQuestions(nombres);

        std::cout << "\n========== SCENARIO 3 : TRIE CROISSANT (MAP 1 MILLION) ==========\n";
        Resultats mp = mesurer<MapRecherche>(nombres, questions);
        afficherResultats("STD::MAP", mp, 1000000, static_cast<int>(questions.size()));
        std::cout << "\nFIN DES TESTS\n";
    }
};

#endif
