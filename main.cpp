// main.cpp
#include <iostream>
#include <string>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/exception/exception.hpp>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;

// Global instance
mongocxx::instance instance{};

class MovieDatabase {
private:
    mongocxx::client client;
    mongocxx::database db;
    mongocxx::collection collection;
    bool isConnected;

    auto createRegexFilter(const std::string& field, const std::string& searchTerm) {
        return document{} << field << open_document
                         << "$regex" << ".*" + searchTerm + ".*"
                         << "$options" << "i"
                         << close_document
                         << finalize;
    }

    void printFormattedJson(const bsoncxx::document::view& doc) {
        std::string json_str = bsoncxx::to_json(doc);
        int indent = 0;
        bool in_quotes = false;

        std::cout << "\n";
        for (char c : json_str) {
            if (c == '"') {
                in_quotes = !in_quotes;
                std::cout << c;
            }
            else if (!in_quotes) {
                if (c == '{' || c == '[') {
                    std::cout << c << '\n' << std::string(++indent * 2, ' ');
                }
                else if (c == '}' || c == ']') {
                    std::cout << '\n' << std::string(--indent * 2, ' ') << c;
                }
                else if (c == ',') {
                    std::cout << c << '\n' << std::string(indent * 2, ' ');
                }
                else if (c == ':') {
                    std::cout << c << ' ';
                }
                else {
                    std::cout << c;
                }
            }
            else {
                std::cout << c;
            }
        }
        std::cout << "\n";
    }

public:
    MovieDatabase() : isConnected(false) {
        try {
            // Specificeer de juiste MongoDB connection string
            mongocxx::uri uri("mongodb://localhost:27017");
            client = mongocxx::client{uri};
            db = client["films_db"];
            collection = db["films"];
            isConnected = true;

            // Test de verbinding
            client.list_database_names();
        }
        catch (const mongocxx::exception& e) {
            std::cerr << "MongoDB Error: " << e.what() << std::endl;
            isConnected = false;
        }
    }

    bool isConnectionValid() const {
        return isConnected;
    }

    void searchByTitle(const std::string& title) {
        if (!isConnected) {
            std::cerr << "Geen verbinding met database" << std::endl;
            return;
        }

        try {
            auto filter = createRegexFilter("titel", title);
            auto result = collection.find_one(filter.view());

            if (result) {
                std::cout << "\nGevonden film:" << std::endl;
                printFormattedJson(*result);
            } else {
                std::cout << "Geen film gevonden met titel: " << title << std::endl;
            }
        }
        catch (const mongocxx::exception& e) {
            std::cerr << "Zoekfout: " << e.what() << std::endl;
        }
    }

    void searchByGenre(const std::string& genre) {
        if (!isConnected) {
            std::cerr << "Geen verbinding met database" << std::endl;
            return;
        }

        try {
            auto filter = createRegexFilter("genre", genre);
            auto cursor = collection.find(filter.view());

            bool found = false;
            std::cout << "\nFilms in genre " << genre << ":" << std::endl;
            for (auto&& doc : cursor) {
                printFormattedJson(doc);
                found = true;
            }

            if (!found) {
                std::cout << "Geen films gevonden in dit genre." << std::endl;
            }
        }
        catch (const mongocxx::exception& e) {
            std::cerr << "Zoekfout: " << e.what() << std::endl;
        }
    }

    void searchByActor(const std::string& actor) {
        if (!isConnected) {
            std::cerr << "Geen verbinding met database" << std::endl;
            return;
        }

        try {
            auto filter = createRegexFilter("acteurs", actor);
            auto cursor = collection.find(filter.view());

            bool found = false;
            std::cout << "\nFilms met acteur " << actor << ":" << std::endl;
            for (auto&& doc : cursor) {
                printFormattedJson(doc);
                found = true;
            }

            if (!found) {
                std::cout << "Geen films gevonden met deze acteur." << std::endl;
            }
        }
        catch (const mongocxx::exception& e) {
            std::cerr << "Zoekfout: " << e.what() << std::endl;
        }
    }
};

void displayMenu() {
    std::cout << "\n=== Film Database Menu ===" << std::endl;
    std::cout << "1. Zoeken op titel" << std::endl;
    std::cout << "2. Zoeken op genre" << std::endl;
    std::cout << "3. Zoeken op acteur" << std::endl;
    std::cout << "4. Afsluiten" << std::endl;
    std::cout << "Keuze: ";
}

int main() {
    MovieDatabase db;

    if (!db.isConnectionValid()) {
        std::cerr << "Kon geen verbinding maken met de database. Controleer of MongoDB draait." << std::endl;
        return 1;
    }

    int choice;
    std::string searchTerm;

    while (true) {
        displayMenu();

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ongeldige invoer. Voer een nummer in." << std::endl;
            continue;
        }
        std::cin.ignore();

        if (choice == 4) break;

        if (choice < 1 || choice > 4) {
            std::cout << "Ongeldige keuze" << std::endl;
            continue;
        }

        std::cout << "Voer zoekterm in: ";
        std::getline(std::cin, searchTerm);

        switch (choice) {
            case 1:
                db.searchByTitle(searchTerm);
                break;
            case 2:
                db.searchByGenre(searchTerm);
                break;
            case 3:
                db.searchByActor(searchTerm);
                break;
        }
    }

    return 0;
}