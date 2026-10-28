#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>

#include "Structures.h"

using namespace std;
using namespace chrono;

// =====================================================
// 4. RESULTATS ET MESURE DES TEMPS
// =====================================================
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

// Meme protocole de mesure pour les deux structures.
template <class Structure>
Resultats mesurer(const vector<int>& nombres, const vector<int>& questions) {
    Structure structure;
    Resultats r;

    auto debut = steady_clock::now();
    for (int valeur : nombres) structure.inserer(valeur);
    auto fin = steady_clock::now();
    r.insertion = duration<double, milli>(fin - debut).count();

    debut = steady_clock::now();
    for (int valeur : questions)
        if (structure.rechercher(valeur)) r.trouves++;
    fin = steady_clock::now();
    r.recherchePresente = duration<double, milli>(fin - debut).count();

    // Les valeurs inserees sont impaires, donc valeur+1 est absente.
    debut = steady_clock::now();
    for (int valeur : questions)
        if (!structure.rechercher(valeur + 1)) r.absents++;
    fin = steady_clock::now();
    r.rechercheAbsente = duration<double, milli>(fin - debut).count();

    debut = steady_clock::now();
    for (int valeur : questions)
        if (structure.supprimer(valeur)) r.supprimes++;
    fin = steady_clock::now();
    r.suppression = duration<double, milli>(fin - debut).count();

    r.tailleFinale = structure.taille();
    return r;
}

// =====================================================
// 5. PREPARATION DES MEMES DONNEES
// =====================================================
vector<int> creerNombres(int n, bool aleatoire) {
    vector<int> nombres;
    for (int i = 0; i < n; i++)
        nombres.push_back(2 * i + 1); // Nombres impairs : 1, 3, 5...

    if (aleatoire) {
        mt19937 generateur(42); // Meme melange a chaque execution.
        shuffle(nombres.begin(), nombres.end(), generateur);
    }
    return nombres;
}

vector<int> creerQuestions(const vector<int>& nombres) {
    vector<int> questions;
    int quantite = static_cast<int>(nombres.size()) / 10;
    for (int i = 0; i < quantite; i++)
        questions.push_back(nombres[i * 10]);
    return questions;
}

void afficherResultats(const string& nom, const Resultats& r, int n, int q) {
    cout << "\n" << nom << "\n";
    cout << "  Insertion (" << n << ") : " << r.insertion << " ms\n";
    cout << "  Recherche presente (" << q << ") : " << r.recherchePresente << " ms\n";
    cout << "  Recherche absente (" << q << ") : " << r.rechercheAbsente << " ms\n";
    cout << "  Suppression (" << q << ") : " << r.suppression << " ms\n";
    cout << "  Verification : trouves=" << r.trouves
         << ", absents=" << r.absents
         << ", supprimes=" << r.supprimes
         << ", taille finale=" << r.tailleFinale << "\n";

    if (r.trouves != q || r.absents != q || r.supprimes != q ||
        r.tailleFinale != n - q)
        cout << "  ERREUR : verification echouee !\n";
}

// Comparaison ABR et Map sur les memes nombres.
void comparer(const string& scenario, const vector<int>& nombres) {
    vector<int> questions = creerQuestions(nombres);
    int n = static_cast<int>(nombres.size());
    int q = static_cast<int>(questions.size());

    cout << "\n========== " << scenario << " ==========\n";
    cout << "N = " << n << " ; tests par operation = " << q << "\n";

    Resultats abr = mesurer<ArbreBinaireRecherche>(nombres, questions);
    afficherResultats("ABR FROM SCRATCH", abr, n, q);

    Resultats mp = mesurer<MapRecherche>(nombres, questions);
    afficherResultats("STD::MAP", mp, n, q);
}

int main() {
    const int UN_MILLION = 1000000;
    const int VINGT_MILLE = 20000;

    cout << fixed << setprecision(3);
    cout << "BENCHMARK ABR vs STD::MAP (temps en ms)\n";
    cout << "Meme nombres, meme ordre, memes recherches et suppressions.\n";
    cout << "Les temps excluent la preparation et la destruction.\n";

    // TEST 1 : 1 million de nombres dans un ordre aleatoire.
    vector<int> aleatoires = creerNombres(UN_MILLION, true);
    comparer("SCENARIO 1 : ALEATOIRE (1 MILLION)", aleatoires);

    // TEST 2 : 20 000 nombres croissants (ABR + Map).
    // ABR non equilibre : inserer 1M en ordre croissant est O(n^2).
    vector<int> triesPetits = creerNombres(VINGT_MILLE, false);
    comparer("SCENARIO 2 : TRIE CROISSANT (20 000)", triesPetits);

    // TEST 3 : 1 million de nombres croissants (Map seule).
    vector<int> triesGrand = creerNombres(UN_MILLION, false);
    vector<int> questions = creerQuestions(triesGrand);

    cout << "\n========== SCENARIO 3 : TRIE CROISSANT (MAP 1 MILLION) ==========\n";
    Resultats mp = mesurer<MapRecherche>(triesGrand, questions);
    afficherResultats("STD::MAP", mp, UN_MILLION, static_cast<int>(questions.size()));

    cout << "\nFIN DES TESTS\n";
    return 0;
}
