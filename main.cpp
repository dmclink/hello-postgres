#include <iostream>
#include <pqxx/pqxx>
#include <chrono>
#include <thread>
#include <cstdlib>

int main() {
    std::cout << "DATABASE_URL: " << std::getenv("DATABASE_URL") << std::endl;
    std::cout << "DATABASE_USER: " << std::getenv("DATABASE_USER") << std::endl;
    try {
        const char* db_url_env = std::getenv("DATABASE_URL");
        if (!db_url_env) {
            std::cerr << "Error: DATABASE_URL environment variable not set." << std::endl;
            return 1;
        }

        pqxx::connection conn(db_url_env);
        std::cout << "Connected to " << conn.dbname() << std::endl;

        {
            pqxx::work tx(conn);
            tx.exec(
                "CREATE TABLE IF NOT EXISTS heartbeat ("
                "id SERIAL PRIMARY KEY, "
                "timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
                ");"
            );
            tx.commit();
            std::cout << "Table 'heartbeat' ensured." << std::endl;
        }

        while (true) {
            try {
                pqxx::work tx(conn);
                // inserting with DEFAULT allows Postgres to handle the timestamp generation
                tx.exec("INSERT INTO heartbeat (timestamp) VALUES (DEFAULT);");
                tx.commit();
                
                std::cout << "Pulse sent to database." << std::endl;
            } catch (const std::exception &e) {
                std::cerr << "Transaction failed: " << e.what() << std::endl;
            }

            // sleep5 minutes
            std::this_thread::sleep_for(std::chrono::minutes(5));
        }

    } catch (const std::exception &e) {
        std::cerr << "Database error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
